#pragma once

#include <necrowarp/entities/neutral/plonzo.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t plonzo_t::think(offset_t position) const noexcept {
		const bool is_friendly{ demeanor == demeanor_e::Playful || demeanor == demeanor_e::Jubilant };

		if (!entity_registry<MapType>.dependent empty<player_t>() && is_friendly) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (entity_registry<MapType>.dependent contains<player_t>(current_position)) {
					return command_pack_t{ command_e::Nuzzle, position, current_position };
				}
			}

			cauto player_pos{ entity_goal_map<MapType, player_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (player_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, player_pos.value() };
			}
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> plonzo_t::die(offset_t position) noexcept {
		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ false, 0 };
		} else {
			return death_info_t<Death>{ false };
		}
	}
} // namespace necrowarp
