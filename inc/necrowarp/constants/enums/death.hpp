#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct death_e : u8 {
		Killed,
		Devoured,
		Crushed,
		Eradicated
	};

	static constexpr cstr to_string(death_e death) noexcept;

	static constexpr color_t to_color(death_e death) noexcept;

	static constexpr runes_t to_colored_string(death_e death) noexcept;
} // namespace necrowarp
