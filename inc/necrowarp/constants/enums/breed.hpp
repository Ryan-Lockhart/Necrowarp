#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct breed_e : u8 {
		Scaly,
		Shaggy,
		Lanky
	};

	static constexpr cstr to_string(breed_e breed) noexcept;

	static constexpr color_t to_color(breed_e breed) noexcept;

	static constexpr runes_t to_colored_string(breed_e breed) noexcept;

	static constexpr cstr to_description(breed_e breed) noexcept;
} // namespace necrowarp
