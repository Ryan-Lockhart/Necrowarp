#pragma once

#include <necrowarp/commands/binary/loose.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline void ranger_t::loose(offset_t position) noexcept {
		if (!can_loose()) {
			return;
		}

		if (!arrow_t::snap(random_engine)) {
			object_registry<MapType>.add(arrow_t{ position });
		}

		knocked = false;
	}

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, loose_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.template at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr || !maybe_ranger->in_range(target_position) || !maybe_ranger->can_loose()) {
			return;
		}

		const entity_e target{ determine_target(entity_registry<MapType>.at(target_position)) };
		
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None) {
				using entity_type = to_entity_type<cval>::type;

				ptr<entity_type> maybe_target{ entity_registry<MapType>.template at<entity_type>(target_position) };

				if (maybe_target == nullptr) {
					return;
				}

				maybe_ranger->loose<MapType>(target_position);

				const i8 damage{ maybe_ranger->get_damage(cval) };

				if constexpr (!is_fodder<entity_type>::value) {
					if (maybe_target->can_survive(damage)) {
						maybe_target->receive_damage(damage);

						if constexpr (is_bleeder<entity_type>::value) {
							constexpr fluid_e fluid{ fluid_type<entity_type>::type };

							fluid_map<MapType>[maybe_target->position] += fluid;
						}

						return;
					}
				}

				maybe_target->template die<MapType>();

				if constexpr (is_npc_entity<entity_type>::value) {
					entity_registry<MapType>.template remove<entity_type>(target_position);
				}
			}
		}, target);
	}
} // namespace necrowarp
