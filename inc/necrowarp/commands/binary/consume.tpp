#pragma once

#include <necrowarp/commands/binary/consume.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, consume_t>::process() const noexcept {
		const entity_group_e entity_group{ entity_registry<MapType>.at(target_position) };

		const entity_e entity_target{ determine_target<EntityType>(entity_group) };
		
		switch (entity_target) {
			case entity_e::Skeleton: {
				const i8 armor_boon = entity_registry<MapType>.dependent at<skeleton_t>(target_position)->armor_boon();

				entity_registry<MapType>.dependent remove<skeleton_t>(target_position);

				++steam_stats::stats<steam_stat_e::SkeletonsConsumed>;

				entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);
				
				player.bolster_armor(armor_boon + player.max_armor() / 8);

				warped_from = std::nullopt;
				
				return;
			} case entity_e::Bonespur: {
				const i8 armor_boon = entity_registry<MapType>.dependent at<bonespur_t>(target_position)->armor_boon();

				entity_registry<MapType>.dependent remove<bonespur_t>(target_position);

				++steam_stats::stats<steam_stat_e::BonespursConsumed>;

				entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);
				
				player.bolster_armor(armor_boon + player.max_armor() / 4);

				warped_from = std::nullopt;
				
				return;
			} default: {
				break;
			}
		}

		const object_group_e object_group{ object_registry<MapType>.at(target_position) };

		const object_e object_target{ determine_target<EntityType>(object_group) };
		
		switch (object_target) {
			case object_e::Bones: {
				const decay_e state{ object_registry<MapType>.dependent at<bones_t>(target_position)->state };

				object_registry<MapType>.dependent remove<bones_t>(target_position);
				entity_registry<MapType>.add(target_position, skeleton_t{ state });

				++steam_stats::stats<steam_stat_e::BonesConsumed>;

				if (state == decay_e::Fresh) {
					player.receive_skull_boon();
				}

				chaotic_warp_t::execute<MapType>(source_position, true);
				
				return;
			} default: {
				break;
			}
		}
	}
} // namespace necrowarp
