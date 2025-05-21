#pragma once

#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, clash_t>::process() noexcept {
		const entity_group_e source_types{ entity_registry.at(source_position) };
		const entity_group_e target_types{ entity_registry.at(target_position) };

		i8 source_damage{ 0 };

		switch (source_types) {
			case entity_e::Player:
			case entity_e::Skeleton:
			case entity_e::Cultist:
			case entity_e::FleshGolem:
			case entity_e::Adventurer: {
				source_damage = 1;
				break;
			} case entity_e::Wraith:
			  case entity_e::Paladin: {
				source_damage = 2;
				break;
			} case entity_e::Bloodhound: {
				source_damage = target_types == entity_e::Paladin ? 0 : 1;
				break;
			} default: {
				return;
			}
		}

		i8 target_damage{ 0 };

		switch (target_types) {
			case entity_e::Player:
			case entity_e::Skeleton:
			case entity_e::Cultist:
			case entity_e::FleshGolem:
			case entity_e::Adventurer: {
				target_damage = 1;
				break;
			} case entity_e::Wraith:
			  case entity_e::Paladin: {
				target_damage = 2;
				break;
			} case entity_e::Bloodhound: {
				target_damage = source_type == entity_e::Paladin ? 0 : 1;
				break;
			} default: {
				return;
			}
		}

		const bool target_killed{
			magic_enum::enum_switch([this, source_damage](auto val) -> bool {
				constexpr entity_e cval{ val };

				if constexpr (cval == entity_e::None || cval == entity_e::Skull || cval == entity_e::Ladder) {
					return false;
				}

				return process_clash<cval>(target_position, source_damage);
			}, source_types)
		};

		if (target_killed) {
			switch (source_types) {
				case entity_e::Player: {
					++game_stats.player_kills;
					draw_warp_cursor = false;
					break;
				} case entity_e::Skeleton:
				  case entity_e::Cultist:
				  case entity_e::Bloodhound:
				  case entity_e::Wraith:
				  case entity_e::FleshGolem: {
					++game_stats.minion_kills;
					break;
				} default: {
					break;
				}
			}
		}

		const bool source_killed{
			magic_enum::enum_switch([this, target_damage](auto val) -> bool {
				constexpr entity_e cval{ val };

				if constexpr (cval == entity_e::None || cval == entity_e::Skull || cval == entity_e::Ladder) {
					return false;
				}

				return process_clash<cval>(source_position, target_damage);
			}, source_types)
		};

		if (source_killed) {
			switch (target_types) {
				case entity_e::Player: {
					++game_stats.player_kills;
					++steam_stats::stats<steam_stat_e::PlayerKills, i32>;
					break;
				} case entity_e::Skeleton:
				  case entity_e::Cultist:
				  case entity_e::Bloodhound:
				  case entity_e::Wraith:
				  case entity_e::FleshGolem: {
					++game_stats.minion_kills;
					++steam_stats::stats<steam_stat_e::MinionKills, i32>;
					break;
				} default: {
					break;
				}
			}
		}
	}
} // namespace necrowarp
