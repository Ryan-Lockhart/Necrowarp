#pragma once

#include <necrowarp/entities/good/paladin.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t paladin_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || game_map<MapType>[current_position].solid || entity_registry<MapType>.dependent empty<ALL_NON_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		if (cauto evil_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; evil_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, evil_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> paladin_t::die(offset_t position) noexcept {
		++steam_stats::stats<stat_e::PaladinsSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, metal_t{ galvanisation_e::Shimmering });
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Paladin });
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, metal_t{ galvanisation_e::Shimmering });
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Paladin });
		}

		player.receive_death_boon<paladin_t>();

		scorekeeper.add(entity_e::Paladin);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
