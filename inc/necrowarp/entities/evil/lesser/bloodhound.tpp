#pragma once

#include <necrowarp/entities/evil/lesser/bloodhound.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t bloodhound_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		if (cauto good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; good_pos.has_value()) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ good_pos.value() + offset };

				if (entity_registry<MapType>.dependent empty<ALL_NON_EVIL>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Lunge, position, good_pos.value(), current_position };
			}

			if (cauto further_good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; further_good_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, further_good_pos.value() };
			}

			return command_pack_t{ command_e::Move, position, good_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ neutral_pos.value() + offset };

				if (entity_registry<MapType>.dependent empty<ALL_NON_EVIL>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Lunge, position, neutral_pos.value(), current_position };
			}

			if (cauto further_neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; further_neutral_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, further_neutral_pos.value() };
			}

			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> bloodhound_t::die(offset_t position) noexcept {
		return death_info_t<Death>{ true };
	}
} // namespace necrowarp
