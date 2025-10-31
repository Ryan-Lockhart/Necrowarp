#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>
#include <bleak/concepts.hpp>
#include <bleak/random.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct affliction_e : i8 {
		Stable = 0,

		Resolute = Stable + 1,
		Dauntless = Resolute + 1,
		Unbreakable = Dauntless + 1,

		Wavering = Stable - 1,
		Stricken = Wavering - 1,
		Terrified = Stricken - 1
	};

	static constexpr cstr to_string(affliction_e affliction) noexcept;

	static constexpr color_t to_color(affliction_e affliction) noexcept;

	static constexpr runes_t to_colored_string(affliction_e affliction) noexcept;

	template<affliction_e Affliction> const constexpr u8 shatter_threshold{};

	template<> inline constexpr u8 shatter_threshold<affliction_e::Unbreakable>{ 0 };
	template<> inline constexpr u8 shatter_threshold<affliction_e::Dauntless>{ 2 };
	template<> inline constexpr u8 shatter_threshold<affliction_e::Resolute>{ 8 };

	template<> inline constexpr u8 shatter_threshold<affliction_e::Stable>{ 16 };

	template<> inline constexpr u8 shatter_threshold<affliction_e::Wavering>{ 32 };
	template<> inline constexpr u8 shatter_threshold<affliction_e::Stricken>{ 48 };
	template<> inline constexpr u8 shatter_threshold<affliction_e::Terrified>{ 64 };

	static constexpr u8 ShatterDistributionMinimum{ 0 };
	static constexpr u8 ShatterDistributionMaximum{ 100 };

	static inline std::uniform_int_distribution<u16> shatter_dis{ ShatterDistributionMinimum, ShatterDistributionMaximum };

	template<RandomEngine Generator> static inline bool shatter(ref<Generator> engine, affliction_e affliction, f32 multiplier = 1.0f) noexcept {
		const u8 chance{ static_cast<u8>(shatter_dis(engine)) };
		
		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr affliction_e cval{ val };

			const u8 threshold{
				clamp<u8>(
					static_cast<f32>(shatter_threshold<cval>) * multiplier,
					ShatterDistributionMinimum, ShatterDistributionMaximum
				)
			};

			return chance < threshold;
		}, affliction);
	}
} // namespace necrowarp
