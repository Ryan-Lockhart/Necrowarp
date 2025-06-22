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
				player.command = command_pack_t{
					!entity_registry<MapType>.empty(grid_cursor<MapType>.get_position()) || (!ignore_objects && !object_registry<MapType>.empty(grid_cursor<MapType>.get_position())) ?
						command_e::ConsumeWarp :
						command_e::TargetWarp,
					player.position,
					grid_cursor<MapType>.get_position()
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

				if (!game_map<MapType>.template within<zone_region_e::Interior>(target_position) || game_map<MapType>[target_position].solid) {
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
			game_stats.reset();

			scorekeeper.reset();

			reset_patrons();

			player.patron = desired_patron;

			game_stats.cheats.activate();

			game_stats.cheats.no_hit = true;
			game_stats.cheats.free_costs = true;
			game_stats.cheats.bypass_invocations = true;

			game_map<MapType>.template reset<zone_region_e::All>();
			fluid_map<MapType>.template reset<zone_region_e::All>();

			entity_registry<MapType>.reset();
			object_registry<MapType>.reset();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

			game_map<MapType>
				.template set<zone_region_e::Border>(closed_state)
				.template generate<zone_region_e::Interior>(
					random_engine,
					globals::map_config.fill_percent,
					globals::map_config.automata_iterations,
					globals::map_config.automata_threshold,
					cell_applicator
				)
				.template collapse<zone_region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

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
				}
			}

			cauto player_pos{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

			if (!player_pos.has_value()) {
				error_log.add("could not find open position for player!");
				terminate_prematurely();
			}

			player.position = player_pos.value();
			good_goal_map<MapType>.add(player_pos.value());

			cauto portal_pos{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

			if (!portal_pos.has_value()) {
				error_log.add("could not find open position for return portal!");
				terminate_prematurely();
			}

			object_registry<MapType>.add(portal_t{ portal_pos.value(), stability_e::Insightful });

			object_registry<MapType>.template spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_up_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Up
			);

			object_registry<MapType>.template spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_down_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Down, random_engine
			);

			object_registry<MapType>.template spawn<skull_t>(
				static_cast<usize>(globals::map_config.starting_skulls),
				static_cast<u32>(globals::map_config.minimum_skull_distance),

				decay_e::Animate
			);

			entity_registry<MapType>.recalculate_goal_map();
			object_registry<MapType>.recalculate_goal_map();

			phase_state_t<phase_e::Playing>::entity_buffer<MapType> = entity_registry<MapType>;
			phase_state_t<phase_e::Playing>::object_buffer<MapType> = object_registry<MapType>;

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

			game_map<MapType>.template reset<zone_region_e::All>();
			fluid_map<MapType>.template reset<zone_region_e::All>();

			entity_registry<MapType>.template reset<ALL_NON_PLAYER>();
			entity_registry<MapType>.template reset_goal_map<player_t>();

			entity_registry<MapType>.reset_alignment_goal_maps();

			object_registry<MapType>.reset();

			constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
			constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

			constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };
			
			game_map<MapType>
				.template set<zone_region_e::Border>(closed_state)
				.template generate<zone_region_e::Interior>(
					random_engine,
					globals::map_config.fill_percent,
					globals::map_config.automata_iterations,
					globals::map_config.automata_threshold,
					cell_applicator,
					ladder_positions
				)
				.template collapse<zone_region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

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
				}
			}

			if (game_map<MapType>[player.position].solid) {
				cauto player_pos{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

				if (!player_pos.has_value()) {
					error_log.add("could not find open position for player!");
					terminate_prematurely();
				}

#if !defined(STEAMLESS)
				steam_stats_s::stats<steam_stat_e::MetersMoved, f32> += offset_t::distance<f32>(player.position, player_pos.value());
#endif

				player.position = player_pos.value();
			}
			
			good_goal_map<MapType>.add(player.position);

			cauto portal_pos{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

			if (!portal_pos.has_value()) {
				error_log.add("could not find open position for return portal!");
				terminate_prematurely();
			}

			object_registry<MapType>.add(portal_t{ portal_pos.value(), stability_e::Insightful });

			i16 num_up_ladders_needed{ globals::map_config.number_of_up_ladders };

			while (!ladder_positions.empty()) {
				cauto position{ ladder_positions.back() };
				ladder_positions.pop_back();

				if (game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position) || !object_registry<MapType>.empty(position) || !object_registry<MapType>.add(ladder_t{ position, verticality_e::Up })) {
					continue;
				}

				--num_up_ladders_needed;
			}

			if (num_up_ladders_needed > 0) {
				object_registry<MapType>.template spawn<ladder_t>(
					static_cast<usize>(num_up_ladders_needed),
					static_cast<u32>(globals::map_config.minimum_ladder_distance),

					verticality_e::Up
				);
			}

			object_registry<MapType>.template spawn<ladder_t>(
				static_cast<usize>(globals::map_config.number_of_down_ladders),
				static_cast<u32>(globals::map_config.minimum_ladder_distance),

				verticality_e::Down, random_engine
			);

			object_registry<MapType>.template spawn<skull_t>(
				static_cast<usize>(globals::map_config.starting_skulls),
				static_cast<u32>(globals::map_config.minimum_skull_distance),

				decay_e::Animate
			);

			entity_registry<MapType>.recalculate_goal_map();
			object_registry<MapType>.recalculate_goal_map();

			buffers_locked = true;

			phase_state_t<phase_e::Playing>::entity_buffer<MapType> = entity_registry<MapType>;
			phase_state_t<phase_e::Playing>::object_buffer<MapType> = object_registry<MapType>;

			buffers_locked = false;

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
			for (cref<ladder_t> ladder : object_storage<ladder_t>) {
				if (entity_registry<MapType>.template contains<ALL_GOOD_NPCS>(ladder.position) || ladder.is_down_ladder() || ladder.has_shackle()) {
					continue;
				}

				return ladder.position;
			}

			return std::nullopt;
		}

		template<map_type_e MapType> static inline bool spawn_random() noexcept {
			cauto spawn_pos = find_spawn_position<MapType>();

			if (!spawn_pos.has_value()) {
				return false;
			}

			const u8 spawn_chance{ static_cast<u8>(globals::spawn_dis(random_engine)) };

			if constexpr (globals::OopsAllAdventurers) {
				entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllRangers) {
				entity_registry<MapType>.template add<true>(ranger_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllSkulkers) {
				entity_registry<MapType>.template add<true>(skulker_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllBattleMonks) {
				entity_registry<MapType>.template add<true>(battle_monk_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllMercenaries) {
				entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllPaladins) {
				entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() });

				return true;
			}

			if constexpr (globals::OopsAllBerserkers) {
				entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() });

				return true;
			}

			if (game_stats.wave_size >= globals::MassiveWaveSize) {
				if (spawn_chance < 40) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 40%
				} else if (spawn_chance < 80) {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 40%
				} else if (spawn_chance < 90) {
					entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() }); // 10%
				} else {
					entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() }); // 10%
				}
			} else if (game_stats.wave_size >= globals::HugeWaveSize) {
				if (spawn_chance < 50) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 50%
				} else if (spawn_chance < 80) {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 30%
				} else if (spawn_chance < 90) {
					entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() }); // 10%
				} else {
					entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() }); // 10%
				}
			} else if (game_stats.wave_size >= globals::LargeWaveSize) {
				if (spawn_chance < 70) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 70%
				} else if (spawn_chance < 90) {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 20%
				} else if (spawn_chance < 95) {
					entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() }); // 5%
				} else {
					entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() }); // 5%
				}
			} else if (game_stats.wave_size >= globals::MediumWaveSize) {
				if (spawn_chance < 80) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 80%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 16%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() }); // 2%
				} else {
					entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() }); // 2%
				}
			} else if (game_stats.wave_size >= globals::SmallWaveSize) {
				if (spawn_chance < 90) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 90%
				} else if (spawn_chance < 96) {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 6%
				} else if (spawn_chance < 98) {
					entity_registry<MapType>.template add<true>(battle_monk_t{ spawn_pos.value() }); // 2%
				} else if (spawn_chance < 99) {
					entity_registry<MapType>.template add<true>(paladin_t{ spawn_pos.value() }); // 1%
				} else {
					entity_registry<MapType>.template add<true>(berserker_t{ spawn_pos.value() }); // 1%
				}
			} else {
				if (spawn_chance < 99) {
					entity_registry<MapType>.template add<true>(adventurer_t{ spawn_pos.value() }); // 99%
				} else {
					entity_registry<MapType>.template add<true>(mercenary_t{ spawn_pos.value() }); // 1%
				}
			}

			return true;
		}

		template<map_type_e MapType> static inline void process_turn() noexcept {
			if (window.is_closing() || !player_acted || descent_flag) {
				return;
			}

			processing_turn = true;

			game_stats.wave_size = clamp(
				static_cast<i16>(globals::map_config.starting_wave + game_stats.total_kills() / globals::KillsPerPopulation),
				globals::MinimumWaveSize,
				globals::MaximumWaveSize
			);

			if (entity_registry<MapType>.template empty<ALL_GOOD_NPCS>() && !game_stats.has_spawns()) {
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

			buffers_locked = true;

			phase_state_t<phase_e::Playing>::entity_buffer<MapType> = entity_registry<MapType>;
			phase_state_t<phase_e::Playing>::object_buffer<MapType> = object_registry<MapType>;

			buffers_locked = false;

			player_acted = false;
			processing_turn = false;
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
			constexpr bool render_async{ IsReleaseBuild };

			if (window.is_closing() || (processing_turn && render_async)) {
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

				if (!processing_turn) {
					object_registry<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
					entity_registry<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
				} else if (!buffers_locked) {
					phase_state_t<phase_e::Playing>::object_buffer<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
					phase_state_t<phase_e::Playing>::entity_buffer<MapType>.draw(camera<MapType>, globals::grid_origin<grid_type_e::Game>());
				} else {
					return;
				}

			}

			ui_registry.render<MapType>();

			renderer.present();
		}

		template<map_type_e MapType> static inline void unload() noexcept {
			terminate_process_turn();

			game_map<MapType>.template reset<zone_region_e::All>();
			
			entity_registry<MapType>.reset();
			object_registry<MapType>.reset();

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
