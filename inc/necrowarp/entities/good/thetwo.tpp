#pragma once

#include <necrowarp/entities/good/thetwo.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t thetwo_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map<MapType>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ descent_pos.value() + offset };

			if (entity_registry<MapType>.dependent empty<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Lunge, position, descent_pos.value(), current_position };
		}

		cauto further_descent_pos{ good_goal_map<MapType>.dependent descend<zone_region_e::Interior>(descent_pos.value(), entity_registry<MapType>) };

		if (!further_descent_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		return command_pack_t{ command_e::Move, position, further_descent_pos.value() };
	}

	template<map_type_e MapType> inline void thetwo_t::die() noexcept {
		const u8 droppings{ get_droppings() };

		object_registry<MapType>.spill(skull_t{ position }, droppings);
		object_registry<MapType>.spill(flesh_t{ position }, droppings);

		spill_fluid<MapType>(position, fluid_type<thetwo_t>::type, droppings);

		if (player_exists()) {
			player->receive_death_boon<thetwo_t>(droppings);
		}

		++steam_stats::stats<steam_stat_e::ThetwoSlain, i32>;

		scorekeeper.add(entity_e::Thetwo, droppings);
	}
} // namespace necrowarp
