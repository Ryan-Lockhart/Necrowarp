#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <optional>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

#include <necrowarp/constants/enums/fluid.hpp>

namespace necrowarp {
	enum struct bloomage_e : u8 {
		SanguineFlower,
		SanguineFern,
		SanguineMoss,
		SanguineMushroom,

		SpectralFlower,
		SpectralFern,
		SpectralMoss,
		SpectralMushroom,

		WretchedFlower,
		WretchedFern,
		WretchedMoss,
		WretchedMushroom,

		InfernalFlower,
		InfernalFern,
		InfernalMoss,
		InfernalMushroom,
	};

	static constexpr cstr to_string(bloomage_e bloomage) noexcept;

	static constexpr color_t to_color(bloomage_e bloomage) noexcept;

	static constexpr runes_t to_colored_string(bloomage_e bloomage) noexcept;

	template<RandomEngine Generator> static constexpr std::optional<bloomage_e> random_bloom(fluid_e fluid) noexcept;
} // namespace necrowarp
