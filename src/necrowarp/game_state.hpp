#pragma once

#include <bleak.hpp>

#include <atomic>
#include <random>

#include <necrowarp/globals.hpp>
#include <necrowarp/phase.hpp>

namespace necrowarp {
	using namespace bleak;

	static constexpr u32 SteamAppID{ 3631430 };

	static inline subsystem_s subsystem{ SteamAppID };

	static inline phase_t phase{};

	static inline window_t window{ globals::GameName.c_str(), globals::window_size, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res/glyphs/glyphs_8x8.png" };
	static inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res/tiles/tileset_16x16.png" };
	static inline atlas_t<globals::IconsetSize> icon_atlas{ renderer, "res/icons/icons_32x32.png" };

	static inline std::mt19937 random_engine{};

	static inline zone_t<cell_state_t, globals::MapSize, globals::BorderSize> game_map{};

	static inline cursor_t cursor{ renderer, "res/sprites/cursor.png", colors::White };

	static inline grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res/sprites/grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };
	static inline grid_cursor_t<globals::CellSize> warp_cursor{ renderer, "res/sprites/grid_cursor.png", colors::Magenta, game_map.zone_origin, game_map.zone_extent };

	static inline bool draw_cursor{ true };
	static inline bool draw_warp_cursor{ false };

	static inline bool gamepad_enabled{ true };

	static inline cptr<gamepad_t> primary_gamepad{ nullptr };

	static inline bool gamepad_active{ false };

	static inline camera_t camera{ globals::game_grid_size(), extent_t::Zero, globals::camera_extent() };

	static inline bool camera_locked{ true };

	static inline bool camera_forced() noexcept { return globals::MapSize.w <= globals::game_grid_size().w || globals::MapSize.h <= globals::game_grid_size().h; }

	static inline bleak::timer_t input_timer{ 125.0 };
	static inline bleak::timer_t cursor_timer{ 125.0 };
	static inline bleak::timer_t epoch_timer{ 250.0 };

	static inline wave_t sine_wave{ 1.0, 0.5, 1.0 };

	static inline mixer_s mixer{};
	
	static inline volatile std::atomic_bool player_acted{ false };
	static inline volatile std::atomic_bool processing_turn{ false };

	static inline volatile std::atomic_bool game_running{ false };

	enum class attribute_e {
		Energy,
		Armor,
		Divinity
	};

	template<attribute_e Type> struct attribute_t {
	  private:
		i16 minimum;
		i16 maximum;

		bool enabled;

	  public:
		bool is_enabled() const noexcept { return enabled; };

		i16 current_minimum() const noexcept { return minimum; };
		i16 current_maximum() const noexcept { return maximum; };

		void enable(i16 min, i16 max) noexcept {
			if (is_enabled()) {
				return;
			}

			minimum = min;
			maximum = max;

			enabled = true;
		};

		void disable() noexcept {
			if (!is_enabled()) {
				return;
			}

			enabled = false;

			minimum = 0;
			maximum = 0;
		};

		void set(i16 min, i16 max) noexcept {
			if (!is_enabled()) {
				return;
			}

			minimum = min;
			maximum = max;
		}
	};

	struct cheats_t {
	  private:
		bool enabled;

	  public:
	  	attribute_t<attribute_e::Energy> energy;
	  	attribute_t<attribute_e::Armor> armor;
	  	attribute_t<attribute_e::Divinity> divinity;

		bool no_hit;
		bool free_costs;
		bool bypass_invocations;

		bool is_enabled() const noexcept { return globals::CheatsAllowed && enabled; }

		void activate() noexcept {
			if (!globals::CheatsAllowed) {
				return;
			}

			enabled = true;
		}

		void deactivate() noexcept {
			enabled = false;
		}
	};

	struct game_stats_t {
		cheats_t cheats{};

		usize game_seed{};

		usize game_depth{ 0 };

		usize wave_size{ static_cast<usize>(globals::map_config.starting_adventurers) };
		usize spawns_remaining{ static_cast<usize>(globals::map_config.starting_adventurers) };

		i16	player_kills{ 0 };
		i16	minion_kills{ 0 };

		i16	total_kills() const noexcept { return player_kills + minion_kills; };

		i16 current_reinforcements() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerReinforcement, globals::MinimumReinforcements, globals::MaximumReinforcements); }
		
		bool has_reinforcements() const noexcept { return current_reinforcements() > 0; }

		void reset() noexcept {
			game_seed = std::random_device{}();
			random_engine.seed(game_seed);

			wave_size = globals::map_config.starting_adventurers;
			spawns_remaining = globals::map_config.starting_adventurers;

			player_kills = 0;
			minion_kills = 0;
		}
	} static inline game_stats{};
} // namespace necrowarp
