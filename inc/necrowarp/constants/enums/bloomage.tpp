#pragma once

#include <necrowarp/constants/enums/bloomage.hpp>

#include <necrowarp/globals.hpp>

#include <random>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>
#include <necrowarp/constants/enums/fluid.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(bloomage_e bloomage) noexcept {
		switch (bloomage) {
			case bloomage_e::SanguineFlower: {
				return "sanguine flowers";
			} case bloomage_e::SanguineFern: {
				return "sanguine fern";
			} case bloomage_e::SanguineMoss: {
				return "sanguine moss";
			} case bloomage_e::SanguineMushroom: {
				return "sanguine mushroom";
			} case bloomage_e::SpectralFlower: {
				return "spectral flowers";
			} case bloomage_e::SpectralFern: {
				return "spectral fern";
			} case bloomage_e::SpectralMoss: {
				return "spectral moss";
			} case bloomage_e::SpectralMushroom: {
				return "spectral mushroom";
			} case bloomage_e::WretchedFlower: {
				return "wretched flowers";
			} case bloomage_e::WretchedFern: {
				return "wretched fern";
			} case bloomage_e::WretchedMoss: {
				return "wretched moss";
			} case bloomage_e::WretchedMushroom: {
				return "wretched mushroom";
			} case bloomage_e::InfernalFlower: {
				return "infernal flowers";
			} case bloomage_e::InfernalFern: {
				return "infernal fern";
			} case bloomage_e::InfernalMoss: {
				return "infernal moss";
			} case bloomage_e::InfernalMushroom: {
				return "infernal mushroom";
			}
		}
	}

	constexpr color_t to_color(bloomage_e bloomage) noexcept {
		switch (bloomage) {
			case bloomage_e::SanguineFlower: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::light::Red);
			} case bloomage_e::SanguineFern: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::Green);
			} case bloomage_e::SanguineMoss: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::dark::Green);
			} case bloomage_e::SanguineMushroom: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::dark::Orange);
			} case bloomage_e::SpectralFlower: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::light::Red);
			} case bloomage_e::SpectralFern: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::Green);
			} case bloomage_e::SpectralMoss: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::dark::Green);
			} case bloomage_e::SpectralMushroom: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::dark::Orange);
			} case bloomage_e::WretchedFlower: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::light::Red);
			} case bloomage_e::WretchedFern: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::Green);
			} case bloomage_e::WretchedMoss: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::dark::Green);
			} case bloomage_e::WretchedMushroom: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::dark::Orange);
			} case bloomage_e::InfernalFlower: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::light::Red);
			} case bloomage_e::InfernalFern: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::Green);
			} case bloomage_e::InfernalMoss: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::dark::Green);
			} case bloomage_e::InfernalMushroom: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::dark::Orange);
			}
		}
	}

	constexpr runes_t to_colored_string(bloomage_e bloomage) noexcept { return runes_t{ to_string(bloomage), to_color(bloomage) }; }

	template<RandomEngine Generator> static constexpr std::optional<bloomage_e> random_bloom(ref<Generator> engine, fluid_e fluid) noexcept {
		if (fluid == fluid_e::None) {
			return std::nullopt;
		}

		const f32 contains_blood{ static_cast<f32>(contains(fluid, fluid_e::Blood)) };
		const f32 contains_ichor{ static_cast<f32>(contains(fluid, fluid_e::Ichor)) };
		const f32 contains_filth{ static_cast<f32>(contains(fluid, fluid_e::Filth)) };
		const f32 contains_ectoplasm{ static_cast<f32>(contains(fluid, fluid_e::Ectoplasm)) };

		return static_cast<bloomage_e>(
			std::discrete_distribution<u16>{
				contains_blood * globals::FlowerDistribution, contains_blood * globals::FernDistribution, contains_blood * globals::MossDistribution, contains_blood * globals::MushroomDistribution,
				contains_ichor * globals::FlowerDistribution, contains_ichor * globals::FernDistribution, contains_ichor * globals::MossDistribution, contains_ichor * globals::MushroomDistribution,
				contains_filth * globals::FlowerDistribution, contains_filth * globals::FernDistribution, contains_filth * globals::MossDistribution, contains_filth * globals::MushroomDistribution,
				contains_ectoplasm * globals::FlowerDistribution, contains_ectoplasm * globals::FernDistribution, contains_ectoplasm * globals::MossDistribution, contains_ectoplasm * globals::MushroomDistribution,
			}(engine)
		);
	}
} // namespace necrowarp
