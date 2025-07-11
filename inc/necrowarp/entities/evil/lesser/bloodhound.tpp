#pragma once

#include <necrowarp/entities/evil/lesser/bloodhound.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t bloodhound_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Lunge, position, descent_pos.value(), current_position };
		}

		cauto further_descent_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(descent_pos.value(), entity_registry<MapType>) };

		if (!further_descent_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		return command_pack_t{ command_e::Move, position, further_descent_pos.value() };
	}

	template<map_type_e MapType> inline void bloodhound_t::killed(offset_t position) noexcept {}
} // namespace necrowarp
