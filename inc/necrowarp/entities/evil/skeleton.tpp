#pragma once

#include <necrowarp/entities/evil/skeleton.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t skeleton_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry.contains<ALL_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ evil_goal_map.descend<zone_region_e::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	inline void skeleton_t::die() noexcept {
		if (state != decay_e::Rotted) {
			object_registry.add<true>(skull_t{ position, decay(state) });
		} else {
			fluid_map[position] += fluid_type<skeleton_t>::type;
		}
	}
} // namespace necrowarp
