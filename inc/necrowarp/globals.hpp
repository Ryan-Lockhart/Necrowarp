#pragma once

#include "bleak/extent.hpp"
#include <bleak.hpp>

namespace necrowarp {
	namespace globals {
		using namespace bleak;

		constexpr bool IsReleaseBuild{
#if defined (BLEAK_DEBUG)
			false
#elif defined(BLEAK_RELEASE)
			true
#else
	#error project error: a debug or release symbol must be defined...
#endif
		};

		constexpr std::string GameName{ "Necrowarp" };
		constexpr std::string GameVersion{ "0.0.1" };
		constexpr std::string GameAuthor{ "Ryan Lockhart" };

		const std::string GameTitle{ std::format("{} v{} by {}", GameName, GameVersion, GameAuthor) };

		constexpr sdl::window_flags WindowFlags{ static_cast<sdl::window_flags>(SDL_WINDOW_SHOWN) };
		constexpr sdl::renderer_flags RendererFlags{ static_cast<sdl::renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE) };

		static inline bool use_frame_limit{ true };

		static inline u32 frame_limit{ 60u };
		static inline f32 frame_time() { return 1000.0f / frame_limit; }
		
		static constexpr extent_t MinimumWindowSize{ 640, 480 }; // VGA
		static constexpr extent_t MaximumWindowSize{ 7680, 4320 }; //  8K

		static inline constexpr bool is_valid_window_size(extent_t size) noexcept {
			return !between<extent_t::scalar_t>(size.w, MinimumWindowSize.w, MaximumWindowSize.w) || !between<extent_t::scalar_t>(size.h, MinimumWindowSize.h, MaximumWindowSize.h);
		}

		enum class resolution_e : u8 {
			// 4:3 resolutions

			Resolution640x480,
			ResolutionVGA = Resolution640x480,
			
			Resolution800x600,
			ResolutionSVGA = Resolution800x600,
			
			Resolution1024x768,
			ResolutionXGA = Resolution1024x768,

			Resolution2048x1536,
			ResolutionQXGA = Resolution2048x1536,

			// 16:9 resolutions

			Resolution1280x720,
			ResolutionHD = Resolution1280x720,

			Resolution1920x1080,
			ResolutionUHD = Resolution1920x1080,

			Resolution2560x1440,
			ResolutionQHD = Resolution2560x1440,

			Resolution3840x2160,
			Resolution4KHD = Resolution3840x2160,

			Resolution7680x4320,
			Resolution8KHD = Resolution7680x4320,

			// 16:10 resolutions

			Resolution1280x800,
			ResolutionSteamDeck = Resolution1280x800,

			// Ultra wide 16:9 resolutions

			Resolution3840x1080,
			ResolutionUHDUW = Resolution3840x1080,

			Resolution5120x1440,
			ResolutionQHDUW = Resolution5120x1440,

			Resolution7680x2160,
			Resolution4KHDUW = Resolution7680x2160,

			FirstResolution = Resolution640x480,
			LastResolution = Resolution7680x2160
		};

		static constexpr usize ResolutionCount{ static_cast<usize>(resolution_e::Resolution7680x2160) + 1ULL };

		static constexpr lut_t<resolution_e, extent_t, ResolutionCount> Resolutions{
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution640x480, extent_t{ 640, 480 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution800x600, extent_t{ 800, 600 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1024x768, extent_t{ 1024, 768 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution2048x1536, extent_t{ 2048, 1536 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1280x720, extent_t{ 1280, 720 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1920x1080, extent_t{ 1920, 1080 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution2560x1440, extent_t{ 2560, 1440 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution3840x2160, extent_t{ 3840, 2160 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution7680x4320, extent_t{ 7680, 4320 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1280x800, extent_t{ 1280, 800 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution3840x1080, extent_t{ 3840, 1080 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution5120x1440, extent_t{ 5120, 1440 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution7680x2160, extent_t{ 7680, 2160 } }
		};
		
		static inline extent_t window_size{ 1280, 720 };

		static constexpr extent_t GlyphSize{ 8, 8 };
		static constexpr extent_t CellSize{ 16, 16 };
		static constexpr extent_t IconSize{ 32, 32 };

		static inline extent_t ui_grid_size() { return window_size / GlyphSize; }
		static inline extent_t game_grid_size() { return window_size / CellSize; }
		static inline extent_t icon_grid_size() { return window_size / IconSize; }

		static constexpr extent_t GlyphsetSize{ 16, 16 };
		static constexpr extent_t TilesetSize{ 16, 5 };
		static constexpr extent_t IconsetSize{ 3, 2 };

		static constexpr extent_t MapSize{ 128, 128 };

		static inline rect_t map_bounds() {
			const extent_t excess{ max<offset_t::scalar_t>(game_grid_size().w - MapSize.w, 0), max<offset_t::scalar_t>(game_grid_size().h - MapSize.h, 0) };

			return rect_t{ offset_t{ excess.w / 2, excess.h / 2 }, extent_t{ game_grid_size().w - excess.w - 1, game_grid_size().h - excess.h - 1 } };
		}

		static constexpr offset_t MapCenter{ MapSize / 2 };

		static constexpr extent_t BorderSize{ 4, 4 };

		static constexpr f32 CellToGlyphRatio{ 2.0f };
		static constexpr f32 GlyphToCellRatio{ 0.5f };

		static inline offset_t convert_cell_to_glyph(cref<offset_t> position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * CellToGlyphRatio), offset_t::scalar_cast(position.y * CellToGlyphRatio) };
		}

		static inline offset_t convert_glyph_to_cell(cref<offset_t> position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * GlyphToCellRatio), offset_t::scalar_cast(position.y * GlyphToCellRatio) };
		}

		constexpr offset_t CursorOffset{ CellSize / 4 };

		static inline extent_t camera_extent() { return MapSize - globals::game_grid_size(); }
		static inline offset_t::scalar_t camera_speed{ 4 };

		struct map_config_t {
			f64 fill_percent;
			u32 automata_iterations;
			u32 automata_threshold;

			u32 view_distance;
			f64 view_span;

			i16 number_of_up_ladders;
			i16 number_of_down_ladders;
			i16 minimum_ladder_distance;

			i16 starting_adventurers;

			i16 starting_skulls;
			i16 minimum_skull_distance;
		};
		
		constexpr map_config_t CavernPreset{
			0.475,
			512,
			4,
			8,
			135.0,
			8,
			4,
			16,
			8,
			16,
			16
		};
		
		constexpr map_config_t TunnelsPreset{
			0.5,
			512,
			4,
			8,
			135.0,
			4,
			2,
			8,
			4,
			8,
			8
		};

		static inline map_config_t map_config{ CavernPreset };

		constexpr i16 SpawnDistributionLow{ 0 };
		constexpr i16 SpawnDistributionHigh{ 99 };

		constexpr i16 SmallWaveSize{ 8 };
		constexpr i16 MediumWaveSize{ 16 };
		constexpr i16 LargeWaveSize{ 32 };
		constexpr i16 HugeWaveSize{ 64 };

		constexpr i16 MinimumWaveSize{ 4 };
		constexpr i16 MaximumWaveSize{ 64 };

		constexpr i16 KillsPerPopulation{ 8 };

		constexpr i16 KillsPerEnergySlot{ 16 };
		constexpr i16 KillsPerArmorSlot{ 8 };
		constexpr i16 KillsPerDivinityTurn{ 64 };

		constexpr i16 MinimumReinforcements{ 0 };
		constexpr i16 MaximumReinforcements{ 4 };

		constexpr i16 FloorsPerReinforcement{ 8 };

		constexpr bool CheatsAllowed{ false };

		constexpr f32 BloodPoolMinimumVolume{ 4.5f };
		constexpr f32 BloodPoolMaximumVolume{ 5.7f };

		static inline std::uniform_real_distribution<f32> blood_pool_dis{ BloodPoolMinimumVolume, BloodPoolMaximumVolume };
	} // namespace globals
} // namespace necrowarp
