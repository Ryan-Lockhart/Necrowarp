#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>
#include <bleak/concepts.hpp>
#include <bleak/random.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct affliction_e : u8 {
		Resolute,
		Stable,
		Wavering,
		Stricken,
		Terrified,
	};

	static constexpr cstr to_string(affliction_e affliction) noexcept;
	
	static constexpr color_t to_color(affliction_e affliction) noexcept;
	
	static constexpr runes_t to_colored_string(affliction_e affliction) noexcept;

	template<affliction_e Affliction> const constexpr u8 defecation_threshold{};

	template<> inline constexpr u8 defecation_threshold<affliction_e::Resolute>{ 100 };
	template<> inline constexpr u8 defecation_threshold<affliction_e::Stable>{ 90 };
	template<> inline constexpr u8 defecation_threshold<affliction_e::Wavering>{ 70 };
	template<> inline constexpr u8 defecation_threshold<affliction_e::Stricken>{ 30 };
	template<> inline constexpr u8 defecation_threshold<affliction_e::Terrified>{ 10 };

	static constexpr u8 DefecationDistributionMinimum{ 0 };
	static constexpr u8 DefecationDistributionMaximum{ 100 };

	static inline std::uniform_int_distribution<u16> defecate_dis{ DefecationDistributionMinimum, DefecationDistributionMaximum };

	template<RandomEngine Generator> static inline bool defecate(ref<Generator> engine, affliction_e affliction) noexcept {
		const u8 chance{ static_cast<u8>(defecate_dis(engine)) };
		
		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr affliction_e cval{ val };

			return chance > defecation_threshold<cval>;
		}, affliction);
	}
} // namespace necrowarp
