#pragma once

#include <necrowarp/constants/enums/affliction.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	static constexpr cstr to_string(affliction_e affliction) noexcept {
		switch (affliction) {
			case affliction_e::Resolute: {
				return "resolute";
			} case affliction_e::Stable: {
				return "stable";
			} case affliction_e::Wavering: {
				return "wavering";
			} case affliction_e::Stricken: {
				return "stricken";
			} case affliction_e::Terrified: {
				return "terrified";
			}
		}
	}
	
	static constexpr color_t to_color(affliction_e affliction) noexcept {
		switch (affliction) {
			case affliction_e::Resolute: {
				return colors::metals::Gold;
			} case affliction_e::Stable: {
				return colors::Green;
			} case affliction_e::Wavering: {
				return colors::Orange;
			} case affliction_e::Stricken: {
				return colors::Red;
			} case affliction_e::Terrified: {
				return colors::Magenta;
			}
		}
	}
	
	static constexpr runes_t to_colored_string(affliction_e affliction) noexcept {
		return runes_t{ to_string(affliction), to_color(affliction) };
	}
} // namespace necrowarp
