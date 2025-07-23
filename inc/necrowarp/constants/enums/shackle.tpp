#pragma once

#include <necrowarp/constants/enums/shackle.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(shackle_e type) noexcept {
		switch (type) {
			case shackle_e::Unshackled: {
				return "unshackled";
			} case shackle_e::Calcitic: {
				return "humerus shackle";
			} case shackle_e::Spectral: {
				return "haunted shackle";
			} case shackle_e::Sanguine: {
				return "bloody shackle";
			} case shackle_e::Galvanic: {
				return "glimmering shackle";
			} case shackle_e::Ravenous: {
				return "wriggling shackle";
			} case shackle_e::Wretched: {
				return "festering shackle";
			} case shackle_e::Cerebral: {
				return "entrancing shackle";
			} case shackle_e::Infernal: {
				return "cackling shackle";
			}
		}
	}

	constexpr color_t to_color(shackle_e shackle) noexcept {
		return magic_enum::enum_switch([](auto val) -> color_t {
			constexpr shackle_e cval{ val };

			return colors::metals::Shackles<cval>;
		}, shackle);
	}

	constexpr runes_t to_colored_string(shackle_e shackle) noexcept {
		return runes_t{ to_string(shackle), to_color(shackle) };
	}
}