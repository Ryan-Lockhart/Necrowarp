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

#include <magic_enum/magic_enum_all.hpp>

namespace necrowarp {
	using namespace bleak;

	class Game {
	  public:
		static inline int run() noexcept {
			if constexpr (IsReleaseBuild) {
				subsystem.initialize(api_state.app_id);
			} else {
				subsystem.initialize();
			}

			if (!subsystem.is_initialized()) {
				error_log.add("[ERROR]: a subsystem failed to initialize! see error(s) above for details...");

				return EXIT_FAILURE;
			}

			startup();

			do {
				input();
				update();
				render();
			} while (window.is_running());

			shutdown();

			subsystem.terminate();

			return EXIT_SUCCESS;
		};

	  private:
		static inline bool camera_input() noexcept {
			if (update_camera()) {
				return true;
			}

			if (Keyboard::is_key_down(bindings::CameraLock)) {
				camera_locked = !camera_locked;
			}

			if (camera_locked) {
				return false;
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_e::North])) {
					--y;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_e::South])) {
					++y;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_e::West])) {
					--x;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_e::East])) {
					++x;
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				return camera.move(direction * globals::camera_speed);
			}

			return false;
		}

		static inline bool character_input() noexcept {
			player.command = command_pack_t{};

			const bool ignore_objects{ Keyboard::is_key_pressed(bindings::IgnoreObjects) };

			if (Keyboard::any_keys_pressed<2>(bindings::Wait)) {
				player.command = command_pack_t{ command_e::None };

				return true;
			} else if (Keyboard::is_key_down(bindings::RandomWarp)) {
				player.command = command_pack_t{ command_e::RandomWarp, player.position };
			} else if (Keyboard::is_key_down(bindings::TargetWarp)) {
				player.command = command_pack_t{
					!entity_registry.empty(grid_cursor.get_position()) || (!ignore_objects && !object_registry.empty(grid_cursor.get_position())) ?
						command_e::ConsumeWarp :
						command_e::TargetWarp,
					player.position,
					grid_cursor.get_position()
				};
			} else if (Keyboard::is_key_down(bindings::CalciticInvocation)) {
				player.command = command_pack_t{ command_e::CalciticInvocation, player.position, player.position };
			} else if (Keyboard::is_key_down(bindings::SpectralInvocation)) {
				player.command = command_pack_t{ command_e::SpectralInvocation, player.position, player.position };
			} else if (Keyboard::is_key_down(bindings::SanguineInvocation)) {
				player.command = command_pack_t{ command_e::SanguineInvocation, player.position, player.position };
			} else if (Keyboard::is_key_down(bindings::GalvanicInvocation)) {
				player.command = command_pack_t{ command_e::GalvanicInvocation, player.position, player.position };
			} else if (Keyboard::is_key_down(bindings::NecromanticAscendance)) {
				player.command = command_pack_t{ command_e::NecromanticAscendance, player.position };
			}

			if (player.command.type != command_e::None) {
				return true;
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (Keyboard::any_keys_pressed<4>(bindings::CharacterMovement[cardinal_e::North])) {
					--y;
				} if (Keyboard::any_keys_pressed<4>(bindings::CharacterMovement[cardinal_e::South])) {
					++y;
				} if (Keyboard::any_keys_pressed<4>(bindings::CharacterMovement[cardinal_e::West])) {
					--x;
				} if (Keyboard::any_keys_pressed<4>(bindings::CharacterMovement[cardinal_e::East])) {
					++x;
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				const offset_t target_position{ player.position + direction };

				if (!game_map.within<zone_region_e::Interior>(target_position) || game_map[target_position].solid) {
					return false;
				}

				const command_e command_type{ !entity_registry.empty(target_position) || (!ignore_objects && !object_registry.empty(target_position)) ? player.clash_or_consume(target_position) : command_e::Move };

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

			Mouse::initialize();
			Keyboard::initialize();

			Mouse::hide_cursor();

			Clock::tick();

			input_timer.reset();
			cursor_timer.reset();
			epoch_timer.reset();
			
#if !defined(STEAMLESS)
			stat_store_timer.reset();
#endif
		}

		static inline void load() noexcept {
			game_stats.reset();

			scorekeeper.reset();

			reset_patrons();

			player.patron = desired_patron;

			game_stats.cheats.activate();

			game_stats.cheats.no_hit = true;
			game_stats.cheats.free_costs = true;
			game_stats.cheats.bypass_invocations = true;

			game_map.reset<zone_region_e::All>();
			fluid_map.reset<zone_region_e::All>();

			entity_registry.reset();
			object_registry.reset();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

			game_map
				.set<zone_region_e::Border>(closed_state)
				.generate<zone_region_e::Interior>(
					random_engine,
					globals::map_config.fill_percent,
					globals::map_config.automata_iterations,
					globals::map_config.automata_threshold,
					cell_applicator
				)
				.collapse<zone_region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

			std::vector<area_t> areas{ area_t::partition(game_map, cell_e::Open) };

			if (areas.size() > 1) {
				cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

				for (crauto area : areas) {
					if (area != largest_area) {
						area.apply(game_map, cell_e::Solid);
					}
				}
			}

			for (offset_t::scalar_t y{ 0 }; y < globals::MapSize.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < globals::MapSize.w; ++x) {
					const offset_t pos{ x, y };

					game_map[pos].recalculate_index(game_map, pos, cell_e::Solid);
				}
			}

			cauto player_pos{ game_map.find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

			if (!player_pos.has_value()) {
				error_log.add("could not find open position for player!");
				terminate_prematurely();
			}

			player.position = player_pos.value();
			good_goal_map.add(player_pos.value());

			object_registry.spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_up_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Up
			);

			object_registry.spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_down_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Down, true
			);

			object_registry.spawn<skull_t>(
				static_cast<usize>(globals::map_config.starting_skulls),
				static_cast<u32>(globals::map_config.minimum_skull_distance),

				decay_e::Animate
			);

			entity_registry.recalculate_goal_map();
			object_registry.recalculate_goal_map();

			phase.transition(phase_e::Playing);

			game_running = true;
			process_turn_async();
		}

		static inline void descend() noexcept {
			terminate_process_turn();

			randomize_patrons();
#if !defined(STEAMLESS)
			steam_stats::store();

			stat_store_timer.record();
#endif
			descent_flag = false;

			++game_stats.game_depth;
#if !defined(STEAMLESS)
			if (steam_stats_s::stats<steam_stat_e::LowestDepth, i32>.get_value() > -static_cast<i32>(game_stats.game_depth)) {
				steam_stats_s::stats<steam_stat_e::LowestDepth, i32> = -static_cast<i32>(game_stats.game_depth);
			}
#endif
			ladder_positions.clear();

			for (crauto ladder : object_storage<ladder_t>) {
				if (ladder.is_up_ladder()) {
					continue;
				}

				ladder_positions.push_back(ladder.position);
			}

			game_map.reset<zone_region_e::All>();
			fluid_map.reset<zone_region_e::All>();

			entity_registry.reset<ALL_NON_PLAYER>();
			entity_registry.reset_goal_map<player_t>();

			entity_registry.reset_alignment_goal_maps();

			object_registry.reset();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };
			
			game_map
				.set<zone_region_e::Border>(closed_state)
				.generate<zone_region_e::Interior>(
					random_engine,
					globals::map_config.fill_percent,
					globals::map_config.automata_iterations,
					globals::map_config.automata_threshold,
					cell_applicator,
					ladder_positions
				)
				.collapse<zone_region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

			std::vector<area_t> areas{ area_t::partition(game_map, cell_e::Open) };

			if (areas.size() > 1) {
				cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

				for (crauto area : areas) {
					if (area != largest_area) {
						area.apply(game_map, cell_e::Solid);
					}
				}
			}

			for (offset_t::scalar_t y{ 0 }; y < globals::MapSize.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < globals::MapSize.w; ++x) {
					const offset_t pos{ x, y };

					game_map[pos].recalculate_index(game_map, pos, cell_e::Solid);
				}
			}

			if (game_map[player.position].solid) {
				cauto player_pos{ game_map.find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

				if (!player_pos.has_value()) {
					error_log.add("could not find open position for player!");
					terminate_prematurely();
				}

#if !defined(STEAMLESS)
				steam_stats_s::stats<steam_stat_e::MetersMoved, f32> += offset_t::distance<f32>(player.position, player_pos.value());
#endif

				player.position = player_pos.value();
			}
			
			good_goal_map.add(player.position);

			i16 num_up_ladders_needed{ globals::map_config.number_of_up_ladders };

			while (!ladder_positions.empty()) {
				cauto position{ ladder_positions.back() };
				ladder_positions.pop_back();

				if (game_map[position].solid || !entity_registry.empty(position) || !object_registry.empty(position) || !object_registry.add(ladder_t{ position, verticality_e::Up })) {
					continue;
				}

				--num_up_ladders_needed;
			}

			if (num_up_ladders_needed > 0) {
				object_registry.spawn<ladder_t>(
					static_cast<usize>(num_up_ladders_needed),
					static_cast<u32>(globals::map_config.minimum_ladder_distance),

					verticality_e::Up
				);
			}

			object_registry.spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_down_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Down, true
			);

			object_registry.spawn<skull_t>(
				static_cast<usize>(globals::map_config.starting_skulls),
				static_cast<u32>(globals::map_config.minimum_skull_distance),

				decay_e::Animate
			);

			entity_registry.recalculate_goal_map();
			object_registry.recalculate_goal_map();

			phase.transition(phase_e::Playing);

			game_running = true;
			process_turn_async();
		}

		static inline void load_async() noexcept {
			std::thread([]() -> void { load(); }).detach();
		}

		static inline void descend_async() noexcept {
			std::thread([]() -> void { descend(); }).detach();
		}

		static inline void input() noexcept {
			if (window.is_closing()) {
				return;
			}

			if (globals::use_frame_limit) {
				Clock::tick(globals::frame_time());
			} else {
				Clock::tick();
			}

			window.poll_events();

			if (Keyboard::is_key_down(bindings::ToggleFullscreen)) {
				window.toggle_fullscreen();
			}

			if (Keyboard::is_key_down(bindings::Quit)) {
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
					player_acted = character_input();

					if (player_acted) {
						epoch_timer.record();
					}
				}

				if (player_acted || camera_input()) {
					input_timer.record();
				}
			}
		}

		static inline std::optional<offset_t> find_spawn_position() noexcept {
			for (cref<ladder_t> ladder : object_storage<ladder_t>) {
				if (entity_registry.contains<ALL_GOOD_NPCS>(ladder.position) || ladder.is_down_ladder() || ladder.has_shackle()) {
					continue;
				}

				return ladder.position;
			}

			return std::nullopt;
		}

		static inline bool spawn_random() noexcept {
			cauto spawn_pos = find_spawn_position();

			if (!spawn_pos.has_value()) {
				return false;
			}

			const u8 spawn_chance{ static_cast<u8>(globals::spawn_dis(random_engine)) };

			if constexpr (globals::OopsAllAdventurers) {
				entity_registry.add<true>(adventurer_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllMercenaries) {
				entity_registry.add<true>(mercenary_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllPaladins) {
				entity_registry.add<true>(paladin_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllPriests) {
				entity_registry.add<true>(priest_t{ spawn_pos.value() });

				return true;
			}

			if (game_stats.wave_size >= globals::MassiveWaveSize) {
				if (spawn_chance < 80) {
					entity_registry.add<true>(mercenary_t{ spawn_pos.value() }); // 80%
				} else if (spawn_chance < 98) {
					entity_registry.add<true>(paladin_t{ spawn_pos.value() }); // 16%
				} else {
					entity_registry.add<true>(priest_t{ spawn_pos.value() }); // 2%
				}
			} else if (game_stats.wave_size >= globals::HugeWaveSize) {
				if (spawn_chance < 50) {
					entity_registry.add<true>(adventurer_t{ spawn_pos.value() }); // 50%
				} else if (spawn_chance < 90) {
					entity_registry.add<true>(mercenary_t{ spawn_pos.value() }); // 40%
				} else if (spawn_chance < 98) {
					entity_registry.add<true>(paladin_t{ spawn_pos.value() }); // 8%
				} else {
					entity_registry.add<true>(priest_t{ spawn_pos.value() }); // 2%
				}
			} else if (game_stats.wave_size >= globals::LargeWaveSize) {
				if (spawn_chance < 70) {
					entity_registry.add<true>(adventurer_t{ spawn_pos.value() }); // 70%
				} else if (spawn_chance < 94) {
					entity_registry.add<true>(mercenary_t{ spawn_pos.value() }); // 24%
				} else if (spawn_chance < 98) {
					entity_registry.add<true>(paladin_t{ spawn_pos.value() }); // 4%
				} else {
					entity_registry.add<true>(priest_t{ spawn_pos.value() }); // 2%
				}
			} else if (game_stats.wave_size >= globals::MediumWaveSize) {
				if (spawn_chance < 80) {
					entity_registry.add<true>(adventurer_t{ spawn_pos.value() }); // 80%
				} else if (spawn_chance < 97) {
					entity_registry.add<true>(mercenary_t{ spawn_pos.value() }); // 17%
				} else if (spawn_chance < 99) {
					entity_registry.add<true>(paladin_t{ spawn_pos.value() }); // 2%
				} else {
					entity_registry.add<true>(priest_t{ spawn_pos.value() }); // 1%
				}
			} else if (game_stats.wave_size >= globals::SmallWaveSize) {
				if (spawn_chance < 90) {
					entity_registry.add<true>(adventurer_t{ spawn_pos.value() }); // 90%
				} else if (spawn_chance < 99) {
					entity_registry.add<true>(mercenary_t{ spawn_pos.value() }); // 9%
				} else {
					entity_registry.add<true>(paladin_t{ spawn_pos.value() }); // 1%
				}
			} else {
				entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
			}

			return true;
		}

		static inline void process_turn() noexcept {
			if (window.is_closing() || !player_acted || descent_flag) {
				return;
			}

			processing_turn = true;

			game_stats.wave_size = clamp(
				static_cast<i16>(globals::map_config.starting_adventurers + game_stats.total_kills() / globals::KillsPerPopulation),
				globals::MinimumWaveSize,
				globals::MaximumWaveSize
			);

			if (entity_registry.empty<ALL_GOOD_NPCS>() && !game_stats.has_spawns()) {
				game_stats.spawns_remaining = game_stats.wave_size;
			}

			while (game_stats.has_spawns()) {
				if (!spawn_random()) {
					break;
				}
				
				--game_stats.spawns_remaining;
			}

			if (game_stats.has_reinforcements() && !game_stats.has_spawns()) {
				for (i16 i{ 0 }; i < game_stats.current_reinforcements(); ++i) {
					if (!spawn_random()) {
						break;
					}
				}
			}
			
			entity_registry.update();

			player_acted = false;
			processing_turn = false;
		}

		static inline void terminate_process_turn() noexcept {
			game_running = false;

			while (processing_turn) {};
		}

		static inline void process_turn_async() noexcept {
			std::thread([]() -> void {
				do { process_turn(); } while (game_running);
			}).detach();
		}

		static inline void update() noexcept {
			sine_wave.update<wave_e::Sine>(Clock::elapsed());

			ui_registry.update();

			if (descent_flag) {
				phase.transition(phase_e::Loading);
				phase.previous_phase = phase_e::Loading;

				descend_async();
			}

			if (phase.current_phase == phase_e::Loading && phase.previous_phase != phase_e::Loading) {
				phase.previous_phase = phase_e::Loading;

				load_async();
			}

			if (phase.current_phase == phase_e::GameOver && phase.previous_phase != phase_e::GameOver) {
				phase.previous_phase = phase_e::GameOver;

				unload_async();
			}

#if !defined(STEAMLESS)
			if (stat_store_timer.ready()) {
				steam_stats::store();

				stat_store_timer.record();
			}
#endif
		}

		static inline void render() noexcept {
			if (window.is_closing()) {
				return;
			}

			renderer.clear(colors::Black);

			if (phase.current_phase == phase_e::Playing) {
				bool exceeds_width{ globals::MapSize.w <= globals::grid_size<grid_type_e::Game>().w };
				bool exceeds_height{ globals::MapSize.h <= globals::grid_size<grid_type_e::Game>().h };

				const extent_t excess_size{ (globals::grid_size<grid_type_e::Game>() - globals::MapSize + 1) * globals::cell_size<grid_type_e::Game> / 2 };

				if (exceeds_width) {
					renderer.draw_fill_rect(rect_t{ offset_t::Zero, extent_t{ excess_size.w, globals::window_size.h } }, color_t { 0xC0 });
					renderer.draw_fill_rect(rect_t{ offset_t{ globals::window_size.w - excess_size.w - 1, 0}, extent_t{ excess_size.w, globals::window_size.h } }, color_t { 0xC0 });
				}

				if (exceeds_height) {
					renderer.draw_fill_rect(rect_t{ offset_t::Zero, extent_t{ globals::window_size.w, excess_size.h } }, color_t { 0xC0 });
					renderer.draw_fill_rect(rect_t{ offset_t{ 0, globals::window_size.h - excess_size.h - 1 }, extent_t{ globals::window_size.w, excess_size.h } }, color_t { 0xC0 });
				}

				game_map.draw(tile_atlas, camera, offset_t{}, globals::grid_origin<grid_type_e::Game>());
				fluid_map.draw(tile_atlas, camera, offset_t{}, globals::grid_origin<grid_type_e::Game>());

				object_registry.draw(camera, globals::grid_origin<grid_type_e::Game>());
				entity_registry.draw(camera, globals::grid_origin<grid_type_e::Game>());
			}

			ui_registry.render();

			renderer.present();
		}

		static inline void unload() noexcept {
			terminate_process_turn();

			game_map.reset<zone_region_e::All>();
			
			entity_registry.reset();
			object_registry.reset();

#if !defined(STEAMLESS)
			steam_stats::store();

			stat_store_timer.record();
#endif
		}

		static inline void unload_async() noexcept {
			std::thread([]() -> void { unload(); }).detach();
		}

		static inline void shutdown() noexcept {
			terminate_process_turn();

			Keyboard::terminate();
			Mouse::terminate();
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
