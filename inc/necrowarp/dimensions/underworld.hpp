#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

#include <necrowarp/literature.tpp>

namespace necrowarp {
	template<> inline void game_s::load<dimension_e::Underworld>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Underworld>();

		game_map<MapType>.dependent reset<region_e::All>();
		fluid_map<MapType>.dependent reset<region_e::All>();

		entity_registry<MapType>.clear();
		object_registry<MapType>.clear();

		literature::reset();

		reset_patrons();

		player.patron = desired_patron;

		literature::receive_dowry(player.patron);

		game_stats.reset();

		game_stats.cheats.enable_all();
		game_stats.cheats.endow_knowledge = false;

		player.refresh();

		scorekeeper.reset();

		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		globals::randomize_map_config(random_engine);

		game_map<MapType>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::map_config.fill_percent,
				globals::map_config.automata_iterations,
				globals::map_config.automata_threshold,
				cell_applicator
			)
			.dependent collapse<region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

		std::vector<area_t> areas{ area_t::partition(game_map<MapType>, cell_e::Open) };

		if (areas.size() > 1) {
			cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

			for (crauto area : areas) {
				if (area != largest_area) {
					area.apply(game_map<MapType>, cell_e::Solid);
				}
			}
		}

		recalculate_maps<MapType>();

		cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

		if (!player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
			error_log.add("[ERROR]: could not find open position for player!");

			terminate_prematurely();
		}

		if constexpr (globals::SpawnTutorialPortal) {
			if (!steam_stats::is_unlocked(achievement_e::CompleteBasicTutorial)) {
				if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !portal_pos.has_value() || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Insightful })) {
					error_log.add("[ERROR]: could not find open position for tutorial portal!");
					terminate_prematurely();
				}
			}
		}

		object_registry<MapType>.dependent spawn<crevice_t>(
			static_cast<usize>(globals::StartingCrevices),
			static_cast<u32>(globals::MinimumCreviceDistance)
		);

		object_registry<MapType>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingUpLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Up
		);

		object_registry<MapType>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingDownLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Down, random_engine
		);

		if constexpr (globals::EnableTribulationPortal) {
			const bool is_softlocked{ literature::is_softlocked<MapType>() };

			if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !portal_pos.has_value() || (!is_softlocked && !globals::tribulation_portal_chance(random_engine)) || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Turbulent })) {
				if (is_softlocked) {
					error_log.add("[ERROR]: could not find open position for tribulation portal!");
					terminate_prematurely();
				}
			}
		}

		object_registry<MapType>.dependent spawn<bones_t>(
			static_cast<usize>(globals::StartingBones),
			static_cast<u32>(globals::MinimumBoneDistance),

			decay_e::Animate
		);
	}

	template<> inline void game_s::descend<dimension_e::Underworld>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Underworld>();

		terminate_process_turn();

		randomize_patrons();
#if !defined(STEAMLESS)
		steam_stats::store();

		stat_store_timer.record();
#endif
		descent_flag = false;

		++game_stats.game_depth;
#if !defined(STEAMLESS)
		if (steam_stats_s::stats<stat_e::LowestDepth>.get_value() > -static_cast<i32>(game_stats.game_depth)) {
			steam_stats_s::stats<stat_e::LowestDepth> = -static_cast<i32>(game_stats.game_depth);
		}
#endif
		fluid_positions.clear();
		ladder_positions.clear();

		for (crauto [position, ladder] : object_registry_storage<ladder_t>) {
			if (!game_map<MapType>.dependent within<region_e::Interior>(position) || ladder.is_up_ladder()) {
				continue;
			}

			ladder_positions.add(position);

			const fluid_e fluid{ fluid_map<MapType>[position] };

			if (fluid == fluid_e::None) {
				continue;
			}

			fluid_positions.add(position, fluid);
		}

		game_map<MapType>.dependent reset<region_e::All>();
		fluid_map<MapType>.dependent reset<region_e::All>();

		entity_registry<MapType>.dependent clear<ALL_NON_PLAYER>();
		entity_registry<MapType>.dependent reset_goal_map<player_t>();

		entity_registry<MapType>.reset_unique_goal_maps();

		object_registry<MapType>.clear();

		deceased.clear();
		concussed.clear();
		afflicted.clear();

		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		globals::randomize_map_config(random_engine);
		
		game_map<MapType>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::map_config.fill_percent,
				globals::map_config.automata_iterations,
				globals::map_config.automata_threshold,
				cell_applicator,
				ladder_positions
			)
			.dependent collapse<region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

		std::vector<area_t> areas{ area_t::partition(game_map<MapType>, cell_e::Open) };

		if (areas.size() > 1) {
			cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

			for (crauto area : areas) {
				if (area != largest_area) {
					area.apply(game_map<MapType>, cell_e::Solid);
				}
			}
		}

		recalculate_cell_map<MapType>();

		for (cauto [position, fluid] : fluid_positions) {
			if (game_map<MapType>[position] != cell_e::Open || fluid == fluid_e::None) {
				continue;
			}

			fluid_map<MapType>[position] = fluid;
		}

		fluid_positions.clear();

		recalculate_fluid_map<MapType, true>();
#if !defined(STEAMLESS)
		cauto previous_position{ player.position };
#endif
		if (game_map<MapType>[player.position].solid) {
			cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
				error_log.add("[ERROR]: could not find open position for player!");
				terminate_prematurely();
			}
		} else if (!entity_registry<MapType>.dependent add<player_t>(player.position)) {
			error_log.add("[ERROR]: failed to add player to map!");

			terminate_prematurely();
		}
#if !defined(STEAMLESS)
		steam_stats_s::stats<stat_e::MetersMoved> += offset_t::distance<f32>(previous_position, player.position);
#endif
		if constexpr (globals::SpawnTutorialPortal) {
			if (!steam_stats::is_unlocked(achievement_e::CompleteBasicTutorial)) {
				if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !portal_pos.has_value() || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Insightful })) {
					error_log.add("[ERROR]: could not find open position for tutorial portal!");
					terminate_prematurely();
				}
			}
		}

		if constexpr (globals::EnableBoonPortal) {
			if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value() && globals::boon_portal_chance(random_engine)) {
				object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Calm });
			}
		}

		if constexpr (globals::EnableTribulationPortal) {
			if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value() && globals::tribulation_portal_chance(random_engine)) {
				object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Turbulent });
			}
		}

		if constexpr (globals::EnableAudiencePortal) {
			if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value() && globals::audience_portal_chance(random_engine)) {
				object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Vocal });
			}
		}

		object_registry<MapType>.dependent spawn<crevice_t>(
			static_cast<usize>(globals::StartingCrevices),
			static_cast<u32>(globals::MinimumCreviceDistance)
		);

		i16 num_up_ladders_needed{ globals::StartingUpLadders };

		for (cauto [position, _] : ladder_positions) {
			if (game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position) || !object_registry<MapType>.empty(position) || !object_registry<MapType>.add(position, ladder_t{ verticality_e::Up })) {
				continue;
			}

			--num_up_ladders_needed;
		}

		ladder_positions.clear();

		if (num_up_ladders_needed > 0) {
			object_registry<MapType>.dependent spawn<ladder_t>(
				static_cast<usize>(num_up_ladders_needed),
				static_cast<u32>(globals::MinimumLadderDistance),

				verticality_e::Up
			);
		}

		object_registry<MapType>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingDownLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Down, random_engine
		);

		object_registry<MapType>.dependent spawn<bones_t>(
			static_cast<usize>(globals::StartingBones),
			static_cast<u32>(globals::MinimumBoneDistance),

			decay_e::Animate
		);
	}

	template<> inline void game_s::plunge<dimension_e::Underworld>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Underworld>();
#if !defined(STEAMLESS)
		steam_stats::store();

		stat_store_timer.record();
#endif
		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		globals::randomize_map_config(random_engine);

		game_map<MapType>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::map_config.fill_percent,
				globals::map_config.automata_iterations,
				globals::map_config.automata_threshold,
				cell_applicator
			)
			.dependent collapse<region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

		std::vector<area_t> areas{ area_t::partition(game_map<MapType>, cell_e::Open) };

		if (areas.size() > 1) {
			cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

			for (crauto area : areas) {
				if (area != largest_area) {
					area.apply(game_map<MapType>, cell_e::Solid);
				}
			}
		}

		recalculate_cell_map<MapType>();

		if (cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
			error_log.add("[ERROR]: could not find open position for player!");

			terminate_prematurely();
		}

		if constexpr (globals::SpawnTutorialPortal) {
			if (!steam_stats::is_unlocked(achievement_e::CompleteBasicTutorial)) {
				if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !portal_pos.has_value() || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Insightful })) {
					error_log.add("[ERROR]: could not find open position for tutorial portal!");
					terminate_prematurely();
				}
			}
		}

		object_registry<MapType>.dependent spawn<crevice_t>(
			static_cast<usize>(globals::StartingCrevices),
			static_cast<u32>(globals::MinimumCreviceDistance)
		);

		object_registry<MapType>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingUpLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Up
		);

		object_registry<MapType>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingDownLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Down, random_engine
		);

		if constexpr (globals::EnableTribulationPortal) {
			const bool softlock_detected{
				[&]() -> bool {
					for (crauto [position, ladder] : object_registry_storage<ladder_t>) {
						if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || ladder.is_up_ladder() || !ladder.has_shackle()) {
							continue;
						}

						switch (ladder.shackle) {
							case shackle_e::Calcitic: {
								if (!literature::is_acquired(grimoire_e::CalciticInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Spectral: {
								if (!literature::is_acquired(grimoire_e::SpectralInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Sanguine: {
								if (!literature::is_acquired(grimoire_e::SanguineInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Galvanic: {
								if (!literature::is_acquired(grimoire_e::GalvanicInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Ravenous: {
								if (!literature::is_acquired(grimoire_e::RavenousInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Wretched: {
								if (!literature::is_acquired(grimoire_e::WretchedInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Cerebral: {
								if (!literature::is_acquired(grimoire_e::CerebralInvocation)) {
									continue;
								}

								return false;
							} case shackle_e::Infernal: {
								if (!literature::is_acquired(grimoire_e::InfernalInvocation)) {
									continue;
								}

								return false;
							} default: {
								continue;
							}
						}
					}

					return true;
				}()
			};

			if (cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value() && (softlock_detected || globals::tribulation_portal_chance(random_engine))) {
				object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Turbulent });
			}
		}

		object_registry<MapType>.dependent spawn<bones_t>(
			static_cast<usize>(globals::StartingBones),
			static_cast<u32>(globals::MinimumBoneDistance),

			decay_e::Animate
		);
	}

	template<> inline void game_s::process_turn<dimension_e::Underworld>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Underworld>();

		game_stats.update_wave_size();

		if (entity_registry<MapType>.dependent empty<ALL_GOOD_NPCS>() && !game_stats.has_spawns() && !entity_registry<MapType>.any_offmap()) {
			game_stats.spawns_remaining = game_stats.wave_size;
		}

		while (entity_registry<MapType>.any_offmap()) {
			if (!spawn_offmap<MapType, ALL_GOOD>()) {
				break;
			}
		}

		while (game_stats.has_spawns()) {
			if (!spawn_good<MapType>()) {
				break;
			}

			--game_stats.spawns_remaining;
		}

		if (game_stats.has_reinforcements() && !game_stats.has_spawns()) {
			for (i16 i{ 0 }; i < game_stats.current_reinforcements(); ++i) {
				if (!spawn_good<MapType>()) {
					break;
				}
			}
		}

		const usize thetwo_target{ object_registry<MapType>.dependent count<flesh_t>() / globals::FleshPerThetwoPopulation };

		while (entity_registry<MapType>.dependent count<thetwo_t>() < thetwo_target) {
			if (!spawn_neutral<MapType>()) {
				break;
			}
		}
	}

	template<> inline void game_s::unload<dimension_e::Underworld>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Underworld>();

		terminate_process_turn();

		game_map<MapType>.dependent reset<region_e::All>();
		fluid_map<MapType>.dependent reset<region_e::All>();
		
		entity_registry<MapType>.clear();
		object_registry<MapType>.clear();

#if !defined(STEAMLESS)
		steam_stats::store();

		stat_store_timer.record();
#endif
	}
} // namespace necrowarp
