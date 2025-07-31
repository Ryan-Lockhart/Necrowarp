#pragma once

#include <necrowarp/entities/good/adventurer.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t adventurer_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_NON_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		if (cauto paladin_pos{ entity_goal_map<MapType, paladin_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; paladin_pos.has_value() && entity_goal_map<MapType, paladin_t>[paladin_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, paladin_pos.value() };
		}

		if (cauto mercenary_pos{ entity_goal_map<MapType, mercenary_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; mercenary_pos.has_value() && entity_goal_map<MapType, mercenary_t>[mercenary_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, mercenary_pos.value() };
		}

		if (cauto evil_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; evil_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, evil_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> adventurer_t::die(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::AdventurersSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Adventurer });
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Adventurer });
		}

		player.receive_death_boon<adventurer_t>();

		scorekeeper.add(entity_e::Adventurer);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
