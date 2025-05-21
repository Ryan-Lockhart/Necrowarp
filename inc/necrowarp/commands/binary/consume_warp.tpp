#pragma once

#include <necrowarp/commands/binary/consume_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, consume_warp_t>::process() noexcept {
		const entity_group_e target_types{ entity_registry.at(target_position) };

		if (target_types != entity_e::Skull && !player.can_perform(discount_e::TargetWarp)) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<steam_stat_e::TargetWarps, i32>;

		switch (target_types) {
			case entity_e::Skull: {
				const decay_e state{ entity_registry.at<skull_t>(target_position)->state };

				const i8 boon{ state == decay_e::Fresh ? player_t::SkullBoon : i8{ 0 } };

				if (!player.can_perform(discount_e::TargetWarp, boon)) {
					player_turn_invalidated = true;

					return;
				}

				entity_registry.remove<skull_t>(target_position);
				entity_registry.add(skeleton_t{ target_position, state });

				++steam_stats::stats<steam_stat_e::SkullsConsumed, i32>;

				player.pay_cost(discount_e::TargetWarp, boon);

				entity_registry.random_warp(source_position);

				return;
			} case entity_e::Skeleton: {
				const i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

				entity_registry.remove<skeleton_t>(target_position);

				++steam_stats::stats<steam_stat_e::SkeletonsConsumed, i32>;

				entity_registry.update<EntityType>(source_position, target_position);

				player.pay_cost(discount_e::TargetWarp);
				player.bolster_armor(armor_boon + player.max_armor() / 8);

				draw_warp_cursor = false;
				return;
			} default: {
				player_turn_invalidated = true;

				return;
			}
		}
	}
} // namespace necrowarp
