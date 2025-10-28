#pragma once

#include <necrowarp/constants/enums/demeanor.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(demeanor_e demeanor) noexcept {
		switch (demeanor) {
			case demeanor_e::Aloof: {
				return "aloof";
			} case demeanor_e::Grumpy: {
				return "grumpy";
			} case demeanor_e::Speculative: {
				return "speculative";
			} case demeanor_e::Wrathful: {
				return "wrathful";
			} case demeanor_e::Playful: {
				return "playful";
			} case demeanor_e::Jubilant: {
				return "jubilant";
			}
		}
	}

	constexpr color_t to_color(demeanor_e demeanor) noexcept {
		switch (demeanor) {
			case demeanor_e::Aloof: {
				return colors::light::Blue;
			} case demeanor_e::Grumpy: {
				return colors::light::Orange;
			} case demeanor_e::Speculative: {
				return colors::light::Green;
			} case demeanor_e::Wrathful: {
				return colors::light::Red;
			} case demeanor_e::Playful: {
				return colors::light::Yellow;
			} case demeanor_e::Jubilant: {
				return colors::light::Magenta;
			}
		}
	}

	constexpr runes_t to_colored_string(demeanor_e demeanor) noexcept { return runes_t{ to_string(demeanor), to_color(demeanor) }; }
} // namespace necrowarp
