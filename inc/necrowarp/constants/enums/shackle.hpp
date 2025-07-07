#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/color.hpp>
#include <bleak/text.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct shackle_e : u8 {
		Unshackled,
		Calcitic,
		Spectral,
		Sanguine,
		Galvanic,
		Ravenous,
		Wretched,
		Infernal,
		Cerebral
	};

	constexpr cstr to_string(shackle_e type) noexcept;

	constexpr color_t to_color(shackle_e type) noexcept;

	constexpr runes_t to_colored_string(shackle_e type) noexcept;
} // namespace necrowarp
