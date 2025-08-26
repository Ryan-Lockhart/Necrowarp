#pragma once

#include <bleak.hpp>

#include <cstdlib>
#include <iostream>

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
#include <necrowarp/literature.hpp>
#include <necrowarp/unified_map_renderer.hpp>

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

			if constexpr (!IsReleaseBuild) {
				if (keyboard_s::is_key<input_e::Down>(keys::Function::Eight)) {
					steam_stats::reset();
				}

				if (keyboard_s::is_key<input_e::Down>(keys::Function::Nine)) {
					steam_stats::unlock();
				}
			}

			if (keyboard_s::any_keys<input_e::Pressed>(bindings::Wait)) {
				player.command = command_pack_t{ command_e::None };

				return true;
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::ChaoticWarp)) {
				player.command = command_pack_t{ command_e::ChaoticWarp, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::PreciseWarp)) {
				const offset_t target_position{ grid_cursor<MapType>.get_position() };

				const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(target_position) };

				if (maybe_entity.has_value()) {
					const entity_e target_entity{ maybe_entity.value() };

					if (player.can_consume(target_entity)) {
						player.command = command_pack_t{ command_e::ConsumeWarp, player.position, target_position };
					}
				} else {
					const object_group_e objects{ object_registry<MapType>.at(target_position) };

					const object_e target_object{ determine_target<player_t>(objects) };

					player.command = command_pack_t{
						(!ignore_objects && player.can_consume(target_object)) ?
							command_e::ConsumeWarp :
							command_e::PreciseWarp,
						player.position,
						target_position
					};	
				}
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::Annihilate)) {
				player.command = command_pack_t{ command_e::Annihilate, player.position, grid_cursor<MapType>.get_position() };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::Repulse)) {
				player.command = command_pack_t{ command_e::Repulse, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::Calcify)) {
				player.command = command_pack_t{ command_e::Calcify, player.position, grid_cursor<MapType>.get_position() };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::Incorporealize)) {
				player.command = command_pack_t{ command_e::Incorporealize, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::CalciticInvocation)) {
				player.command = command_pack_t{ command_e::CalciticInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::SpectralInvocation)) {
				player.command = command_pack_t{ command_e::SpectralInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::SanguineInvocation)) {
				player.command = command_pack_t{ command_e::SanguineInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::GalvanicInvocation)) {
				player.command = command_pack_t{ command_e::GalvanicInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::RavenousInvocation)) {
				player.command = command_pack_t{ command_e::RavenousInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::WretchedInvocation)) {
				player.command = command_pack_t{ command_e::WretchedInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::CerebralInvocation)) {
				player.command = command_pack_t{ command_e::CerebralInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::InfernalInvocation)) {
				player.command = command_pack_t{ command_e::InfernalInvocation, player.position, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::NecromanticAscendance)) {
				player.command = command_pack_t{ command_e::NecromanticAscendance, player.position };
			} else if (keyboard_s::is_key<input_e::Pressed>(bindings::CalamitousRetaliation)) {
				player.command = command_pack_t{ command_e::CalamitousRetaliation, player.position };
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

				if (!game_map<MapType>.dependent within<region_e::Interior>(target_position) || (game_map<MapType>[target_position].solid && !player.is_incorporeal())) {
					return false;
				}

				const command_e command_type{ !entity_registry<MapType>.empty(target_position) || (!ignore_objects && !object_registry<MapType>.empty(target_position)) ? player.clash_or_consume<MapType>(target_position) : command_e::Move };

				player.command = command_pack_t{ command_type, player.position, target_position };

				warped_from = std::nullopt;

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

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void load() noexcept;

		template<map_type_e MapType> static inline void load() noexcept;

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void descend() noexcept;

		template<map_type_e MapType> static inline void descend() noexcept;

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void plunge() noexcept;

		template<map_type_e MapType> static inline void plunge() noexcept;

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void process_turn() noexcept;

		template<map_type_e MapType> static inline void process_turn() noexcept;

		template<dimension_e Dimension> requires is_material<Dimension>::value static inline void unload() noexcept;

		template<map_type_e MapType> static inline void unload() noexcept;

		template<map_type_e MapType> static inline void load_async() noexcept;

		template<map_type_e MapType> static inline void descend_async() noexcept;

		template<map_type_e MapType> static inline void plunge_async() noexcept;

		template<map_type_e MapType> static inline void process_turn_async() noexcept;

		template<map_type_e MapType> static inline void unload_async() noexcept;

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

			if (input_timer.ready()) {
				if (camera_input<MapType>()) {
					input_timer.record();
				} else if (keyboard_s::is_key<input_e::Down>(keys::Alpha::M)) {
					phase_state_t<phase_e::Playing>::show_big_map = !phase_state_t<phase_e::Playing>::show_big_map;

					input_timer.record();
				} else if (keyboard_s::is_key<input_e::Down>(bindings::HastenTimestep) && hasten_timestep()) {
					input_timer.record();
				} else if (keyboard_s::is_key<input_e::Down>(bindings::HarryTimestep) && harry_timestep()) {
					input_timer.record();
				}
			}

			static command_pack_t cached_command{};

			if constexpr (globals::EnableRushMode) {
				if (!globals::rush_mode_toggle && processing_turn) {
					return;
				}

				if (processing_turn) {
					if (character_input<MapType>()) {
						cached_command = player.command;

						cache_timer.record();
					}

					return;
				}

				if (cache_timer.ready()) {
					cached_command = {};
				}
			} else {
				if (processing_turn) {
					return;
				}
			}

			if constexpr (globals::EnableRushMode) {
				if (globals::rush_mode_toggle) {
					if (epoch_timer.ready() || rush_timer.ready()) {
						if (character_input<MapType>() && epoch_timer.ready()) {
							epoch_timer.record();
							rush_timer.record();
		
							player_acted = true;
						} else if (rush_timer.ready()) {
							rush_timer.record();
		
							if (cached_command.type != command_e::None) {
								player.command = cached_command;
							}
		
							player_acted = true;
						}
					}
				} else {
					if (!player_acted && epoch_timer.ready()) {
						player_acted = character_input<MapType>();
	
						if (player_acted) {
							epoch_timer.record();
						}
					}
				}				
			} else {
				if (!player_acted && epoch_timer.ready()) {
					player_acted = character_input<MapType>();

					if (player_acted) {
						epoch_timer.record();
					}
				}
			}
		}

		template<map_type_e MapType, object_e Target>
			requires (Target == object_e::Ladder || Target == object_e::Crevice)
		static inline std::optional<offset_t> find_spawn_position() noexcept {
			using object_type = typename to_object_type<Target>::type;

			for (crauto [position, object] : object_registry_storage<object_type>) {
				if (!entity_registry<MapType>.empty(position)) {
					continue;
				}

				if constexpr (Target == object_e::Ladder) {
					if (object.is_down_ladder() || object.has_shackle()) {
						continue;
					}
				}

				if constexpr (Target == object_e::Crevice) {
					if (entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_NEUTRAL>(position)) {
						continue;
					}
				}

				return position;
			}

			return std::nullopt;
		}

		template<map_type_e MapType, wave_size_e Size> static inline void spawn_good(offset_t position, u8 chance) noexcept {
			if constexpr (Size == wave_size_e::Miniscule) {
				if (chance < 99) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 99%
				} else {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 1%
				}
			} else if constexpr (Size == wave_size_e::Tiny) {
				if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 96%
				} else if (chance < 98) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 2%
				} else if (chance < 99) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 1%
				} else {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 1%
				}
			} else if constexpr (Size == wave_size_e::Small) {
				if (chance < 90) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 90%
				} else if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 6%
				} else if (chance < 98) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 2%
				} else {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 2%
				}
			} else if constexpr (Size == wave_size_e::Medium) {
				if (chance < 80) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 80%
				} else if (chance < 92) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 12%
				} else if (chance < 94) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 2%
				} else if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 2%
				} else if (chance < 97) {
					entity_registry<MapType>.dependent add<true>(position, mist_lady_t{}); // 1%
				} else if (chance < 98) {
					entity_registry<MapType>.dependent add<true>(position, banner_bearer_t{}); // 1%
				} else if (chance < 99) {
					entity_registry<MapType>.dependent add<true>(position, battle_monk_t{}); // 1%
				} else {
					entity_registry<MapType>.dependent add<true>(position, berserker_t{}); // 1%
				}
			} else if constexpr (Size == wave_size_e::Large) {
				if (chance < 50) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 50%
				} else if (chance < 74) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 24%
				} else if (chance < 82) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 8%
				} else if (chance < 90) {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 8%
				} else if (chance < 92) {
					entity_registry<MapType>.dependent add<true>(position, mist_lady_t{}); // 2%
				} else if (chance < 94) {
					entity_registry<MapType>.dependent add<true>(position, banner_bearer_t{}); // 2%
				} else if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, battle_monk_t{}); // 2%
				} else if (chance < 98) {
					entity_registry<MapType>.dependent add<true>(position, berserker_t{}); // 2%
				} else {
					entity_registry<MapType>.dependent add<true>(position, paladin_t{ random_engine }); // 2%
				}
			} else if constexpr (Size == wave_size_e::Huge) {
				if (chance < 32) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 32%
				} else if (chance < 68) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 36%
				} else if (chance < 76) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 8%
				} else if (chance < 84) {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 8%
				} else if (chance < 86) {
					entity_registry<MapType>.dependent add<true>(position, mist_lady_t{}); // 2%
				} else if (chance < 88) {
					entity_registry<MapType>.dependent add<true>(position, banner_bearer_t{}); // 2%
				} else if (chance < 92) {
					entity_registry<MapType>.dependent add<true>(position, battle_monk_t{}); // 4%
				} else if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, berserker_t{}); // 4%
				} else {
					entity_registry<MapType>.dependent add<true>(position, paladin_t{ random_engine }); // 4%
				}
			} else if constexpr (Size == wave_size_e::Massive) {
				if (chance < 8) {
					entity_registry<MapType>.dependent add<true>(position, adventurer_t{}); // 8%
				} else if (chance < 56) {
					entity_registry<MapType>.dependent add<true>(position, mercenary_t{}); // 48%
				} else if (chance < 68) {
					entity_registry<MapType>.dependent add<true>(position, ranger_t{}); // 12%
				} else if (chance < 80) {
					entity_registry<MapType>.dependent add<true>(position, skulker_t{}); // 12%
				} else if (chance < 82) {
					entity_registry<MapType>.dependent add<true>(position, mist_lady_t{}); // 2%
				} else if (chance < 84) {
					entity_registry<MapType>.dependent add<true>(position, banner_bearer_t{}); // 2%
				} else if (chance < 90) {
					entity_registry<MapType>.dependent add<true>(position, battle_monk_t{}); // 6%
				} else if (chance < 96) {
					entity_registry<MapType>.dependent add<true>(position, berserker_t{}); // 6%
				} else {
					entity_registry<MapType>.dependent add<true>(position, paladin_t{ random_engine }); // 4%
				}
			}
		}

		template<map_type_e MapType> static inline bool spawn_good() noexcept {
			cauto maybe_spawn = find_spawn_position<MapType, object_e::Ladder>();

			if (!maybe_spawn.has_value()) {
				return false;
			}

			const offset_t spawn_position{ maybe_spawn.value() };

			if constexpr (globals::OopsAllEnabled) {
				return entity_registry<MapType>.dependent add<true>(spawn_position, to_entity_type<globals::OopsAllEnum>::type{});
			}

			magic_enum::enum_switch([&](auto val) {
				constexpr wave_size_e cval{ val };

				return spawn_good<MapType, cval>(spawn_position, game_stats.spawn_chance(random_engine));
			}, game_stats.determine_wave_size());

			return true;
		}

		template<map_type_e MapType, GoodEntity... EntityTypes> static inline bool spawn_offmap() noexcept {
			cauto maybe_spawn = find_spawn_position<MapType, object_e::Ladder>();

			if (!maybe_spawn.has_value()) {
				return false;
			}

			const offset_t spawn_position{ maybe_spawn.value() };

			static std::discrete_distribution<usize> offmap_dis{ static_cast<f64>(reinforcement_count<EntityTypes> * 2)... };

			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr entity_e cval{ val };

				using entity_type = typename to_entity_type<cval>::type;

				if constexpr (is_good<entity_type>::value) {
					if (entity_registry<MapType>.dependent add<true>(spawn_position, entity_type{})) {
						--reinforcement_count<entity_type>;

						return true;
					}

					return false;
				}

				return false;
			}, static_cast<entity_e>(static_cast<usize>(entity_e::Adventurer) + offmap_dis(random_engine)));
		}

		template<map_type_e MapType> static inline bool spawn_neutral() noexcept {
			cauto maybe_spawn = find_spawn_position<MapType, object_e::Crevice>();

			if (!maybe_spawn.has_value()) {
				return false;
			}

			const offset_t spawn_position{ maybe_spawn.value() };

			entity_registry<MapType>.dependent add<true>(spawn_position, thetwo_t{});

			return true;
		}

		static inline void terminate_process_turn() noexcept {
			game_running = false;

			while (processing_turn) {};
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

				if (registry_access.try_lock()) {
					unified_map_renderer_s<MapType>::draw(game_atlas, entity_registry<MapType>, object_registry<MapType>, camera<MapType>, offset_t::Zero, globals::grid_origin<grid_type_e::Game>());

					registry_access.unlock();
				} else if (buffer_access.try_lock()) {
					unified_map_renderer_s<MapType>::draw(game_atlas, entity_buffer<MapType>, object_buffer<MapType>, camera<MapType>, offset_t::Zero, globals::grid_origin<grid_type_e::Game>());

					buffer_access.unlock();
				}
			}

			ui_registry.render<MapType>();

			renderer.present();
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
