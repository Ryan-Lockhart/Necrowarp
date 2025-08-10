#pragma once

#include <necrowarp/commands/binary/annihilate.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, annihilate_t>::process() const noexcept {
		if (!player.can_perform(grimoire_e::Annihilate)) {
			player_turn_invalidated = true;

			return;
		}

		const i8 damage{ static_cast<i8>((player_t::MaximumDamage + player.get_armor()) * (player.has_ascended() ? 2 : 1)) };

		if (damage <= 0) {
			player_turn_invalidated = true;

			return;
		}

		offset_t pos{ source_position };

		offset_t delta{ std::abs(target_position.x - source_position.x), std::abs(target_position.y - source_position.y) };

		offset_t step{ source_position.x < target_position.x ? 1 : -1, source_position.y < target_position.y ? 1 : -1 };

		i32 err = delta.x - delta.y;

		fluid_e carried_fluid{ fluid_e::None };

		for (;;) {
			if (!game_map<MapType>.dependent within<region_e::Interior>(pos) || (!player.is_incorporeal() && game_map<MapType>[pos].solid)) {
				player_turn_invalidated = true;

				return;
			}

			if (pos == target_position) {
				if (carried_fluid != fluid_e::None && !game_map<MapType>[pos].solid) {
					spill_fluid<MapType>(pos, carried_fluid);
				}

				break;
			}

			if (pos != source_position) {
				const std::optional<entity_e> maybe_target{ entity_registry<MapType>.at(pos) };

				if (maybe_target.has_value()) {
					magic_enum::enum_switch([&](auto val) {
						constexpr entity_e cval{ val };

						using entity_type = typename to_entity_type<cval>::type;

						ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(pos) };

						if (maybe_target == nullptr) {
							return;
						}

						ref<entity_type> target{ *maybe_target };

						cauto try_bleed = [&] {
							if constexpr (is_bleeder_v<entity_type>) {
								if constexpr (is_bleeder<entity_type>::conditional) {
									if (!target.can_bleed()) {
										return;
									}
								}

								carried_fluid += is_bleeder<entity_type>::type;

								if (carried_fluid != fluid_e::None) {
									spill_fluid<MapType>(pos, carried_fluid);
								}
							}
						};

						if constexpr (is_berker<entity_type>::value) {
							target.enrage();

							if (!target.is_exhausted()) {
								target.recuperate();
							}
						}

						if constexpr (!is_fodder<entity_type>::value) {
							if constexpr (is_cleaver<player_t>::value && is_armored<entity_type>::value) {
								if constexpr (is_volumetric<entity_type>::value) {
									const f16 fluid_damage{ fluid_pool_volume(damage) };

									if (target.dependent can_survive<entity_type>(fluid_damage)) {
										if (target.dependent receive_damage<entity_type>(fluid_damage)) {	
											try_bleed();
										}
					
										return;
									}
								} else {
									if (target.dependent can_survive<entity_type>(damage)) {
										if (target.dependent receive_damage<entity_type>(damage)) {	
											try_bleed();
										}
					
										return;
									}
								}
							} else {
								if constexpr (is_volumetric<entity_type>::value) {
									const f16 fluid_damage{ fluid_pool_volume(damage) };

									if (target.can_survive(fluid_damage)) {
										if (target.receive_damage(fluid_damage)) {	
											try_bleed();
										}
					
										return;
									}
								} else {
									if (target.can_survive(damage)) {
										if (target.receive_damage(damage)) {	
											try_bleed();
										}
					
										return;
									}
								}
							}
						}

						try_bleed();

						if constexpr (!is_player<entity_type>::value) {
							const death_info_t<death_e::Killed> info { target.dependent die<MapType, death_e::Killed>(pos) };

							if (info.perished) {
								entity_registry<MapType>.dependent remove<entity_type>(pos);

								++game_stats.player_kills;
								++steam_stats::stats<steam_stat_e::PlayerKills>;
							}
						}
					}, maybe_target.value());
				} else {
					if (carried_fluid != fluid_e::None && !game_map<MapType>[pos].solid) {
						spill_fluid<MapType>(pos, carried_fluid);
					}
				}
			}

			i32 e2 = 2 * err;

			if (e2 > -delta.y) {
				err -= delta.y;
				pos.x += step.x;
			}

			if (e2 < delta.x) {
				err += delta.x;
				pos.y += step.y;
			}
		}

		warped_from = std::nullopt;

		entity_command_t<EntityType, move_t>{ source_position, target_position }.dependent process<MapType>();

		++steam_stats::stats<steam_stat_e::Annihilations>;

		player.pay_cost(grimoire_e::Annihilate);

		literature::use(grimoire_e::Annihilate);
	}
} // namespace necrowarp
