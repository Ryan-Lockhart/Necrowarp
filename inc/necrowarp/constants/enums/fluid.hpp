#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/color.hpp>
#include <bleak/text.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct fluid_e : u8 {
		None = 0,

		Blood = 1 << 0,
		Ichor = Blood << 1,

		BloodyIchor = Blood | Ichor,

		Filth = Ichor << 1,

		BloodyFilth = Blood | Filth,
		IchorousFilth = Ichor | Filth,

		BloodyIchorousFilth = Blood | Ichor | Filth,

		Ectoplasm = Filth << 1,

		BloodyEctoplasm = Blood | Ectoplasm,
		IchorousEctoplasm = Ichor | Ectoplasm,
		FilthyEctoplasm = Filth | Ectoplasm,

		BloodyIchorousEctoplasm = Blood | Ichor | Ectoplasm,
		BloodyFilthyEctoplasm = Blood | Filth | Ectoplasm,
		IchorousFilthyEctoplasm = Ichor | Filth | Ectoplasm,

		BloodyIchorousFilthyEctoplasm = Blood | Ichor | Filth | Ectoplasm,
	};

	constexpr fluid_e operator+(fluid_e lhs, fluid_e rhs) noexcept;
	constexpr fluid_e operator-(fluid_e lhs, fluid_e rhs) noexcept;
	constexpr fluid_e operator%(fluid_e lhs, fluid_e rhs) noexcept;

	constexpr ref<fluid_e> operator+=(ref<fluid_e> lhs, fluid_e rhs) noexcept;
	constexpr ref<fluid_e> operator-=(ref<fluid_e> lhs, fluid_e rhs) noexcept;
	constexpr ref<fluid_e> operator%=(ref<fluid_e> lhs, fluid_e rhs) noexcept;

	constexpr cstr to_string(fluid_e fluid) noexcept;

	constexpr color_t to_color(fluid_e fluid) noexcept;

	constexpr runes_t to_colored_string(fluid_e fluid) noexcept;
} // namespace necrowarp
