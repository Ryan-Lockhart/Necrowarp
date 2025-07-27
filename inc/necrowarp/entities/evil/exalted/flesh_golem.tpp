#pragma once

#include <necrowarp/entities/evil/exalted/flesh_golem.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t flesh_golem_t::think(offset_t position) const noexcept {
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

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> flesh_golem_t::die(offset_t position) noexcept {
		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, flesh_t{});
		}

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, static_cast<i8>(investiture * ProteinRatio) };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
