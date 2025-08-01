#pragma once

#include <necrowarp/entities/good/mist_lady.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t mist_lady_t::think(offset_t position) const noexcept {
		cauto descent_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			cauto flee_pos{ non_good_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (!flee_pos.has_value()) {
				return command_pack_t{ command_e::Wander, position };
			}

			return command_pack_t{ command_e::Move, position, flee_pos.value() };	
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> mist_lady_t::die(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::MistLadiesSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::MistLady });
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::MistLady });
		}

		player.receive_death_boon<mist_lady_t>();

		scorekeeper.add(entity_e::MistLady);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
