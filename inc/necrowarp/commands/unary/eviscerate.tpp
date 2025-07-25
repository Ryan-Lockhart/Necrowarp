#pragma once

#include <necrowarp/commands/unary/eviscerate.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	inline bool brutalize(offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim, ref<i8> damage) noexcept {
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
			if (!coinflip(random_engine)) {
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
				if constexpr (is_volumetric<VictimType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (victim.dependent can_survive<InitiatorType>(fluid_damage)) {
						victim.dependent receive_damage<InitiatorType>(fluid_damage);
	
						try_bleed();

						if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
							--damage;
						} else {
							damage = 0;
						}
	
						return false;
					}
				} else {
					if (victim.dependent can_survive<InitiatorType>(damage)) {
						victim.dependent receive_damage<InitiatorType>(damage);
	
						try_bleed();

						if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
							--damage;
						} else {
							damage = 0;
						}
	
						return false;
					}
				}
			} else {
				if constexpr (is_volumetric<VictimType>::value) {
					const f16 fluid_damage{ fluid_pool_volume(damage) };

					if (victim.can_survive(fluid_damage)) {
						victim.receive_damage(fluid_damage);
	
						try_bleed();

						if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
							--damage;
						} else {
							damage = 0;
						}
	
						return false;
					}
				} else {
					if (victim.can_survive(damage)) {
						victim.receive_damage(damage);
	
						try_bleed();

						if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
							--damage;
						} else {
							damage = 0;
						}
	
						return false;
					}
				}				
			}
		}

		try_bleed();

		--damage;

		return true;
	}

	template<CombatantEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, eviscerate_t>::process() const noexcept {
		ptr<EntityType> initiator_ptr{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

		if (initiator_ptr == nullptr) {
			return;
		}

		ref<EntityType> initiator{ *initiator_ptr };

		i8 remaining_damage = initiator.get_damage();

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			if (remaining_damage <= 0) {
				break;
			}

			cauto position{ source_position + offset };

			const entity_e victim_enum{ determine_target<EntityType>(entity_registry<MapType>.at(position)) };

			if (victim_enum == entity_e::None) {
				continue;
			}

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr entity_e cval{ val };

				using victim_type = to_entity_type<cval>::type;

				if constexpr (!is_evil<victim_type>::value) {
					if constexpr (!is_null_entity<victim_type>::value && !std::is_same<EntityType, victim_type>::value) {
						ptr<victim_type> victim_ptr{ entity_registry<MapType>.dependent at<victim_type>(position) };

						if (victim_ptr == nullptr) {
							return;
						}

						ref<victim_type> victim{ *victim_ptr };

						const bool target_killed{ brutalize<MapType>(position, initiator, victim, remaining_damage) };

						if (target_killed) {
							if constexpr (is_player<victim_type>::value) {
								victim.dependent killed<MapType>();
							} else {
								victim.dependent killed<MapType>(position);
							}

							if constexpr (is_npc_entity<victim_type>::value) {
								entity_registry<MapType>.dependent remove<victim_type>(position);
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
					}
				}
			}, victim_enum);
		}
	}
} // namespace necrowarp
