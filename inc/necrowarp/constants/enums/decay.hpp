#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct decay_e : i8 {
		Rotted = -1,
		Animate = 0,
		Fresh = 1,
	};

	constexpr decay_e decay(decay_e state) noexcept;

	constexpr cstr to_string(decay_e decay) noexcept;

	constexpr color_t to_color(decay_e decay) noexcept;

	constexpr runes_t to_colored_string(decay_e decay) noexcept;
} // namespace necrowarp
