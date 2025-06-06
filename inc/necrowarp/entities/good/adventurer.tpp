#pragma once

#include <necrowarp/entities/good/adventurer.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	inline command_pack_t adventurer_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry.contains<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map.descend<zone_region_e::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	inline void adventurer_t::die() noexcept {
		object_registry.add<true>(skull_t{ position });

		fluid_map[position] += fluid_type<adventurer_t>::type;

		player.receive_death_boon<adventurer_t>();

		++steam_stats::stats<steam_stat_e::AdventurersSlain, i32>;

		scorekeeper.add(entity_e::Adventurer);
	}
} // namespace necrowarp
