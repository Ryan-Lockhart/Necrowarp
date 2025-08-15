#pragma once

#include <necrowarp/commands/unary/repulse.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>
#include <necrowarp/literature.hpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, repulse_t>::process() const noexcept {
		if (!player.can_perform(grimoire_e::Repulse)) {
			player_turn_invalidated = true;

			return;
		}

		const i8 base_damage{ static_cast<i8>(player_t::MaximumDamage + player.get_armor()) };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			cauto current_position{ source_position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || game_map<MapType>[current_position].solid || entity_registry<MapType>.empty(current_position)) {
				continue;
			}

			cauto target_position{ current_position + offset };

			const bool concussive{ !game_map<MapType>.dependent within<region_e::Interior>(target_position) || game_map<MapType>[target_position].solid || !entity_registry<MapType>.empty(target_position) };

			const i8 damage{ static_cast<i8>(base_damage * (concussive ? 1.50f : 0.75f)) };

			const std::optional<entity_e> maybe_target{ entity_registry<MapType>.at(target_position) };

			if (maybe_target.has_value() && is_interactable<player_t>(maybe_target.value())) {
				magic_enum::enum_switch([&](auto val) {
					constexpr entity_e cval{ val };

					using entity_type = typename to_entity_type<cval>::type;

					if constexpr (is_good<entity_type>::value) {
						ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(current_position) };

						if (maybe_target == nullptr) {
							return;
						}

						ref<entity_type> target{ *maybe_target };

						cauto try_displace = [&] -> bool {
							if (!concussive && entity_registry<MapType>.dependent update<entity_type>(current_position, target_position)) {
								concussed.add(target_position);

								return true;
							}

							return false;
						};

						cauto try_bleed = [&] {
							if constexpr (is_bleeder_v<entity_type>) {
								if constexpr (is_bleeder<entity_type>::conditional) {
									if (!target.can_bleed()) {
										return;
									}
								}

								spill_fluid<MapType>(current_position, is_bleeder<entity_type>::type);
							}
						};

						if constexpr (is_berker<entity_type>::value) {
							target.enrage();

							if (!target.is_exhausted()) {
								target.recuperate();
							}
						}

						if (damage <= 0) {
							try_displace();

							return;
						}

						if constexpr (!is_fodder<entity_type>::value) {
							if constexpr (is_cleaver<player_t>::value && is_armored<entity_type>::value) {
								if constexpr (is_volumetric<entity_type>::value) {
									const f16 fluid_damage{ fluid_pool_volume(damage) };

									if (target.dependent can_survive<entity_type>(fluid_damage)) {
										if (target.dependent receive_damage<entity_type>(fluid_damage)) {	
											try_bleed();
										}

										if (try_displace()) {
											return;
										}
									}
								} else {
									if (target.dependent can_survive<entity_type>(damage)) {
										if (target.dependent receive_damage<entity_type>(damage)) {	
											try_bleed();
										}

										if (try_displace()) {
											return;
										}
									}
								}
							} else {
								if constexpr (is_volumetric<entity_type>::value) {
									const f16 fluid_damage{ fluid_pool_volume(damage) };

									if (target.can_survive(fluid_damage)) {
										if (target.receive_damage(fluid_damage)) {	
											try_bleed();
										}

										if (try_displace()) {
											return;
										}
									}
								} else {
									if (target.can_survive(damage)) {
										if (target.receive_damage(damage)) {	
											try_bleed();
										}

										if (try_displace()) {
											return;
										}
									}
								}
							}
						}

						try_bleed();

						if (concussive) {
							const death_info_t<death_e::Crushed> info{ target.dependent die<MapType, death_e::Crushed>(current_position) };

							if (info.perished) {
								entity_registry<MapType>.dependent remove<entity_type>(current_position);

								++game_stats.player_kills;
								++steam_stats::stats<stat_e::PlayerKills>;
							}
						} else {
							const death_info_t<death_e::Killed> info{ target.dependent die<MapType, death_e::Killed>(current_position) };

							if (info.perished) {
								entity_registry<MapType>.dependent remove<entity_type>(current_position);

								++game_stats.player_kills;
								++steam_stats::stats<stat_e::PlayerKills>;
							}
						}
					}
				}, maybe_target.value());
			}
		}

		++steam_stats::stats<stat_e::Repulsions>;

		player.pay_cost(grimoire_e::Repulse);

		literature::use(grimoire_e::Repulse);
	}
} // namespace necrowarp
