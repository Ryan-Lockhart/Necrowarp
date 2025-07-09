#pragma once

#include <bleak.hpp>

#include <cstdlib>
#include <iostream>
#include <thread>

#include <necrowarp/entities.hpp>
#include <necrowarp/commands.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities.tpp>
#include <necrowarp/commands.tpp>

#include <necrowarp/game_state.hpp>
#include <necrowarp/ui_state.hpp>
#include <necrowarp/globals.hpp>
#include <necrowarp/scorekeeper.hpp>
#include <necrowarp/animation.hpp>
#include <necrowarp/patronage.tpp>

#include <magic_enum/magic_enum_all.hpp>

namespace necrowarp {
	using namespace bleak;

	struct game_s {
	  public:
		static inline int run() noexcept;

	  private:
		template<map_type_e MapType> static inline bool camera_input() noexcept {
			if (update_camera<MapType>()) {
				return true;
			}

			if (keyboard_s::is_key<input_e::Down>(bindings::CameraLock)) {
				camera_locked = !camera_locked;
			}

			if (camera_locked) {
				return false;
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (keyboard_s::is_key<input_e::Pressed>(bindings::CameraMovement[cardinal_e::North])) {
					--y;
				} if (keyboard_s::is_key<input_e::Pressed>(bindings::CameraMovement[cardinal_e::South])) {
					++y;
				} if (keyboard_s::is_key<input_e::Pressed>(bindings::CameraMovement[cardinal_e::West])) {
					--x;
				} if (keyboard_s::is_key<input_e::Pressed>(bindings::CameraMovement[cardinal_e::East])) {
					++x;
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				return camera<MapType>.move(direction * globals::camera_speed<MapType>);
			}

			return false;
		}

		template<map_type_e MapType> static inline bool character_input() noexcept {
			player.command = command_pack_t{};

			const bool ignore_objects{ keyboard_s::is_key<input_e::Pressed>(bindings::IgnoreObjects) };

			if (keyboard_s::any_keys<input_e::Pressed>(bindings::Wait)) {
				player.command = command_pack_t{ command_e::None };

				return true;
			} else if (keyboard_s::is_key<input_e::Down>(bindings::RandomWarp)) {
				player.command = command_pack_t{ command_e::RandomWarp, player.position };
			} else if (keyboard_s::is_key<input_e::Down>(bindings::TargetWarp)) {
				const offset_t target_position{ grid_cursor<MapType>.get_position() };

				const entity_group_e entities{ entity_registry<MapType>.at(target_position) };
				const object_group_e objects{ object_registry<MapType>.at(target_position) };

				const entity_e target_entity{ determine_target<player_t>(entities) };
				const object_e target_object{ determine_target<player_t>(objects) };

				player.command = command_pack_t{
					player.can_consume(target_entity) || player.can_consume(target_object) ?
						command_e::ConsumeWarp :
						command_e::TargetWarp,
					player.position,
					target_position
				};
			} else if (keyboard_s::is_key<input_e::Down>(bindings::CalciticInvocation)) {
				player.command = command_pack_t{ command_e::CalciticInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Down>(bindings::SpectralInvocation)) {
				player.command = command_pack_t{ command_e::SpectralInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Down>(bindings::SanguineInvocation)) {
				player.command = command_pack_t{ command_e::SanguineInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Down>(bindings::GalvanicInvocation)) {
				player.command = command_pack_t{ command_e::GalvanicInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Down>(bindings::NecromanticAscendance)) {
				player.command = command_pack_t{ command_e::NecromanticAscendance, player.position };
			}

			if (player.command.type != command_e::None) {
				return true;
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (keyboard_s::any_keys<input_e::Pressed>(bindings::CharacterMovement[cardinal_e::North])) {
					--y;
				} if (keyboard_s::any_keys<input_e::Pressed>(bindings::CharacterMovement[cardinal_e::South])) {
					++y;
				} if (keyboard_s::any_keys<input_e::Pressed>(bindings::CharacterMovement[cardinal_e::West])) {
					--x;
				} if (keyboard_s::any_keys<input_e::Pressed>(bindings::CharacterMovement[cardinal_e::East])) {
					++x;
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				const offset_t target_position{ player.position + direction };

				if (!game_map<MapType>.dependent within<region_e::Interior>(target_position) || game_map<MapType>[target_position].solid) {
					return false;
				}

				const command_e command_type{ !entity_registry<MapType>.empty(target_position) || (!ignore_objects && !object_registry<MapType>.empty(target_position)) ? player.clash_or_consume<MapType>(target_position) : command_e::Move };

				player.command = command_pack_t{ command_type, player.position, target_position };

				draw_warp_cursor = false;

				return true;
			}

			return false;
		}

		static inline void startup() noexcept {
#if !defined(STEAMLESS)
			if (SteamUtils()->IsSteamRunningOnSteamDeck()) {
				window.set_size(globals::Resolutions[globals::resolution_e::ResolutionSteamDeck]);
				window.set_fullscreen();
			}

			api_state.user_id = steam::user::get_steam_id();

			steam_stats::transcribe();
#endif

			mouse_s::initialize();
			keyboard_s::initialize();

			mouse_s::hide_cursor();

			Clock::tick();

			input_timer.reset();
			cursor_timer.reset();
			epoch_timer.reset();
			
#if !defined(STEAMLESS)
			stat_store_timer.reset();
#endif
		}

		template<map_type_e MapType> static inline void load() noexcept {
			game_map<MapType>.dependent reset<region_e::All>();
			fluid_map<MapType>.dependent reset<region_e::All>();

			entity_registry<MapType>.clear();
			object_registry<MapType>.clear();

			reset_patrons();

			player.patron = desired_patron;

			game_stats.reset();

			game_stats.cheats.activate();

			game_stats.cheats.no_hit = true;
			game_stats.cheats.free_costs = true;
			game_stats.cheats.bypass_invocations = true;

			scorekeeper.reset();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

			globals::randomize_map_config(random_engine);

			game_map<MapType>
				.dependent set<region_e::Border>(closed_state)
				.dependent generate<region_e::Interior>(
					random_engine,
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

			for (offset_t::scalar_t y{ 0 }; y < globals::MapSize<MapType>.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < globals::MapSize<MapType>.w; ++x) {
					const offset_t pos{ x, y };

					game_map<MapType>[pos].recalculate_index(game_map<MapType>, pos, cell_e::Solid);
					fluid_map<MapType>[pos].recalculate_index(fluid_map<MapType>, pos, fluid_e::None);
				}
			}

			fluid_map_dirty = false;

			cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
				error_log.add("could not find open position for player!");
				terminate_prematurely();
			}

			if constexpr (globals::SpawnTutorialPortal) {
				cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

				if (!portal_pos.has_value() || !object_registry<MapType>.add(portal_t{ portal_pos.value(), stability_e::Insightful })) {
					error_log.add("could not find open position for tutorial portal!");
					terminate_prematurely();
				}
			}

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

			object_registry<MapType>.dependent spawn<bones_t>(
				static_cast<usize>(globals::StartingBones),
				static_cast<u32>(globals::MinimumBoneDistance),

				decay_e::Animate
			);

			entity_registry<MapType>.recalculate_goal_map();
			object_registry<MapType>.recalculate_goal_map();

			phase.transition(phase_e::Playing);

			game_running = true;
			process_turn_async<MapType>();
		}

		template<map_type_e MapType> static inline void descend() noexcept {
			terminate_process_turn();

			randomize_patrons();
#if !defined(STEAMLESS)
			steam_stats::store();

			stat_store_timer.record();
#endif
			descent_flag = false;

			++game_stats.game_depth;
#if !defined(STEAMLESS)
			if (steam_stats_s::stats<steam_stat_e::LowestDepth>.get_value() > -static_cast<i32>(game_stats.game_depth)) {
				steam_stats_s::stats<steam_stat_e::LowestDepth> = -static_cast<i32>(game_stats.game_depth);
			}
#endif
			fluid_positions.clear();
			ladder_positions.clear();

			for (crauto [position, ladder] : object_registry_storage<ladder_t>) {
				if (!fluid_map<MapType>.dependent within<region_e::Interior>(position) || ladder.is_up_ladder()) {
					continue;
				}

				const fluid_e fluid{ fluid_map<MapType>[position] };

				if (fluid == fluid_e::None) {
					continue;
				}

				fluid_positions.add(position, fluid);
				ladder_positions.add(position);
			}

			game_map<MapType>.dependent reset<region_e::All>();
			fluid_map<MapType>.dependent reset<region_e::All>();

			entity_registry<MapType>.dependent clear<ALL_NON_PLAYER>();
			entity_registry<MapType>.dependent reset_goal_map<player_t>();

			entity_registry<MapType>.reset_unique_goal_maps();

			object_registry<MapType>.clear();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

			globals::randomize_map_config(random_engine);
			
			game_map<MapType>
				.dependent set<region_e::Border>(closed_state)
				.dependent generate<region_e::Interior>(
					random_engine,
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

			for (cauto [position, fluid] : fluid_positions) {
				if (game_map<MapType>[position] != cell_e::Open || fluid == fluid_e::None) {
					continue;
				}

				fluid_map<MapType>[position] = fluid;
			}

			fluid_positions.clear();

			for (offset_t::scalar_t y{ 0 }; y < globals::MapSize<MapType>.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < globals::MapSize<MapType>.w; ++x) {
					const offset_t position{ x, y };

					game_map<MapType>[position].recalculate_index(game_map<MapType>, position, cell_e::Solid);
					fluid_map<MapType>[position].recalculate_index(fluid_map<MapType>, position, fluid_e::None);
				}
			}

			fluid_map_dirty = false;
#if !defined(STEAMLESS)
			cauto previous_position{ player.position };
#endif
			if (game_map<MapType>[player.position].solid) {
				cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

				if (!player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
					error_log.add("could not find open position for player!");
					terminate_prematurely();
				}
			}
#if !defined(STEAMLESS)
			steam_stats_s::stats<steam_stat_e::MetersMoved> += offset_t::distance<f32>(previous_position, player.position);
#endif
			cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!portal_pos.has_value()) {
				error_log.add("could not find open position for return portal!");
				terminate_prematurely();
			}

			if constexpr (globals::SpawnTutorialPortal) {
				object_registry<MapType>.add(portal_t{ portal_pos.value(), stability_e::Insightful });
			}

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

			entity_registry<MapType>.recalculate_goal_map();
			object_registry<MapType>.recalculate_goal_map();

			phase.transition(phase_e::Playing);

			game_running = true;
			process_turn_async<MapType>();
		}

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void plunge() noexcept;

		template<map_type_e MapType> static inline void plunge() noexcept;

		template<map_type_e MapType> static inline void load_async() noexcept { std::thread([]() -> void { load<MapType>(); }).detach(); }

		template<map_type_e MapType> static inline void descend_async() noexcept { std::thread([]() -> void { descend<MapType>(); }).detach(); }

		template<map_type_e MapType> static inline void plunge_async() noexcept;

		template<map_type_e MapType> static inline void input() noexcept {
			if (window.is_closing()) {
				return;
			}

			if (globals::use_frame_limit) {
				Clock::tick(globals::frame_time());
			} else {
				Clock::tick();
			}

			window.poll_events();

			if (keyboard_s::is_key<input_e::Down>(bindings::ToggleFullscreen)) {
				window.toggle_fullscreen();
			}

			if (keyboard_s::is_key<input_e::Down>(bindings::Quit)) {
				switch(phase.current_phase) {
					case phase_e::MainMenu:
						phase.transition(phase_e::Exiting);
						break;
					case phase_e::Playing:
						phase.transition(phase_e::Paused);
						break;
					case phase_e::Paused:
						phase.transition(phase_e::Playing);
						break;
					case phase_e::Loading:
						break;
					default:
						phase.revert();
						break;
				}
			}

			if (phase.current_phase != phase_e::Playing) {
				return;
			}

			if (processing_turn) {
				return;
			}

			if (input_timer.ready()) {
				if (epoch_timer.ready() && !player_acted) {
					player_acted = character_input<MapType>();

					if (player_acted) {
						epoch_timer.record();
					}
				}

				if (player_acted || camera_input<MapType>()) {
					input_timer.record();
				}
			}
		}

		template<map_type_e MapType> static inline std::optional<offset_t> find_spawn_position() noexcept {
			for (crauto [position, ladder] : object_registry_storage<ladder_t>) {
				if (entity_registry<MapType>.dependent contains<ALL_GOOD_NPCS>(position) || ladder.is_down_ladder() || ladder.has_shackle()) {
					continue;
				}

				return position;
			}

			return std::nullopt;
		}

		template<map_type_e MapType> static inline bool spawn_random() noexcept {
			cauto maybe_spawn = find_spawn_position<MapType>();

			if (!maybe_spawn.has_value()) {
				return false;
			}

			const offset_t spawn_position{ maybe_spawn.value() };

			if constexpr (globals::OopsAllAdventurers) {
				entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{});

				return true;
			}

			if constexpr (globals::OopsAllMercenaries) {
				entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{});

				return true;
			}

			if constexpr (globals::OopsAllRangers) {
				entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{});

				return true;
			}

			if constexpr (globals::OopsAllSkulkers) {
				entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{});

				return true;
			}

			if constexpr (globals::OopsAllMistLadies) {
				entity_registry<MapType>.dependent add<true>(spawn_position, mist_lady_t{});

				return true;
			}

			if constexpr (globals::OopsAllBannerBearers) {
				entity_registry<MapType>.dependent add<true>(spawn_position, banner_bearer_t{});

				return true;
			}

			if constexpr (globals::OopsAllThetwo) {
				entity_registry<MapType>.dependent add<true>(spawn_position, thetwo_t{});

				return true;
			}

			if constexpr (globals::OopsAllBattleMonks) {
				entity_registry<MapType>.dependent add<true>(spawn_position, battle_monk_t{});

				return true;
			}

			if constexpr (globals::OopsAllBerserkers) {
				entity_registry<MapType>.dependent add<true>(spawn_position, berserker_t{});

				return true;
			}

			if constexpr (globals::OopsAllPaladins) {
				entity_registry<MapType>.dependent add<true>(spawn_position, paladin_t{ maybe_spawn.value(), random_engine });

				return true;
			}

			const u8 spawn_chance{ static_cast<u8>(globals::spawn_dis(random_engine)) };

			if (game_stats.wave_size <= globals::MassiveWaveSize && game_stats.wave_size > globals::HugeWaveSize) {
				if (spawn_chance < 12) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 12%
				} else if (spawn_chance < 60) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 48%
				} else if (spawn_chance < 72) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 12%
				} else if (spawn_chance < 84) {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 12%
				} else if (spawn_chance < 90) {
					entity_registry<MapType>.dependent add<true>(spawn_position, battle_monk_t{}); // 6%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, berserker_t{}); // 6%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, paladin_t{}); // 4%
				}
			} else if (game_stats.wave_size <= globals::HugeWaveSize && game_stats.wave_size > globals::LargeWaveSize) {
				if (spawn_chance < 36) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 36%
				} else if (spawn_chance < 72) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 36%
				} else if (spawn_chance < 80) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 8%
				} else if (spawn_chance < 88) {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 8%
				} else if (spawn_chance < 92) {
					entity_registry<MapType>.dependent add<true>(spawn_position, battle_monk_t{}); // 4%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, berserker_t{}); // 4%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, paladin_t{}); // 4%
				}
			} else if (game_stats.wave_size <= globals::LargeWaveSize && game_stats.wave_size > globals::MediumWaveSize) {
				if (spawn_chance < 50) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 50%
				} else if (spawn_chance < 74) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 24%
				} else if (spawn_chance < 84) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 10%
				} else if (spawn_chance < 94) {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 10%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, battle_monk_t{}); // 2%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.dependent add<true>(spawn_position, berserker_t{}); // 2%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, paladin_t{}); // 2%
				}
			} else if (game_stats.wave_size <= globals::MediumWaveSize && game_stats.wave_size > globals::SmallWaveSize) {
				if (spawn_chance < 80) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 80%
				} else if (spawn_chance < 90) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 10%
				} else if (spawn_chance < 94) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 4%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 4%
				} else if (spawn_chance < 99) {
					entity_registry<MapType>.dependent add<true>(spawn_position, battle_monk_t{}); // 1%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, berserker_t{}); // 1%
				}
			} else if (game_stats.wave_size <= globals::SmallWaveSize && game_stats.wave_size > globals::TinyWaveSize) {
				if (spawn_chance < 90) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 90%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 6%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 2%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 2%
				}
			} else if (game_stats.wave_size <= globals::TinyWaveSize && game_stats.wave_size > globals::MinisculeWaveSize) {
				if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 96%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 2%
				} else if (spawn_chance < 99) {
					entity_registry<MapType>.dependent add<true>(spawn_position, ranger_t{}); // 1%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, skulker_t{}); // 1%
				}
			} else {
				if (spawn_chance < 96) {
					entity_registry<MapType>.dependent add<true>(spawn_position, adventurer_t{}); // 99%
				} else {
					entity_registry<MapType>.dependent add<true>(spawn_position, mercenary_t{}); // 1%
				}
			}

			return true;
		}

		template<map_type_e MapType> static inline void process_turn() noexcept {
			if (window.is_closing() || !game_running || !player_acted || descent_flag || plunge_flag) {
				return;
			}

			buffer_access.lock<true>();

			entity_registry<MapType>.dependent store<ALL_ENTITIES>();
			object_registry<MapType>.dependent store<ALL_OBJECTS>();

			buffer_access.unlock();

			registry_access.lock<true>();

			processing_turn = true;

			game_stats.wave_size = clamp(
				static_cast<i16>(globals::StartingWaveSize + game_stats.total_kills() / globals::KillsPerPopulation),
				globals::MinimumWaveSize,
				globals::MaximumWaveSize
			);

			if (entity_registry<MapType>.dependent empty<ALL_GOOD_NPCS>() && !game_stats.has_spawns()) {
				game_stats.spawns_remaining = game_stats.wave_size;
			}

			while (game_stats.has_spawns()) {
				if (!spawn_random<MapType>()) {
					break;
				}
				
				--game_stats.spawns_remaining;
			}

			if (game_stats.has_reinforcements() && !game_stats.has_spawns()) {
				for (i16 i{ 0 }; i < game_stats.current_reinforcements(); ++i) {
					if (!spawn_random<MapType>()) {
						break;
					}
				}
			}
			
			entity_registry<MapType>.update();

			registry_access.unlock();

			update_camera<MapType>();

			processing_turn = false;
			
			player_acted = false;
		}

		static inline void terminate_process_turn() noexcept {
			game_running = false;

			while (processing_turn) {};
		}

		template<map_type_e MapType> static inline void process_turn_async() noexcept {
			std::thread([]() -> void {
				do { process_turn<MapType>(); } while (game_running);
			}).detach();
		}

		template<map_type_e MapType> static inline void update() noexcept;

		template<map_type_e MapType> static inline void render() noexcept {
			if (window.is_closing()) {
				return;
			}

			renderer.clear(colors::Black);

			if (phase.current_phase == phase_e::Playing) {
				bool exceeds_width{ globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w };
				bool exceeds_height{ globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h };

				const extent_t excess_size{ (globals::grid_size<grid_type_e::Game>() - globals::MapSize<MapType> + 1) * globals::cell_size<grid_type_e::Game> / 2 };

				if (exceeds_width) {
					renderer.draw_fill_rect(rect_t{ offset_t::Zero, extent_t{ excess_size.w, globals::window_size.h } }, color_t{ u8{ 0xC0 } });
					renderer.draw_fill_rect(rect_t{ offset_t{ globals::window_size.w - excess_size.w - 1, 0}, extent_t{ excess_size.w, globals::window_size.h } }, color_t{ u8{ 0xC0 } });
				}

				if (exceeds_height) {
					renderer.draw_fill_rect(rect_t{ offset_t::Zero, extent_t{ globals::window_size.w, excess_size.h } }, color_t{ u8{ 0xC0 } });
					renderer.draw_fill_rect(rect_t{ offset_t{ 0, globals::window_size.h - excess_size.h - 1 }, extent_t{ globals::window_size.w, excess_size.h } }, color_t{ u8{ 0xC0 } });
				}

				game_map<MapType>.draw(game_atlas, camera<MapType>, offset_t{}, globals::grid_origin<grid_type_e::Game>());
				fluid_map<MapType>.draw(game_atlas, camera<MapType>, offset_t{}, globals::grid_origin<grid_type_e::Game>());

				if (registry_access.try_lock()) {
					object_registry<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
					entity_registry<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());

					registry_access.unlock();
				} else if (buffer_access.try_lock()) {
					object_buffer<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
					entity_buffer<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());

					buffer_access.unlock();
				}
			}

			ui_registry.render<MapType>();

			renderer.present();
		}

		template<map_type_e MapType> static inline void unload() noexcept {
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

		template<map_type_e MapType> static inline void unload_async() noexcept {
			std::thread([]() -> void { unload<MapType>(); }).detach();
		}

		static inline void shutdown() noexcept {
			terminate_process_turn();

			keyboard_s::terminate();
			mouse_s::terminate();
#if defined (BLEAK_DEBUG)
			message_log.flush_to_console();
			error_log.flush_to_console();
#else
			message_log.flush_to_file();
			error_log.flush_to_file();
#endif
		}

		static inline void terminate_prematurely() noexcept {
			std::cout << "Message Log:\n";
			message_log.flush_to_console(std::cout);

			std::cerr << "\nError Log:\n";
			error_log.flush_to_console(std::cerr);

			shutdown();

			exit(EXIT_FAILURE);
		}
	};
} // namespace necrowarp
