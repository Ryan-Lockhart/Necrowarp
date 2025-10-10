#pragma once

#include <necrowarp/constants/enums/breed.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(breed_e breed) noexcept {
		switch (breed) {
			case breed_e::Scaly: {
				return "neonatal";
			} case breed_e::Shaggy: {
				return "young";
			} case breed_e::Lanky: {
				return "mature";
			}
		}
	}

	constexpr color_t to_color(breed_e breed) noexcept {
		switch (breed) {
			case breed_e::Scaly: {
				return colors::dark::Green;
			} case breed_e::Shaggy: {
				return colors::dark::Orange;
			} case breed_e::Lanky: {
				return colors::dark::Grey;
			}
		}
	}

	constexpr runes_t to_colored_string(breed_e breed) noexcept { return runes_t{ to_string(breed), to_color(breed) }; }
} // namespace necrowarp
