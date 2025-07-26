#pragma once

#include <necrowarp/entities/evil/lesser/abomination.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t abomination_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ non_evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::Wander, position };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void abomination_t::killed(offset_t position) noexcept {
		object_registry<MapType>.spill(position, flesh_t{});
	}

	template<map_type_e MapType> inline i8 abomination_t::devoured(offset_t position) noexcept { return 1; }
} // namespace necrowarp
