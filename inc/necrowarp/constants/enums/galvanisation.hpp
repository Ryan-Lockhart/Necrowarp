#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct galvanisation_e : u8 {
		Twisted = 0,
		Shimmering = Twisted + 1,
		Wriggling = Shimmering + 1,
		Writhing = Wriggling + 1,
	};

	constexpr galvanisation_e galvanise(galvanisation_e state) noexcept;

	constexpr cstr to_string(galvanisation_e state) noexcept;

	constexpr color_t to_color(galvanisation_e state) noexcept;

	constexpr runes_t to_colored_string(galvanisation_e state) noexcept;
} // namespace necrowarp
