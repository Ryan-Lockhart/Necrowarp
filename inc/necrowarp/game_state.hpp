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

#if defined(BLEAK_RELEASE)
	static inline subsystem_s subsystem{ api_state.app_id };
#elif defined(BLEAK_DEBUG)
	static inline subsystem_s subsystem{};
#endif

	static inline phase_t phase{};

	static inline window_t window{ globals::GameName, globals::window_size, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res/gfx/glyphs/glyphs_8x8.png" };

	static inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res/gfx/tiles/static_64x64.png" };
	static inline atlas_t<globals::AnimatedSize> animated_atlas{ renderer, "res/gfx/tiles/animated_64x64.png" };

	static inline atlas_t<globals::IconsetSize> icon_atlas{ renderer, "res/gfx/icons/icons_32x32.png" };

	static inline texture_t ui_cursor_texture{ renderer, "res/gfx/sprites/cursor.png" };
	static inline texture_t grid_cursor_texture{ renderer, "res/gfx/sprites/grid_cursor.png" };
	
	static inline game_stats_t game_stats{};

	static inline std::bernoulli_distribution coinflip{};

	static inline std::bernoulli_distribution triflip{ 1.0 / 3.0 };

	template<map_type_e MapType> static inline zone_t<map_cell_t, globals::MapSize<MapType>, globals::BorderSize<MapType>> game_map{};

	static inline std::vector<offset_t> ladder_positions{};

	template<map_type_e MapType> static inline zone_t<fluid_cell_t, globals::MapSize<MapType>, globals::BorderSize<MapType>> fluid_map{};

	static inline cursor_t ui_cursor{};

	template<map_type_e MapType> static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> grid_cursor{ colors::metals::Gold, game_map<MapType>.zone_origin, game_map<MapType>.zone_extent };
	template<map_type_e MapType> static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor{ colors::Magenta, game_map<MapType>.zone_origin, game_map<MapType>.zone_extent };

	static inline bool draw_cursor{ true };
	static inline bool draw_warp_cursor{ false };

	template<map_type_e MapType> static inline camera_t camera{ globals::grid_size<grid_type_e::Game>(), extent_t::Zero, globals::camera_extent<MapType>() };

	static inline bool camera_locked{ true };

	template<map_type_e MapType> static inline bool camera_forced() noexcept { return globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w || globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h; }
	
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
	
	static inline volatile std::atomic<bool> player_acted{ false };
	static inline volatile std::atomic<bool> processing_turn{ false };
	static inline volatile std::atomic<bool> buffers_locked{ false };

	static inline volatile std::atomic<bool> game_running{ false };

	static inline volatile std::atomic<dimension_e> current_dimension{ dimension_e::Underworld };

	template<map_type_e MapType> inline bool spill_fluid(offset_t position, fluid_e fluid) noexcept {
		if (!game_map<MapType>.template within<zone_region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open) {
			return false;
		}

		if (fluid_map<MapType>[position] != fluid) {
			fluid_map<MapType>[position] += fluid;

			return true;
		}

		std::queue<creeper_t<offset_t::product_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			const creeper_t<offset_t::product_t> current{ frontier.front() };
			frontier.pop();

			visited.insert(current.position);

			if (fluid_map<MapType>[current.position] != fluid) {
				fluid_map<MapType>[current.position] += fluid;

				return true;
			}

			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !game_map<MapType>.template within<zone_region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
					continue;
				}

				frontier.emplace(offset_position, offset_t::product_t{ current.distance + 1 });					
			}
		}

		return false;
	}

	static inline f32 fluid_pool_volume() noexcept { return globals::fluid_pool_dis(random_engine); }

	static inline f32 fluid_pool_volume(i8 count) noexcept { 
		f32 amount{};

		for (i8 i{ 0 }; i < count; ++i) {
			amount += globals::fluid_pool_dis(random_engine);
		}

		return amount;
	}
} // namespace necrowarp
