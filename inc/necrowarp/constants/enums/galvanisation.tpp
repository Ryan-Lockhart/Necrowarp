#pragma once

#include <necrowarp/constants/enums/galvanisation.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr galvanisation_e galvanise(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return galvanisation_e::Shimmering;
			} case galvanisation_e::Shimmering: {
				return galvanisation_e::Wriggling;
			} case galvanisation_e::Wriggling: {
				return galvanisation_e::Writhing;
			} case galvanisation_e::Writhing: {
				return galvanisation_e::Writhing;
			}
		}
	}

	constexpr cstr to_string(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return "twisted";
			} case galvanisation_e::Shimmering: {
				return "shimmering";
			} case galvanisation_e::Wriggling: {
				return "wriggling";
			} case galvanisation_e::Writhing: {
				return "writhing";
			}
		}
	}

	constexpr color_t to_color(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return colors::metals::Tin;
			} case galvanisation_e::Shimmering: {
				return colors::light::Magenta;
			} case galvanisation_e::Wriggling: {
				return colors::light::Orange;
			} case galvanisation_e::Writhing: {
				return colors::light::Green;
			}
		}
	}

	constexpr runes_t to_colored_string(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return runes_t{ to_string(galvanisation_e::Twisted), colors::metals::Tin };
			} case galvanisation_e::Shimmering: {
				return runes_t{ to_string(galvanisation_e::Shimmering), colors::light::Magenta };
			} case galvanisation_e::Wriggling: {
				return runes_t{ to_string(galvanisation_e::Wriggling), colors::light::Orange };
			} case galvanisation_e::Writhing: {
				return runes_t{ to_string(galvanisation_e::Writhing), colors::light::Green };
			}
		}
	}
}