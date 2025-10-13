#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct species_e : u8 {
		SanguineTetrapod,
		SanguineHexapod,

		SpectralTetrapod,
		SpectralHexapod,

		WretchedTetrapod,
		WretchedHexapod,

		InfernalTetrapod,
		InfernalHexapod,
	};

	static constexpr cstr to_string(species_e species) noexcept;

	static constexpr color_t to_color(species_e species) noexcept;

	static constexpr runes_t to_colored_string(species_e species) noexcept;
} // namespace necrowarp
