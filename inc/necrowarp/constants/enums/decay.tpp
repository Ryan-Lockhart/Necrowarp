#pragma once

#include <necrowarp/constants/enums/decay.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	constexpr decay_e decay(decay_e state) noexcept {
		switch (state) {
			case decay_e::Fresh: {
				return decay_e::Animate;
			} case decay_e::Animate: {
				return decay_e::Rotted;
			} case decay_e::Rotted: {
				return decay_e::Rotted;
			}
		}
	}

	constexpr cstr to_string(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return "rotted";
			} case decay_e::Animate: {
				return "animate";
			} case decay_e::Fresh: {
				return "fresh";
			}
		}
	}

	constexpr color_t to_color(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return colors::light::Red;
			} case decay_e::Animate: {
				return colors::light::Magenta;
			} case decay_e::Fresh: {
				return colors::light::Green;
			}
		}
	}

	constexpr runes_t to_colored_string(decay_e decay) noexcept { return runes_t{ to_string(decay), to_color(decay) }; }
} // namespace necrowarp
