#pragma once

#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool instigate(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<InitiatorType>::value) {
			if (InitiatorType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_elusive<VictimType>::value) {
			if constexpr (VictimType::HasStaticDodge) {
				if (VictimType::dodge(random_engine)) {
					return false;
				}
			} else {
				if (victim.dodge(random_engine)) {
					return false;
				}
			}
		}

		const i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (is_berker<InitiatorType>::value) {
			initiator.enrage();

			if (!initiator.is_exhausted()) {
				initiator.recuperate();
			}
		}

		if constexpr (!is_fodder<VictimType>::value) {
			if constexpr (is_cleaver<InitiatorType>::value && is_armored<VictimType>::value) {
				if (victim.template can_survive<InitiatorType>(damage)) {
					victim.template receive_damage<InitiatorType>(damage);

					if constexpr (is_bleeder<VictimType>::value) {
						constexpr fluid_e fluid{ fluid_type<VictimType>::type };

						fluid_map<MapType>[victim.position] += fluid;

						if constexpr (is_berker<InitiatorType>::value) {
							initiator.enspatter(fluid);
						}
					}

					return false;
				}
			} else {
				if (victim.can_survive(damage)) {
					victim.receive_damage(damage);

					if constexpr (is_bleeder<VictimType>::value) {
						constexpr fluid_e fluid{ fluid_type<VictimType>::type };

						fluid_map<MapType>[victim.position] += fluid;

						if constexpr (is_berker<InitiatorType>::value) {
							initiator.enspatter(fluid);
						}
					}

					return false;
				}
			}
		}

		if constexpr (is_bleeder<VictimType>::value && is_berker<InitiatorType>::value) {
			initiator.enspatter(fluid_type<VictimType>::type);
		}

		return true;
	}

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool retaliate(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<VictimType>::value) {
			if (VictimType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_elusive<InitiatorType>::value) {
			if constexpr (InitiatorType::HasStaticDodge) {
				if (InitiatorType::dodge(random_engine)) {
					return false;
				}
			} else {
				if (initiator.dodge(random_engine)) {
					return false;
				}
			}
		}

		const i8 damage{ victim.get_damage(to_entity_enum<InitiatorType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (is_berker<VictimType>::value) {
			victim.enrage();

			if (!victim.is_exhausted()) {
				victim.recuperate();
			}
		}

		if constexpr (!is_fodder<InitiatorType>::value) {
			if constexpr (is_cleaver<VictimType>::value && is_armored<InitiatorType>::value) {
				if (initiator.template can_survive<VictimType>(damage)) {
					initiator.template receive_damage<VictimType>(damage);

					if constexpr (is_bleeder<InitiatorType>::value) {
						constexpr fluid_e fluid{ fluid_type<InitiatorType>::type };

						fluid_map<MapType>[initiator.position] += fluid;

						if constexpr (is_berker<VictimType>::value) {
							victim.enspatter(fluid);
						}
					}

					return false;
				}
			} else {
				if (initiator.can_survive(damage)) {
					initiator.receive_damage(damage);

					if constexpr (is_bleeder<InitiatorType>::value) {
						constexpr fluid_e fluid{ fluid_type<InitiatorType>::type };

						fluid_map<MapType>[initiator.position] += fluid;

						if constexpr (is_berker<VictimType>::value) {
							victim.enspatter(fluid);
						}
					}

					return false;
				}
			}
		}

		if constexpr (is_bleeder<InitiatorType>::value && is_berker<VictimType>::value) {
			victim.enspatter(fluid_type<InitiatorType>::type);
		}

		return true;
	}

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool reflect(ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		if constexpr (is_clumsy<InitiatorType>::value) {
			if (InitiatorType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_elusive<InitiatorType>::value) {
			if constexpr (InitiatorType::HasStaticDodge) {
				if (InitiatorType::dodge(random_engine)) {
					return false;
				}
			} else {
				if (initiator.dodge(random_engine)) {
					return false;
				}
			}
		}

		const i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if (damage <= 0) {
			return false;
		}

		if constexpr (is_berker<InitiatorType>::value) {
			initiator.enrage();

			if (!initiator.is_exhausted()) {
				initiator.recuperate();
			}
		}

		if constexpr (!is_fodder<InitiatorType>::value) {
			if (initiator.can_survive(damage)) {
				initiator.receive_damage(damage);

				if constexpr (is_bleeder<InitiatorType>::value) {
					constexpr fluid_e fluid{ fluid_type<InitiatorType>::type };

					fluid_map<MapType>[initiator.position] += fluid;

					if constexpr (is_berker<InitiatorType>::value) {
						initiator.enspatter(fluid);
					}
				}

				return false;
			}
		}

		return true;
	}

	template<CombatantEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, clash_t>::process() const noexcept {
		ptr<EntityType> initiator_ptr{ entity_registry<MapType>.template at<EntityType>(source_position) };

		if (initiator_ptr == nullptr) {
			return;
		}

		ref<EntityType> initiator{ *initiator_ptr };
		
		const entity_e victim_enum{ determine_target<EntityType>(entity_registry<MapType>.at(target_position)) };

		if (victim_enum == entity_e::None) {
			return;
		}

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			using victim_type = to_entity_type<cval>::type;

			if constexpr (!is_null_entity<victim_type>::value && !std::is_same<EntityType, victim_type>::value) {
				ptr<victim_type> victim_ptr{ entity_registry<MapType>.template at<victim_type>(target_position) };

				if (victim_ptr == nullptr) {
					return;
				}

				ref<victim_type> victim{ *victim_ptr };

				const bool target_killed{ instigate<MapType>(initiator, victim) };

				const bool source_killed{ [&]() -> bool {
					if constexpr (cval == entity_e::Bonespur) {
						return reflect<MapType>(initiator, victim);
					} else {
						return retaliate<MapType>(initiator, victim);
					}
				}()};

				if (target_killed) {
					victim.template die<MapType>();

					if constexpr (is_npc_entity<victim_type>::value) {
						entity_registry<MapType>.template remove<victim_type>(victim.position);
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
					initiator.template die<MapType>();

					if constexpr (is_npc_entity<EntityType>::value) {
						entity_registry<MapType>.template remove<EntityType>(initiator.position);
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
