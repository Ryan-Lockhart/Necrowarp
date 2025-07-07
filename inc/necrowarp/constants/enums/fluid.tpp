#pragma once

#include <necrowarp/constants/enums/fluid.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr fluid_e operator+(fluid_e lhs, fluid_e rhs) noexcept { return static_cast<fluid_e>(static_cast<u8>(lhs) | static_cast<u8>(rhs)); }

	constexpr ref<fluid_e> operator+=(ref<fluid_e> lhs, fluid_e rhs) noexcept { return lhs = lhs + rhs, lhs; }

	constexpr fluid_e operator-(fluid_e lhs, fluid_e rhs) noexcept { return static_cast<fluid_e>(static_cast<u8>(lhs) & ~static_cast<u8>(rhs)); }

	constexpr ref<fluid_e> operator-=(ref<fluid_e> lhs, fluid_e rhs) noexcept { return lhs = lhs - rhs, lhs; }

	constexpr fluid_e operator%(fluid_e lhs, fluid_e rhs) noexcept { return static_cast<fluid_e>(static_cast<u8>(lhs) ^ static_cast<u8>(rhs)); }

	constexpr ref<fluid_e> operator%=(ref<fluid_e> lhs, fluid_e rhs) noexcept { return lhs = lhs % rhs, lhs; }

	constexpr cstr to_string(fluid_e fluid) noexcept {
		switch (fluid) {
			case fluid_e::None: {
				return "none";
			} case fluid_e::Blood: {
				return "blood";
			} case fluid_e::Ichor: {
				return "ichor";
			} case fluid_e::BloodyIchor: {
				return "bloody ichor";
			} case fluid_e::Filth: {
				return "filth";
			} case fluid_e::BloodyFilth: {
				return "bloody filth";
			} case fluid_e::IchorousFilth: {
				return "ichorous filth";
			} case fluid_e::BloodyIchorousFilth: {
				return "bloody, ichorous filth";
			} case fluid_e::Ectoplasm: {
				return "ectoplasm";
			} case fluid_e::BloodyEctoplasm: {
				return "bloody ectoplasm";
			} case fluid_e::IchorousEctoplasm: {
				return "ichorous ectoplasm";
			} case fluid_e::FilthyEctoplasm: {
				return "filthy ectoplasm";
			} case fluid_e::BloodyIchorousEctoplasm: {
				return "bloody, ichorous ectoplasm";
			} case fluid_e::BloodyFilthyEctoplasm: {
				return "bloody, filthy ectoplasm";
			} case fluid_e::IchorousFilthyEctoplasm: {
				return "ichorous, filthy ectoplasm";
			} case fluid_e::BloodyIchorousFilthyEctoplasm: {
				return "bloody, ichorous, filthy ectoplasm";
			}
		}
	}

	constexpr color_t to_color(fluid_e fluid) noexcept {
		return magic_enum::enum_switch([](auto val) -> color_t {
			constexpr fluid_e cval{ val };

			return colors::materials::Fluids<cval>;
		}, fluid);
	}

	constexpr runes_t to_colored_string(fluid_e fluid) noexcept {
		const cstr string{ to_string(fluid) };

		switch (fluid) {
			case fluid_e::None: {
				return runes_t{ string, colors::White };
			} default: {
				return runes_t{ string, to_color(fluid) };
			}
		}
	}
}