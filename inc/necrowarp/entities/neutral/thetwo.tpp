#pragma once

#include <necrowarp/entities/neutral/thetwo.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t thetwo_t::think(offset_t position) const noexcept {
		if (can_shed() || is_shedding()) {
			return command_pack_t{ command_e::Shed, position };
		}

		const bool is_hungry{ protein < max_protein() };
		
		const bool threat_nearby{ entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_NEUTRAL>(position) };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (object_registry<MapType>.dependent contains<flesh_t>(current_position) && is_hungry && !threat_nearby) {
				return command_pack_t{ command_e::Devour, position, current_position };
			}

			if (entity_registry<MapType>.dependent contains<ALL_NON_NEUTRAL>(current_position)) {
				const entity_e target{ determine_target<thetwo_t>(entity_registry<MapType>.at(current_position)) };

				if (!can_devour(target)) {
					return command_pack_t{ command_e::Clash, position, current_position };
				} else {
					return command_pack_t{ command_e::Devour, position, current_position };
				}
			}
		}

		if (can_metabolise()) {
			return command_pack_t{ command_e::Metabolise, position };
		}

		if (!object_registry<MapType>.dependent empty<flesh_t>() && is_hungry) {
			cauto meal_pos{ object_goal_map<MapType, flesh_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (meal_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, meal_pos.value() };
			} else if (object_registry<MapType>.dependent contains<flesh_t>(position)) {

			}
		}

		cauto descent_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void thetwo_t::killed(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::ThetwoSlain>;

		const u8 droppings{ get_droppings() };

		if (droppings <= 0) {
			return;
		}

		object_registry<MapType>.spill(position, bones_t{}, droppings);
		object_registry<MapType>.spill(position, flesh_t{}, droppings);

		player.receive_death_boon<thetwo_t>(droppings);

		scorekeeper.add(entity_e::Thetwo, droppings);
	}

	template<map_type_e MapType> inline i8 thetwo_t::devoured(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::ThetwoSlain>;

		const u8 droppings{ get_droppings() };

		if (droppings <= 0) {
			return 0;
		}

		object_registry<MapType>.spill(position, bones_t{}, droppings);

		player.receive_death_boon<thetwo_t>(droppings);

		scorekeeper.add(entity_e::Thetwo, droppings);

		return droppings;
	}
} // namespace necrowarp
