#pragma once

#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<InitiatorType>::value && !std::is_same<InitiatorType, VictimType>::value)
	inline bool instigate(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		cauto try_bleed = [&] {
			if constexpr (is_bleeder_v<VictimType>) {
				if constexpr (is_bleeder<VictimType>::conditional) {
					if (!victim.can_bleed()) {
						return;
					}
				}

				fluid_e fluid{ is_bleeder<VictimType>::type };

				if constexpr (is_thirsty_for<InitiatorType, VictimType>) {
					if (initiator.can_imbibe()) {
						initiator.imbibe();
						
						fluid -= shares(is_thirsty<InitiatorType>::type, is_bleeder<VictimType>::type);
					}
				}

				if (fluid != fluid_e::None) {
					spill_fluid<MapType>(target_position, fluid);

					if constexpr (is_spatterable<InitiatorType>::value) {
						initiator.enspatter(fluid);
					}				
				}
			}
		};

		if constexpr (is_docile<InitiatorType>::value) {
			return false;
		}

		if constexpr (is_clumsy<InitiatorType>::value) {
			if (InitiatorType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_good<InitiatorType>::value) {
			if (entity_goal_map<MapType, furtive_horror_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= furtive_horror_t::EffectRadius) {
				if (furtive_horror_t::fumble(random_engine)) {
					return false;
				}
			}
		}

		if constexpr (is_elusive<VictimType>::value && !is_inevadable<InitiatorType>::value) {
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

		i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if constexpr (is_encouragable<InitiatorType>::value) {
			if (entity_goal_map<MapType, banner_bearer_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= banner_bearer_t::EffectRadius) {
				damage = max<i8>(damage * banner_bearer_t::EffectDamageMultiplier, 1);
			}
		}

		if (damage <= 0) {
			return false;
		}

		if constexpr (is_holy<InitiatorType>::value && is_unholy<VictimType>::value) {
			victim.sear();
		}

		if constexpr (is_berker<InitiatorType>::value) {
			initiator.enrage();

			if (!initiator.is_exhausted()) {
				initiator.recuperate();
			}
		}

		if constexpr (!is_fodder<VictimType>::value) {
			if constexpr (is_cleaver<InitiatorType>::value && is_armored<VictimType>::value) {
				if constexpr (is_volumetric<VictimType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (victim.dependent can_survive<InitiatorType>(fluid_damage)) {
						if (victim.dependent receive_damage<InitiatorType>(fluid_damage)) {	
							try_bleed();
						}
	
						return false;
					}
				} else {
					if (victim.dependent can_survive<InitiatorType>(damage)) {
						if (victim.dependent receive_damage<InitiatorType>(damage)) {	
							try_bleed();
						}
	
						return false;
					}
				}
			} else {
				if constexpr (is_volumetric<VictimType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (victim.can_survive(fluid_damage)) {
						if (victim.receive_damage(fluid_damage)) {	
							try_bleed();
						}
	
						return false;
					}
				} else {
					if (victim.can_survive(damage)) {
						if (victim.receive_damage(damage)) {	
							try_bleed();
						}
	
						return false;
					}
				}
			}
		}

		try_bleed();

		return true;
	}

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<VictimType>::value && !std::is_same<InitiatorType, VictimType>::value)
	inline bool retaliate(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		cauto try_bleed = [&] {
			if constexpr (is_bleeder_v<InitiatorType>) {
				if constexpr (is_bleeder<InitiatorType>::conditional) {
					if (!initiator.can_bleed()) {
						return;
					}
				}

				fluid_e fluid{ is_bleeder<InitiatorType>::type };

				if constexpr (is_thirsty_for<VictimType, InitiatorType>) {
					if (victim.can_imbibe()) {
						victim.imbibe();
						
						fluid -= shares(is_thirsty<VictimType>::type, is_bleeder<InitiatorType>::type);
					}
				}

				if (fluid != fluid_e::None) {
					spill_fluid<MapType>(source_position, fluid);

					if constexpr (is_spatterable<VictimType>::value) {
						victim.enspatter(fluid);
					}
				}
			}
		};

		if constexpr (is_docile<VictimType>::value) {
			return false;
		}

		if constexpr (is_clumsy<VictimType>::value) {
			if (VictimType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_good<VictimType>::value) {
			if (entity_goal_map<MapType, furtive_horror_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= furtive_horror_t::EffectRadius) {
				if (furtive_horror_t::fumble(random_engine)) {
					return false;
				}
			}
		}

		if constexpr (is_elusive<InitiatorType>::value && !is_inevadable<VictimType>::value) {
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

		i8 damage{ victim.get_damage(to_entity_enum<InitiatorType>::value) };

		if constexpr (is_encouragable<VictimType>::value) {
			if (entity_goal_map<MapType, banner_bearer_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= banner_bearer_t::EffectRadius) {
				damage = max<i8>(damage * banner_bearer_t::EffectDamageMultiplier, 1);
			}
		}

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
				if constexpr (is_volumetric<InitiatorType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (initiator.dependent can_survive<VictimType>(fluid_damage)) {
						initiator.dependent receive_damage<VictimType>(fluid_damage);

						try_bleed();

						return false;
					}
				} else {
					if (initiator.dependent can_survive<VictimType>(damage)) {
						initiator.dependent receive_damage<VictimType>(damage);

						try_bleed();

						return false;
					}
				}
			} else {
				if constexpr (is_volumetric<InitiatorType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (initiator.can_survive(fluid_damage)) {
						initiator.receive_damage(fluid_damage);

						try_bleed();

						return false;
					}
				} else {
					if (initiator.can_survive(damage)) {
						initiator.receive_damage(damage);

						try_bleed();

						return false;
					}
				}
			}
		}

		try_bleed();

		return true;
	}

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<InitiatorType>::value && !std::is_same<InitiatorType, VictimType>::value)
	inline bool reflect(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept {
		cauto try_bleed = [&] {
			if constexpr (is_bleeder<InitiatorType>::value) {
				if constexpr (is_bleeder<InitiatorType>::conditional) {
					if (!initiator.can_bleed()) {
						return;
					}
				}

				constexpr fluid_e fluid{ is_bleeder<InitiatorType>::type };

				spill_fluid<MapType>(source_position, fluid);

				if constexpr (is_spatterable<InitiatorType>::value) {
					initiator.enspatter(fluid);
				}

				if constexpr (is_spatterable<VictimType>::value) {
					victim.enspatter(fluid);
				}
			}
		};

		if constexpr (is_clumsy<InitiatorType>::value) {
			if (InitiatorType::fumble(random_engine)) {
				return false;
			}
		}

		if constexpr (is_good<InitiatorType>::value) {
			if (entity_goal_map<MapType, furtive_horror_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= furtive_horror_t::EffectRadius) {
				if (furtive_horror_t::fumble(random_engine)) {
					return false;
				}
			}
		}

		if constexpr (is_elusive<InitiatorType>::value && !is_inevadable<VictimType>::value) {
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

		i8 damage{ initiator.get_damage(to_entity_enum<VictimType>::value) };

		if constexpr (is_encouragable<InitiatorType>::value) {
			if (entity_goal_map<MapType, banner_bearer_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= banner_bearer_t::EffectRadius) {
				damage = max<i8>(damage * banner_bearer_t::EffectDamageMultiplier, 1);
			}
		}

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
			if constexpr (is_cleaver<VictimType>::value && is_armored<InitiatorType>::value) {
				if constexpr (is_volumetric<InitiatorType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (initiator.dependent can_survive<VictimType>(fluid_damage)) {
						initiator.dependent receive_damage<VictimType>(fluid_damage);

						try_bleed();

						return false;
					}
				} else {
					if (initiator.dependent can_survive<VictimType>(damage)) {
						initiator.dependent receive_damage<VictimType>(damage);

						try_bleed();

						return false;
					}
				}
			} else {
				if constexpr (is_volumetric<InitiatorType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (initiator.can_survive(fluid_damage)) {
						initiator.receive_damage(fluid_damage);

						try_bleed();

						return false;
					}
				} else {
					if (initiator.can_survive(damage)) {
						initiator.receive_damage(damage);

						try_bleed();

						return false;
					}
				}
			}
		}

		try_bleed();

		return true;
	}

	template<CombatantEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, clash_t>::process() const noexcept {
		if constexpr (!is_docile<EntityType>::value) {
			ptr<EntityType> initiator_ptr{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

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
					ptr<victim_type> victim_ptr{ entity_registry<MapType>.dependent at<victim_type>(target_position) };

					if (victim_ptr == nullptr) {
						return;
					}

					ref<victim_type> victim{ *victim_ptr };

					const bool target_killed{ instigate<MapType>(source_position, target_position, initiator, victim) };

					const bool source_killed{
						[&]() -> bool {
							if constexpr (!is_docile<victim_type>::value) {
								if constexpr (cval == entity_e::Bonespur) {
									return reflect<MapType>(source_position, target_position, initiator, victim);
								} else {
									return retaliate<MapType>(source_position, target_position, initiator, victim);
								}
							}

							return false;
						}()
					};

					if (target_killed) {
						if constexpr (is_player<victim_type>::value) {
							victim.dependent die<MapType, death_e::Killed>();
						} else {
							victim.dependent die<MapType, death_e::Killed>(target_position);
						}

						if constexpr (is_npc_entity<victim_type>::value) {
							entity_registry<MapType>.dependent remove<victim_type>(target_position);
						}

						if constexpr (is_player<EntityType>::value) {
							++game_stats.player_kills;
							++steam_stats::stats<steam_stat_e::PlayerKills>;

							warped_from = std::nullopt;
						} else if constexpr (is_evil<EntityType>::value) {
							++game_stats.minion_kills;
							++steam_stats::stats<steam_stat_e::MinionKills>;
						}
					}

					if (source_killed) {
						if constexpr (is_player<EntityType>::value) {
							initiator.dependent die<MapType, death_e::Killed>();
						} else {
							initiator.dependent die<MapType, death_e::Killed>(source_position);
						}

						if constexpr (is_npc_entity<EntityType>::value) {
							entity_registry<MapType>.dependent remove<EntityType>(source_position);
						}

						if constexpr (is_player<victim_type>::value) {
							++game_stats.player_kills;
							++steam_stats::stats<steam_stat_e::PlayerKills>;

							warped_from = std::nullopt;
						} else if constexpr (is_evil<victim_type>::value) {
							++game_stats.minion_kills;
							++steam_stats::stats<steam_stat_e::MinionKills>;
						}
					}
				}			
			}, victim_enum);
		}
	}
} // namespace necrowarp
