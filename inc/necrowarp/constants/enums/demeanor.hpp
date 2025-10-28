#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct demeanor_e : u8 {
		Aloof,
		Grumpy,
		Speculative,
		Wrathful,
		Playful,
		Jubilant
	};

	static constexpr cstr to_string(demeanor_e demeanor) noexcept;

	static constexpr color_t to_color(demeanor_e demeanor) noexcept;

	static constexpr runes_t to_colored_string(demeanor_e demeanor) noexcept;
} // namespace necrowarp
