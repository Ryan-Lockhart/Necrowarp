#pragma once

#include <necrowarp/commands/binary/loose.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline bool ranger_t::loose(offset_t position) noexcept {
		if (!can_loose()) {
			return false;
		}

		nocked = false;

		const bool missed{ ranger_t::fumble(random_engine) };
		const bool snapped{ missed && arrow_t::snap(random_engine) };

		if (!snapped) {
			if (object_registry<MapType>.dependent contains<arrow_t>(position)) {
				ptr<arrow_t> maybe_arrow{ object_registry<MapType>.dependent at<arrow_t>(position) };

				if (maybe_arrow != nullptr && !maybe_arrow->is_full()) {
					++(*maybe_arrow);
				}

				return missed;
			}

			object_registry<MapType>.spill(position, arrow_t{});
		}

		return missed;
	}

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, loose_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr || !ranger_t::in_range(source_position, target_position) || !maybe_ranger->can_loose()) {
			return;
		}

		ref<ranger_t> ranger{ *maybe_ranger };

		const entity_e target{ determine_target<ranger_t>(entity_registry<MapType>.at(target_position)) };
		
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None) {
				using entity_type = to_entity_type<cval>::type;

				ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(target_position) };

				if (maybe_target == nullptr) {
					return;
				}

				ref<entity_type> target{ *maybe_target };
				
				cauto try_bleed = [&] {
					if constexpr (is_bleeder_v<entity_type>) {
						if constexpr (is_bleeder<entity_type>::conditional) {
							if (!target.can_bleed()) {
								return;
							}
						}

						constexpr fluid_e fluid{ is_bleeder<entity_type>::type };
						
						spill_fluid<MapType>(target_position, fluid);
					}
				};

				if (!ranger.loose<MapType>(target_position)) {
					return;
				}

				const i8 damage{ ranger.get_damage(cval) };

				if constexpr (!is_fodder<entity_type>::value) {
					if (target.can_survive(damage)) {
						target.receive_damage(damage);

						try_bleed();

						return;
					}
				}

				try_bleed();

				if constexpr (is_player<entity_type>::value) {
					target.dependent die<MapType>();
				} else {
					target.dependent die<MapType>(target_position);
				}

				if constexpr (is_npc_entity<entity_type>::value) {
					entity_registry<MapType>.dependent remove<entity_type>(target_position);
				}
			}
		}, target);
	}
} // namespace necrowarp
