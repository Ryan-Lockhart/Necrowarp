#pragma once

#include <necrowarp/commands/binary/consume_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, consume_warp_t>::process() const noexcept {
		const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(target_position) };

		if (maybe_entity.has_value()) {
			if (!player.can_perform(grimoire_e::PreciseWarp) || !is_interactable<EntityType>(maybe_entity.value())) {
				player_turn_invalidated = true;

				return;
			}

			const entity_e entity_target{ maybe_entity.value() };

			switch (entity_target) {
				case entity_e::Skeleton: {
					const i8 armor_boon = entity_registry<MapType>.dependent at<skeleton_t>(target_position)->armor_boon();

					entity_registry<MapType>.dependent remove<skeleton_t>(target_position);

					++steam_stats::stats<steam_stat_e::SkeletonsConsumed>;

					entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

					++steam_stats::stats<steam_stat_e::PreciseWarps>;

					player.pay_cost(grimoire_e::PreciseWarp);

					literature::use(grimoire_e::PreciseWarp);

					player.bolster_armor(armor_boon + player.max_armor() / 8);

					warped_from = std::nullopt;

					return;
				} case entity_e::Bonespur: {
					const i8 armor_boon = entity_registry<MapType>.dependent at<bonespur_t>(target_position)->armor_boon();

					entity_registry<MapType>.dependent remove<bonespur_t>(target_position);

					++steam_stats::stats<steam_stat_e::BonespursConsumed>;

					entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

					++steam_stats::stats<steam_stat_e::PreciseWarps>;

					player.pay_cost(grimoire_e::PreciseWarp);

					literature::use(grimoire_e::PreciseWarp);

					player.bolster_armor(armor_boon + player.max_armor() / 4);

					warped_from = std::nullopt;

					return;
				} default: {
					break;
				}
			}
		}

		const object_group_e object_group{ object_registry<MapType>.at(target_position) };

		const object_e object_target{ determine_target<EntityType>(object_group) };

		switch (object_target) {
			case object_e::Bones: {
				const decay_e state{ object_registry<MapType>.dependent at<bones_t>(target_position)->state };

				const i8 boon{ state == decay_e::Fresh ? player_t::BoneBoon : i8{ 0 } };

				if (!player.can_perform(grimoire_e::PreciseWarp, boon)) {
					player_turn_invalidated = true;

					return;
				}

				object_registry<MapType>.dependent remove<bones_t>(target_position);

				++steam_stats::stats<steam_stat_e::BonesConsumed>;

				entity_registry<MapType>.add(target_position, skeleton_t{ state });

				++steam_stats::stats<steam_stat_e::PreciseWarps>;

				player.pay_cost(grimoire_e::PreciseWarp, boon);

				literature::use(grimoire_e::PreciseWarp);

				chaotic_warp_t::execute<MapType>(source_position, true);

				return;
			} default: {
				break;
			}
		}

		player_turn_invalidated = true;
	}
} // namespace necrowarp
