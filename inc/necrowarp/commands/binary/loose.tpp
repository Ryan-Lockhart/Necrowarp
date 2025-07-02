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

					return missed;
				}
			}

			object_registry<MapType>.spill(arrow_t{ position });
		}

		return missed;
	}

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, loose_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr || !maybe_ranger->in_range(target_position) || !maybe_ranger->can_loose()) {
			return;
		}

		const entity_e target{ determine_target<ranger_t>(entity_registry<MapType>.at(target_position)) };
		
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None) {
				using entity_type = to_entity_type<cval>::type;

				ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(target_position) };

				if (maybe_target == nullptr) {
					return;
				}

				if (!maybe_ranger->loose<MapType>(target_position)) {
					return;
				}

				const i8 damage{ maybe_ranger->get_damage(cval) };

				if constexpr (!is_fodder<entity_type>::value) {
					if (maybe_target->can_survive(damage)) {
						maybe_target->receive_damage(damage);

						if constexpr (is_bleeder<entity_type>::value) {
							constexpr fluid_e fluid{ fluid_type<entity_type>::type };

							spill_fluid<MapType>(maybe_target->position, fluid);
						}

						return;
					}
				}

				maybe_target->template die<MapType>();

				if constexpr (is_npc_entity<entity_type>::value) {
					entity_registry<MapType>.dependent remove<entity_type>(target_position);
				}
			}
		}, target);
	}
} // namespace necrowarp
