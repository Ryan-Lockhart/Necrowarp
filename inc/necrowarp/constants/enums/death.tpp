#pragma once

#include <necrowarp/constants/enums/death.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(death_e death) noexcept {
		switch (death) {
			case death_e::Killed: {
				return "killed";
			} case death_e::Devoured: {
				return "devoured";
			} case death_e::Crushed: {
				return "crushed";
			} case death_e::Eradicated: {
				return "eradicated";
			}
		}
	}

	constexpr color_t to_color(death_e death) noexcept {
		switch (death) {
			case death_e::Killed: {
				return colors::Red;
			} case death_e::Devoured: {
				return colors::Orange;
			} case death_e::Crushed: {
				return colors::dark::Yellow;
			} case death_e::Eradicated: {
				return colors::dark::Magenta;
			}
		}
	}

	constexpr runes_t to_colored_string(death_e death) noexcept { return runes_t{ to_string(death), to_color(death) }; }
} // namespace necrowarp
