#pragma once

#include <necrowarp/entities/evil/cultist.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t cultist_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.template contains<ALL_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ evil_goal_map<MapType>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void cultist_t::die() noexcept {
		object_registry<MapType>.spill(skull_t{ position, decay_e::Rotted });

		spill_fluid<MapType>(position, fluid_type<cultist_t>::type);
	}
} // namespace necrowarp
