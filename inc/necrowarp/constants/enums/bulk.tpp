#pragma once

#include <necrowarp/constants/enums/bulk.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	static constexpr bulk_e grow(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return bulk_e::Young;
			} case bulk_e::Young: {
				return bulk_e::Mature;
			} case bulk_e::Mature: {
				return bulk_e::Bulky;
			} case bulk_e::Bulky: {
				return bulk_e::Gross;
			} case bulk_e::Gross: {
				return bulk_e::Titanic;
			} case bulk_e::Titanic: {
				return bulk_e::Titanic;
			}
		}
	}

	constexpr cstr to_string(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return "neonatal";
			} case bulk_e::Young: {
				return "young";
			} case bulk_e::Mature: {
				return "mature";
			} case bulk_e::Bulky: {
				return "bulky";
			} case bulk_e::Gross: {
				return "gross";
			} case bulk_e::Titanic: {
				return "titanic";
			}
		}
	}

	constexpr color_t to_color(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return colors::light::Magenta;
			} case bulk_e::Young: {
				return colors::light::Blue;
			} case bulk_e::Mature: {
				return colors::light::Green;
			} case bulk_e::Bulky: {
				return colors::light::Yellow;
			} case bulk_e::Gross: {
				return colors::light::Orange;
			} case bulk_e::Titanic: {
				return colors::light::Red;
			}
		}
	}

	constexpr runes_t to_colored_string(bulk_e bulk) noexcept { return runes_t{ to_string(bulk), to_color(bulk) }; }
} // namespace necrowarp
