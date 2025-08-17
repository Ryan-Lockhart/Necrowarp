#pragma once

#include <necrowarp/commands/binary/descend.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, descend_t>::process() const noexcept {
		if (game_stats.game_depth >= globals::MaximumDepth) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<stat_e::MetersMoved>;

		steam_stats::unlock(achievement_e::DescendDownLadder);

		descent_flag = true;
	}
} // namespace necrowarp
