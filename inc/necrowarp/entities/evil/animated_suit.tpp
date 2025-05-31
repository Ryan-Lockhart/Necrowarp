#pragma once

#include <necrowarp/entities/evil/animated_suit.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t animated_suit_t::think() const noexcept {
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

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	inline void animated_suit_t::die() noexcept {
		object_registry.add<true>(metal_t{ position, state });

		fluid_map[position] += fluid_type<animated_suit_t>::type;
	}
} // namespace necrowarp
