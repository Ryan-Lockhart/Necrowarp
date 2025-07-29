#pragma once

#include <necrowarp/entities/evil/lesser/cultist.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t cultist_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		if (cauto player_pos{ entity_goal_map<MapType, player_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; player_pos.has_value() && entity_goal_map<MapType, player_t>[player_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, player_pos.value() };
		}

		if (cauto good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; good_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, good_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> cultist_t::die(offset_t position) noexcept {
		if constexpr (Death != death_e::Crushed || Death != death_e::Eradicated) {
			object_registry<MapType>.spill(position, bones_t{ decay_e::Rotted });
		}

		return death_info_t<Death>{ true };
	}
} // namespace necrowarp
