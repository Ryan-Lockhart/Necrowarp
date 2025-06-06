#pragma once

#include <necrowarp/commands/binary/consume.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, consume_t>::process() const noexcept {
		const entity_group_e entity_group{ entity_registry.at(target_position) };

		const entity_e entity_target{ determine_target<EntityType>(entity_group) };
		
		switch (entity_target) {
			case entity_e::Skeleton: {
				const i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

				entity_registry.remove<skeleton_t>(target_position);

				++steam_stats::stats<steam_stat_e::SkeletonsConsumed, i32>;

				entity_registry.update<EntityType>(source_position, target_position);
				
				player.bolster_armor(armor_boon + player.max_armor() / 8);

				draw_warp_cursor = false;
				
				return;
			} default: {
				break;
			}
		}

		const object_group_e object_group{ object_registry.at(target_position) };

		const object_e object_target{ determine_target<EntityType>(object_group) };
		
		switch (object_target) {
			case object_e::Skull: {
				const decay_e state{ object_registry.at<skull_t>(target_position)->state };

				object_registry.remove<skull_t>(target_position);
				entity_registry.add(skeleton_t{ target_position, state });

				++steam_stats::stats<steam_stat_e::SkullsConsumed, i32>;

				if (state == decay_e::Fresh) {
					player.receive_skull_boon();
				}

				entity_registry.random_warp(source_position);
				
				return;
			} default: {
				break;
			}
		}
	}
} // namespace necrowarp
