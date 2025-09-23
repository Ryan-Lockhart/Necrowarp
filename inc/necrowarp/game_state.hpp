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
	static inline subsystem_s subsystem{ api_state_s::app_id };
#elif defined(BLEAK_DEBUG)
	static inline subsystem_s subsystem{};
#endif

	static inline phase_t phase{};

	static inline window_t window{ globals::GameName, globals::window_size, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res/gfx/glyphs/glyphs_8x8.png" };

	static inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res/gfx/tiles/static_64x64.png" };
	static inline atlas_t<globals::AnimatedSize> animated_atlas{ renderer, "res/gfx/tiles/animated_64x64.png" };

	static inline atlas_t<globals::IconsetSize> icon_atlas{ renderer, "res/gfx/icons/icons_64x64.png" };

	static inline texture_t ui_cursor_texture{ renderer, "res/gfx/sprites/cursor.png" };
	static inline texture_t grid_cursor_texture{ renderer, "res/gfx/sprites/grid_cursor.png" };
	
	static inline game_stats_t game_stats{};

	static inline std::bernoulli_distribution coinflip{};

	static inline std::bernoulli_distribution triflip{ 1.0 / 3.0 };

	template<map_type_e MapType> static inline zone_t<map_cell_t, globals::MapSize<MapType>, globals::BorderSize<MapType>> game_map{};

	static inline sparse_t<bool> ladder_positions{};
	static inline sparse_t<fluid_e> fluid_positions{};

	template<map_type_e MapType> static inline zone_t<fluid_cell_t, globals::MapSize<MapType>, globals::BorderSize<MapType>> fluid_map{};

	static inline std::atomic<bool> fluid_map_dirty{ false };

	static inline cursor_t ui_cursor{};

	template<map_type_e MapType> static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> grid_cursor{ colors::metals::Gold, game_map<MapType>.zone_origin, game_map<MapType>.zone_extent };
	template<map_type_e MapType> static inline grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor{ colors::Magenta, game_map<MapType>.zone_origin, game_map<MapType>.zone_extent };

	static inline bool draw_cursor{ true };

	static inline std::optional<offset_t> warped_from{ std::nullopt };

	template<map_type_e MapType> static inline camera_t camera{ globals::grid_size<grid_type_e::Game>(), globals::camera_origin<MapType>(), globals::camera_extent<MapType>() };

	static inline bool camera_locked{ true };

	template<map_type_e MapType> static inline bool camera_forced() noexcept { return globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w || globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h; }
	
	static constexpr usize input_interval{ 125ULL };
	static inline bleak::timer_t input_timer{ input_interval };

	static constexpr usize cursor_interval{ 125ULL };
	static inline bleak::timer_t cursor_timer{ cursor_interval };

	enum struct timestep_e : u8 {
		Slow,
		Normal,
		Fast,
		Faster,
		Ludicrous
	};

	static constexpr bleak::lut_t<timestep_e, usize, magic_enum::enum_count<timestep_e>()> epoch_interval{
		pair_t<timestep_e, usize>{ timestep_e::Slow, 500ULL },
		pair_t<timestep_e, usize>{ timestep_e::Normal, 375ULL },
		pair_t<timestep_e, usize>{ timestep_e::Fast, 250ULL },
		pair_t<timestep_e, usize>{ timestep_e::Faster, 125ULL },
		pair_t<timestep_e, usize>{ timestep_e::Ludicrous, 50ULL }
	};

	static constexpr bleak::lut_t<timestep_e, usize, magic_enum::enum_count<timestep_e>()> rush_interval{
		pair_t<timestep_e, usize>{ timestep_e::Slow, epoch_interval[timestep_e::Slow] * 2 },
		pair_t<timestep_e, usize>{ timestep_e::Normal, epoch_interval[timestep_e::Normal] * 2 },
		pair_t<timestep_e, usize>{ timestep_e::Fast, epoch_interval[timestep_e::Fast] * 2 },
		pair_t<timestep_e, usize>{ timestep_e::Faster, epoch_interval[timestep_e::Faster] * 2 },
		pair_t<timestep_e, usize>{ timestep_e::Ludicrous, epoch_interval[timestep_e::Ludicrous] * 2 }
	};

	static inline timestep_e epoch_timestep{ timestep_e::Normal };

	static inline bleak::timer_t epoch_timer{ epoch_interval[epoch_timestep] };
	static inline bleak::timer_t rush_timer{ rush_interval[epoch_timestep] };

	static inline std::uniform_int_distribution<usize> epoch_interval_dis{ static_cast<usize>(epoch_interval[epoch_timestep] * 0.10), static_cast<usize>(epoch_interval[epoch_timestep] * 0.90) };

	static inline bool hasten_timestep() noexcept {
		if (epoch_timestep == timestep_e::Ludicrous) {
			return false;
		}

		epoch_timestep = static_cast<timestep_e>(static_cast<u8>(epoch_timestep) + 1);

		epoch_timer.interval = epoch_interval[epoch_timestep];
		rush_timer.interval = rush_interval[epoch_timestep];

		epoch_interval_dis = std::uniform_int_distribution<usize>{ static_cast<usize>(epoch_interval[epoch_timestep] * 0.10), static_cast<usize>(epoch_interval[epoch_timestep] * 0.90) };

		return true;
	}

	static inline bool harry_timestep() noexcept {
		if (epoch_timestep == timestep_e::Slow) {
			return false;
		}

		epoch_timestep = static_cast<timestep_e>(static_cast<u8>(epoch_timestep) - 1);

		epoch_timer.interval = epoch_interval[epoch_timestep];
		rush_timer.interval = rush_interval[epoch_timestep];

		epoch_interval_dis = std::uniform_int_distribution<usize>{ static_cast<usize>(epoch_interval[epoch_timestep] * 0.10), static_cast<usize>(epoch_interval[epoch_timestep] * 0.90) };

		return true;
	}

	template<RandomEngine Generator> static inline usize random_epoch_interval(ref<Generator> engine) noexcept { return epoch_interval_dis(engine); }
	
#if !defined(STEAMLESS)
	static constexpr usize stat_store_interval{ 30000ULL };
	static inline bleak::timer_t stat_store_timer{ stat_store_interval };
#endif

	static inline wave_t sine_wave{ 1.0, 0.5, 1.0 };
	static inline wave_t stealth_wave{ 0.125, 0.25, 1.0 };

	static inline mixer_s mixer{};

	static inline priority_mutex registry_access{};

	static inline priority_mutex buffer_access{};
	
	static inline volatile std::atomic<bool> player_acted{ false };

	static inline volatile std::atomic<bool> processing_turn{ false };

	static inline volatile std::atomic<bool> game_running{ false };

	static inline volatile std::atomic<dimension_e> current_dimension{ dimension_e::Underworld };

	template<map_type_e MapType> inline void recalculate_maps() noexcept {
		for (offset_t::scalar_t y{ 0 }; y < globals::MapSize<MapType>.h; ++y) {
			for (offset_t::scalar_t x{ 0 }; x < globals::MapSize<MapType>.w; ++x) {
				const offset_t pos{ x, y };

				game_map<MapType>[pos].recalculate_index(game_map<MapType>, pos, cell_e::Solid);
				fluid_map<MapType>[pos].recalculate_index(fluid_map<MapType>, pos, [](fluid_cell_t value) -> bool { return value != fluid_cell_t{}; });
			}
		}
	}

	template<map_type_e MapType> inline void recalculate_cell_map() noexcept {
		for (cauto position : game_map<MapType>.zone_offsets) {
			game_map<MapType>[position].recalculate_index(game_map<MapType>, position, cell_e::Solid);
		}
	}

	template<map_type_e MapType, bool Force = false> inline void recalculate_fluid_map() noexcept {
		if constexpr (!Force) {
			if (!fluid_map_dirty) {
				return;
			}
		}

		for (cauto position : fluid_map<MapType>.zone_offsets) {
			fluid_map<MapType>[position].recalculate_index(fluid_map<MapType>, position, [](fluid_cell_t value) -> bool { return value != fluid_cell_t{}; });
		}

		fluid_map_dirty = false;
	}

	template<map_type_e MapType> inline bool spill_fluid(offset_t position, fluid_e fluid) noexcept {
		if (fluid == fluid_e::None || !game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open) {
			return false;
		}

		if (fluid_map<MapType>[position] != fluid) {
			fluid_map_dirty = true;

			fluid_map<MapType>[position] += fluid;
			fluid -= static_cast<fluid_e>(fluid_map<MapType>[position]);

			if (fluid == fluid_e::None) {
				return true;
			}
		}

		std::priority_queue<creeper_t<f32>, std::vector<creeper_t<f32>>, creeper_t<f32>::less> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			if (fluid == fluid_e::None) {
				return true;
			}

			const creeper_t<f32> current{ frontier.top() };
			frontier.pop();

			visited.insert(current.position);

			if (fluid_map<MapType>[current.position] != fluid) {
				fluid_map_dirty = true;

				fluid_map<MapType>[current.position] += fluid;
				fluid -= static_cast<fluid_e>(fluid_map<MapType>[current.position]);

				if (fluid == fluid_e::None) {
					return true;
				}
			}

			for (cauto creeper : neighbourhood_creepers<distance_function_e::Octile, f32>) {
				const offset_t offset_position{ current.position + creeper.position };

				if (!visited.insert(offset_position).second || !game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
					continue;
				}

				frontier.emplace(offset_position, f32{ current.distance + creeper.distance });					
			}
		}

		return false;
	}

	template<map_type_e MapType> inline bool spill_fluid(offset_t position, fluid_e fluid, usize amount) noexcept {
		if (fluid == fluid_e::None || !game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open || amount <= 0) {
			return false;
		}

		if (fluid_map<MapType>[position] != fluid) {
			fluid_map_dirty = true;

			fluid_map<MapType>[position] += fluid;

			if (amount <= 1) {
				return true;
			} else {
				--amount;
			}
		}

		std::priority_queue<creeper_t<f32>, std::vector<creeper_t<f32>>, creeper_t<f32>::less> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			if (fluid == fluid_e::None) {
				return true;
			}

			const creeper_t<f32> current{ frontier.top() };
			frontier.pop();

			visited.insert(current.position);

			if (fluid_map<MapType>[current.position] != fluid) {
				fluid_map_dirty = true;

				fluid_map<MapType>[current.position] += fluid;

				if (amount <= 1) {
					return true;
				} else {
					--amount;
				}
			}

			for (cauto creeper : neighbourhood_creepers<distance_function_e::Octile, f32>) {
				const offset_t offset_position{ current.position + creeper.position };

				if (!visited.insert(offset_position).second || !game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
					continue;
				}

				frontier.emplace(offset_position, f32{ current.distance + creeper.distance });					
			}
		}

		return false;
	}

	static inline f16 fluid_pool_volume() noexcept { return globals::fluid_pool_dis(random_engine); }

	static inline f16 fluid_pool_volume(i8 count) noexcept { 
		f16 amount{};

		for (i8 i{ 0 }; i < count; ++i) {
			amount += globals::fluid_pool_dis(random_engine);
		}

		return amount;
	}
} // namespace necrowarp
