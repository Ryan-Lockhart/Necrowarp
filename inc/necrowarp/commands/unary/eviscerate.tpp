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
	inline bool brutalize(ref<InitiatorType> initiator, ref<VictimType> victim, ref<i8> damage) noexcept {
		if constexpr (is_clumsy<InitiatorType>::value) {
			if (!coinflip(random_engine)) {
				return false;
			}
		}

		if (damage <= 0) {
			return false;
		}

		if constexpr (!is_fodder<VictimType>::value) {
			if (victim.can_survive(damage)) {
				victim.receive_damage(damage);

				if constexpr (is_bleeder<VictimType>::value) {
					const fluid_e fluid{ fluid_type<VictimType>::type };

					spill_fluid<MapType>(victim.position, fluid);

					if constexpr (is_berker<VictimType>::value) {
						victim.enspatter(fluid);
					}
				}

				if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
					--damage;
				} else {
					damage = 0;
				}

				return false;
			}

			if constexpr (is_berker<VictimType>::value || is_serene<VictimType>::value) {
				--damage;
			} else {
				damage -= victim.get_health();
			}

			return true;
		}

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

			cauto position{ initiator.position + offset };

			const entity_e victim_enum{ determine_target<EntityType>(entity_registry<MapType>.at(position)) };

			if (victim_enum == entity_e::None) {
				return;
			}

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr entity_e cval{ val };

				using victim_type = to_entity_type<cval>::type;

				if constexpr (!is_evil_entity<victim_type>::value) {
					if constexpr (!is_null_entity<victim_type>::value && !std::is_same<EntityType, victim_type>::value) {
						ptr<victim_type> victim_ptr{ entity_registry<MapType>.dependent at<victim_type>(position) };

						if (victim_ptr == nullptr) {
							return;
						}

						ref<victim_type> victim{ *victim_ptr };

						const bool target_killed{ brutalize<MapType>(initiator, victim, remaining_damage) };

						if (target_killed) {
							victim.dependent die<MapType>();

							if constexpr (is_npc_entity<victim_type>::value) {
								entity_registry<MapType>.dependent remove<victim_type>(victim.position);
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
					}
				}
			}, victim_enum);
		}
	}
} // namespace necrowarp
