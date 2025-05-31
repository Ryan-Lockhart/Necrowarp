#pragma once

#include <necrowarp/entities/evil/bloodhound.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t bloodhound_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry.contains<ALL_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ evil_goal_map.descend<zone_region_t::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry.contains<ALL_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto further_descent_pos{ evil_goal_map.descend<zone_region_t::Interior>(descent_pos.value(), entity_registry) };

		if (!further_descent_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		return command_pack_t{ command_e::Move, position, further_descent_pos.value() };
	}

	inline void bloodhound_t::die() noexcept {
		fluid_map[position] += fluid_type<bloodhound_t>::type;
	}
} // namespace necrowarp
