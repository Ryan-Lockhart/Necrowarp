#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	enum struct bloomage_e : u8 {
		SanguineBush,
		SanguineFlowers,
		SanguineFern,
		SanguineMoss,

		SpectralBush,
		SpectralFlowers,
		SpectralFern,
		SpectralMoss,

		WretchedBush,
		WretchedFlowers,
		WretchedFern,
		WretchedMoss,

		InfernalBush,
		InfernalFlowers,
		InfernalFern,
		InfernalMoss,
	};

	static constexpr cstr to_string(bloomage_e bloomage) noexcept;

	static constexpr color_t to_color(bloomage_e bloomage) noexcept;

	static constexpr runes_t to_colored_string(bloomage_e bloomage) noexcept;
} // namespace necrowarp
