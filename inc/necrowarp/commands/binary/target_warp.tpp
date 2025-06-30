#pragma once

#include <necrowarp/commands/binary/target_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	extern ptr<player_t> player;

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, target_warp_t>::process() const noexcept {
		if (!player_exists()) {
			error_log.add("the player was lost to the abyss!");

			return;
		}

		if (!player->can_perform(discount_e::TargetWarp)) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<steam_stat_e::TargetWarps, i32>;

		player->pay_cost(discount_e::TargetWarp);

		entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

		steam_stats::stats<steam_stat_e::MetersWarped, f32> += offset_t::distance<f32>(source_position, player->position);

		draw_warp_cursor = false;		
	}
} // namespace necrowarp
