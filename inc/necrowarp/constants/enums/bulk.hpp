#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct bulk_e : u8 {
		Neonatal,
		Young,
		Mature,
		Bulky,
		Gross,
		Titanic
	};

	static constexpr bulk_e grow(bulk_e bulk) noexcept;

	static constexpr cstr to_string(bulk_e bulk) noexcept;

	static constexpr color_t to_color(bulk_e bulk) noexcept;

	static constexpr runes_t to_colored_string(bulk_e bulk) noexcept;
} // namespace necrowarp
