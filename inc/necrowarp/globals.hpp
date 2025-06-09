#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct grid_type_e : u8 {
		UI,
		Game,
		Icon
	};

	namespace globals {
		constexpr cstr GameName{ "Necrowarp" };
		constexpr cstr GameVersion{ "0.0.1" };
		constexpr cstr GameAuthor{ "Ryan Lockhart" };

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

		enum struct resolution_e : u8 {
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

			// 8:5 resolutions

			Resolution1280x800,
			ResolutionWXGA = Resolution1280x800,
			ResolutionSteamDeck = Resolution1280x800,

			Resolution1440x900,
			ResolutionWXGAPlus = Resolution1440x900,

			Resolution1680x1050,
			ResolutionWSXGAPlus = Resolution1680x1050,

			Resolution1920x1200,
			ResolutionWUXGA = Resolution1920x1200,

			Resolution2560x1600,
			ResolutionWQXGA = Resolution2560x1600,

			Resolution3840x2400,
			ResolutionWQUXGA = Resolution3840x2400,

			// 32:9 resolutions (Ultra-wide)

			Resolution2560x720,
			ResolutionHDUW = Resolution2560x720,

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
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1440x900, extent_t{ 1440, 900 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1680x1050, extent_t{ 1680, 1050 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution1920x1200, extent_t{ 1920, 1200 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution2560x1600, extent_t{ 2560, 1600 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution3840x2400, extent_t{ 3840, 2400 } },

			pair_t<resolution_e, extent_t>{ resolution_e::Resolution2560x720, extent_t{ 2560, 720 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution3840x1080, extent_t{ 3840, 1080 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution5120x1440, extent_t{ 5120, 1440 } },
			pair_t<resolution_e, extent_t>{ resolution_e::Resolution7680x2160, extent_t{ 7680, 2160 } }
		};

		static constexpr cstr to_string(resolution_e resolution) noexcept {
			switch (resolution) {
				case resolution_e::Resolution640x480: {
					return "4:3 - 640 x 480 (VGA)";
				} case resolution_e::Resolution800x600: {
					return "4:3 - 800 x 600 (SVGA)";
				} case resolution_e::Resolution1024x768: {
					return "4:3 - 1024 x 768 (XGA)";
				} case resolution_e::Resolution2048x1536: {
					return "4:3 - 2048 x 1536 (QXGA)";
				} case resolution_e::Resolution1280x720: {
					return "16:9 - 1280 x 720 (HD)";
				} case resolution_e::Resolution1920x1080: {
					return "16:9 - 1920 x 1080 (UHD)";
				} case resolution_e::Resolution2560x1440: {
					return "16:9 - 2560 x 1440 (QHD)";
				} case resolution_e::Resolution3840x2160: {
					return "16:9 - 3840 x 2160 (4KHD)";
				} case resolution_e::Resolution7680x4320: {
					return "16:9 - 7680 x 4320 (8KHD)";
				} case resolution_e::Resolution1280x800: {
					return "8:5 - 1280 x 800 (WXGA / Steam Deck)";
				} case resolution_e::Resolution1440x900: {
					return "8:5 - 1440 x 900 (WXGA+)";
				} case resolution_e::Resolution1680x1050: {
					return "8:5 - 1680 x 1050 (WSXGA+)";
				} case resolution_e::Resolution1920x1200: {
					return "8:5 - 1920 x 1200 (WUXGA)";
				} case resolution_e::Resolution2560x1600: {
					return "8:5 - 2560 x 1600 (WQXGA)";
				} case resolution_e::Resolution3840x2400: {
					return "8:5 - 3840 x 2400 (WQUXGA)";
				} case resolution_e::Resolution2560x720: {
					return "32:9 - 2560 x 720 (HD-UW)";
				} case resolution_e::Resolution3840x1080: {
					return "32:9 - 3840 x 1080 (UHD-UW)";
				} case resolution_e::Resolution5120x1440: {
					return "32:9 - 5120 x 1440 (QHD-UW)";
				} case resolution_e::Resolution7680x2160: {
					return "32:9 - 7680 x 2160 (4KHD-UW)";
				} default: {
					return "unknown resolution";
				}
			}
		}
		
		static inline extent_t window_size{ 1920, 1080 };

		template<grid_type_e GridType> static constexpr extent_t cell_size{};

		template<> inline constexpr extent_t cell_size<grid_type_e::UI>{ 8, 8 };
		template<> inline constexpr extent_t cell_size<grid_type_e::Game>{ 64, 64 };
		template<> inline constexpr extent_t cell_size<grid_type_e::Icon>{ 32, 32 };

		template<grid_type_e GridType> inline extent_t grid_overflow() noexcept { return window_size % cell_size<GridType>; }

		template<grid_type_e GridType> inline bool is_grid_flush() noexcept { return grid_overflow<GridType>() == extent_t::Zero; }

		template<grid_type_e GridType> inline extent_t grid_size() noexcept { return window_size / cell_size<GridType>; }

		template<grid_type_e GridType> constexpr bool use_grid_offset{ false };

		template<> inline constexpr bool use_grid_offset<grid_type_e::UI>{ false };
		template<> inline constexpr bool use_grid_offset<grid_type_e::Game>{ true };
		template<> inline constexpr bool use_grid_offset<grid_type_e::Icon>{ false };

		template<grid_type_e GridType> inline offset_t grid_origin() noexcept {
			if constexpr (use_grid_offset<GridType>) {
				return offset_t{ grid_overflow<GridType>() / 2 - 1 };
			} else {
				return offset_t::Zero;
			}
		}

		template<grid_type_e GridType> inline offset_t grid_extent() noexcept {
			if constexpr (use_grid_offset<GridType>) {
				return offset_t{ window_size - grid_overflow<GridType>() / 2 - 1 };
			} else {
				return offset_t::Zero;
			}
		}

		static constexpr extent_t GlyphsetSize{ 16, 16 };

		static constexpr extent_t TilesetSize{ 16, 2 };
		static constexpr extent_t EntitysetSize{ 16, 2 };

		static constexpr extent_t IconsetSize{ 2, 4 };

		static constexpr distance_function_e DistanceFunction{ distance_function_e::Octile };

		static constexpr extent_t MapSize{ 128, 128 };

		static constexpr offset_t MapOrigin{ offset_t::Zero };
		static constexpr offset_t MapExtent{ MapOrigin + MapSize - 1 };

		static constexpr rect_t MapBounds{ MapOrigin, MapSize };

		static inline rect_t map_bounds() noexcept {
			const extent_t excess{ max<offset_t::scalar_t>(grid_size<grid_type_e::Game>().w - MapSize.w, 0), max<offset_t::scalar_t>(grid_size<grid_type_e::Game>().h - MapSize.h, 0) };

			return rect_t{ offset_t{ excess.w / 2, excess.h / 2 }, extent_t{ grid_size<grid_type_e::Game>().w - excess.w - 1, grid_size<grid_type_e::Game>().h - excess.h - 1 } };
		}

		static constexpr offset_t MapCenter{ MapSize / 2 };

		static constexpr extent_t BorderSize{ 4, 4 };

		static constexpr f32 CellToGlyphRatio{ static_cast<f32>(cell_size<grid_type_e::Game>.w) / static_cast<f32>(cell_size<grid_type_e::UI>.w) };
		static constexpr f32 GlyphToCellRatio{ static_cast<f32>(cell_size<grid_type_e::UI>.w) / static_cast<f32>(cell_size<grid_type_e::Game>.w) };

		static inline offset_t convert_cell_to_glyph(offset_t position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * CellToGlyphRatio), offset_t::scalar_cast(position.y * CellToGlyphRatio) };
		}

		static inline offset_t convert_glyph_to_cell(offset_t position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * GlyphToCellRatio), offset_t::scalar_cast(position.y * GlyphToCellRatio) };
		}

		static inline extent_t camera_extent() noexcept { return MapSize - globals::grid_size<grid_type_e::Game>(); }
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

			i16 starting_wave;

			i16 starting_skulls;
			i16 minimum_skull_distance;
		};
		
		constexpr map_config_t CavernPreset{
			0.45,
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
			0.55,
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

		constexpr i16 MinimumDepth{ 0 };
		constexpr i16 MaximumDepth{ 999 };

		constexpr f32 MinimumDepthMultiplier{ 1.0f };
		constexpr f32 MaximumDepthMultiplier{ 10.0f };

		constexpr u16 SpawnDistributionLow{ 0 };
		constexpr u16 SpawnDistributionHigh{ 99 };

		static inline std::uniform_int_distribution<u16> spawn_dis{ SpawnDistributionLow, SpawnDistributionHigh };

		constexpr i16 SmallWaveSize{ 8 };
		constexpr i16 MediumWaveSize{ 16 };
		constexpr i16 LargeWaveSize{ 32 };
		constexpr i16 HugeWaveSize{ 64 };
		constexpr i16 MassiveWaveSize{ 128 };

		constexpr i16 MinimumWaveSize{ 4 };
		constexpr i16 MaximumWaveSize{ 128 };

		constexpr f32 MinimumWaveSizeMultiplier{ 0.5f };
		constexpr f32 MaximumWaveSizeMultiplier{ 2.0f };

		constexpr i16 KillsPerPopulation{ 8 };

		constexpr i16 KillsPerEnergySlot{ 16 };
		constexpr i16 KillsPerArmorSlot{ 8 };
		constexpr i16 KillsPerDivinityTurn{ 64 };

		constexpr i16 MinimumReinforcements{ 0 };
		constexpr i16 MaximumReinforcements{ 4 };

		constexpr i16 FloorsPerReinforcement{ 8 };

		constexpr bool CheatsAllowed{ true };

		constexpr f32 FluidPoolMinimumVolume{ 4.5f };
		constexpr f32 FluidPoolMaximumVolume{ 5.7f };

		constexpr i8 MinimumCatalyst{ 4 };
		constexpr i8 MaximumCatalyst{ 9 };

		static inline std::uniform_real_distribution<f32> fluid_pool_dis{ FluidPoolMinimumVolume, FluidPoolMaximumVolume };

		constexpr bool OopsAllAdventurers{ false };
		constexpr bool OopsAllMercenaries{ false };
		constexpr bool OopsAllPaladins{ false };
		constexpr bool OopsAllPriests{ false };

		template<typename T> struct has_unique_descriptor {
			static constexpr bool value = false;
		};

		template<typename T> constexpr bool has_unique_descriptor_v = has_unique_descriptor<T>::value;
	} // namespace globals
} // namespace necrowarp
