#pragma once

#include <necrowarp/entities/good/thetwo.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t thetwo_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.template contains<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map<MapType>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ descent_pos.value() + offset };

			if (!entity_registry<MapType>.template contains<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Lunge, position, descent_pos.value(), current_position };
		}

		cauto further_descent_pos{ good_goal_map<MapType>.template descend<zone_region_e::Interior>(descent_pos.value(), entity_registry<MapType>) };

		if (!further_descent_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		return command_pack_t{ command_e::Move, position, further_descent_pos.value() };
	}

	template<map_type_e MapType> inline void thetwo_t::die() noexcept {
		object_registry<MapType>.template add<true>(skull_t{ position });
		object_registry<MapType>.template add<true>(flesh_t{ position });

		fluid_map<MapType>[position] += fluid_type<thetwo_t>::type;

		player.receive_death_boon<thetwo_t>();

		++steam_stats::stats<steam_stat_e::ThetwoSlain, i32>;

		scorekeeper.add(entity_e::Thetwo);
	}
} // namespace necrowarp
