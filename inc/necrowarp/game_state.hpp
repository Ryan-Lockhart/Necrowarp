#pragma once

#include <bleak.hpp>

#include <atomic>
#include <random>
#include <vector>

#include <necrowarp/cell.hpp>
#include <necrowarp/globals.hpp>
#include <necrowarp/phase.hpp>
#include <necrowarp/stats.hpp>

#include <magic_enum/magic_enum_utility.hpp>

namespace necrowarp {
	using namespace bleak;

	static inline subsystem_s subsystem{};

	static inline phase_t phase{};

	static inline window_t window{ globals::GameName, globals::window_size, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res/gfx/glyphs/glyphs_8x8.png" };

	static inline atlas_t<globals::TilesetSize> tile_atlas{ renderer, "res/gfx/tiles/tileset_64x64.png" };
	static inline atlas_t<globals::EntitysetSize> entity_atlas{ renderer, "res/gfx/tiles/entities_64x64.png" };

	static inline atlas_t<globals::IconsetSize> icon_atlas{ renderer, "res/gfx/icons/icons_32x32.png" };
	
	static inline game_stats_t game_stats{};

	static inline std::bernoulli_distribution coinflip{};

	static inline std::bernoulli_distribution triflip{ 1.0 / 3.0 };

	static inline zone_ct_t<map_cell_t, globals::MapSize, globals::BorderSize> game_map{};

	static inline std::vector<offset_t> ladder_positions{};

	static inline zone_ct_t<fluid_cell_t, globals::MapSize, globals::BorderSize> fluid_map{};

	static inline cursor_t ui_cursor{ renderer, "res/gfx/sprites/cursor.png", colors::White };

	static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> grid_cursor{ renderer, "res/gfx/sprites/grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };
	static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor{ renderer, "res/gfx/sprites/grid_cursor.png", colors::Magenta, game_map.zone_origin, game_map.zone_extent };

	static inline bool draw_cursor{ true };
	static inline bool draw_warp_cursor{ false };

	static inline camera_t camera{ globals::grid_size<grid_type_e::Game>(), extent_t::Zero, globals::camera_extent() };

	static inline bool camera_locked{ true };

	static inline bool camera_forced() noexcept { return globals::MapSize.w <= globals::grid_size<grid_type_e::Game>().w || globals::MapSize.h <= globals::grid_size<grid_type_e::Game>().h; }
	
	static constexpr usize input_interval{ 125ULL };
	static inline bleak::timer_t input_timer{ input_interval };

	static constexpr usize cursor_interval{ 125ULL };
	static inline bleak::timer_t cursor_timer{ cursor_interval };

	static constexpr usize epoch_interval{ 250ULL };
	static inline bleak::timer_t epoch_timer{ epoch_interval };
	
#if !defined(STEAMLESS)
	static constexpr usize stat_store_interval{ 30000ULL };
	static inline bleak::timer_t stat_store_timer{ stat_store_interval };
#endif

	static inline wave_t sine_wave{ 1.0, 0.5, 1.0 };

	static inline mixer_s mixer{};
	
	static inline volatile std::atomic_bool player_acted{ false };
	static inline volatile std::atomic_bool processing_turn{ false };
	static inline volatile std::atomic_bool buffers_locked{ false };

	static inline volatile std::atomic_bool game_running{ false };

	static inline f32 fluid_pool_volume() noexcept { return globals::fluid_pool_dis(random_engine); }

	static inline f32 fluid_pool_volume(i8 count) noexcept { 
		f32 amount{};

		for (i8 i{ 0 }; i < count; ++i) {
			amount += globals::fluid_pool_dis(random_engine);
		}

		return amount;
	}
} // namespace necrowarp
