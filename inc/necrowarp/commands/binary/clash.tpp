#pragma once

#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool instigate(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<InitiatorType>::value) {
			if (!coinflip(random_engine)) {
				return false;
			}
		}

		const i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (!is_fodder<VictimType>::value) {
			if (victim.can_survive(damage)) {
				victim.receive_damage(damage);

				if constexpr (is_bleeder<VictimType>::value) {
					fluid_map[victim.position] += fluid_type<VictimType>::type;
				}

				return false;
			}
		}

		return true;
	}

	template<CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool retaliate(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<VictimType>::value) {
			if (!coinflip(random_engine)) {
				return false;
			}
		}

		const i8 damage{ victim.get_damage(to_entity_enum<InitiatorType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (!is_fodder<InitiatorType>::value) {
			if (initiator.can_survive(damage)) {
				initiator.receive_damage(damage);

				if constexpr (is_bleeder<InitiatorType>::value) {
					fluid_map[initiator.position] += fluid_type<InitiatorType>::type;
				}

				return false;
			}
		}

		return true;
	}

	template<CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool reflect(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<InitiatorType>::value) {
			if (!coinflip(random_engine)) {
				return false;
			}
		}

		const i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (!is_fodder<InitiatorType>::value) {
			if (initiator.can_survive(damage)) {
				initiator.receive_damage(damage);

				if constexpr (is_bleeder<InitiatorType>::value) {
					fluid_map[initiator.position] += fluid_type<InitiatorType>::type;
				}

				return false;
			}
		}

		return true;
	}

	template<CombatantEntity EntityType> inline void entity_command_t<EntityType, clash_t>::process() const noexcept {
		ptr<EntityType> initiator_ptr{ entity_registry.at<EntityType>(source_position) };

		if (initiator_ptr == nullptr) {
			return;
		}

		ref<EntityType> initiator{ *initiator_ptr };
		
		const entity_e victim_enum{ determine_target<EntityType>(entity_registry.at(target_position)) };

		if (victim_enum == entity_e::None) {
			return;
		}

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			using victim_type = to_entity_type<cval>::type;

			if constexpr (!is_null_entity<victim_type>::value && !std::is_same<EntityType, victim_type>::value) {
				ptr<victim_type> victim_ptr{ entity_registry.at<victim_type>(target_position) };

				if (victim_ptr == nullptr) {
					return;
				}

				ref<victim_type> victim{ *victim_ptr };

				const bool target_killed{ instigate(initiator, victim) };

				const bool source_killed{ [&]() -> bool {
					if constexpr (cval == entity_e::Bonespur) {
						return reflect(initiator, victim);
					} else {
						return retaliate(initiator, victim);
					}
				}()};

				if (target_killed) {
					victim.die();

					if constexpr (is_npc_entity<victim_type>::value) {
						entity_registry.remove<victim_type>(victim.position);
					}

					switch (to_entity_enum<EntityType>::value) {
						case entity_e::Player: {
							++game_stats.player_kills;
							++steam_stats::stats<steam_stat_e::PlayerKills, i32>;

							draw_warp_cursor = false;
							break;
						} case entity_e::Skeleton:
						  case entity_e::Cultist:
						  case entity_e::Bloodhound:
						  case entity_e::AnimatedSuit:
						  case entity_e::Bonespur:
						  case entity_e::Wraith:
						  case entity_e::FleshGolem:
						  case entity_e::DeathKnight: {
							++game_stats.minion_kills;
							++steam_stats::stats<steam_stat_e::MinionKills, i32>;
							break;
						} default: {
							break;
						}
					}
				}

				if (source_killed) {
					initiator.die();

					if constexpr (is_npc_entity<EntityType>::value) {
						entity_registry.remove<EntityType>(initiator.position);
					}

					switch (victim_enum) {
						case entity_e::Player: {
							++game_stats.player_kills;
							++steam_stats::stats<steam_stat_e::PlayerKills, i32>;
							break;
						} case entity_e::Skeleton:
						  case entity_e::Cultist:
						  case entity_e::Bloodhound:
						  case entity_e::AnimatedSuit:
						  case entity_e::Bonespur:
						  case entity_e::Wraith:
						  case entity_e::FleshGolem:
						  case entity_e::DeathKnight: {
							++game_stats.minion_kills;
							++steam_stats::stats<steam_stat_e::MinionKills, i32>;
							break;
						} default: {
							break;
						}
					}
				}
			}			
		}, victim_enum);
	}
} // namespace necrowarp
