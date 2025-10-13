#pragma once

#include <necrowarp/constants/enums/bloomage.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(bloomage_e bloomage) noexcept {
		switch (bloomage) {
			case bloomage_e::SanguineBush: {
				return "sanguine bush";
			} case bloomage_e::SanguineFlowers: {
				return "sanguine flowers";
			} case bloomage_e::SanguineFern: {
				return "sanguine fern";
			} case bloomage_e::SanguineMoss: {
				return "sanguine moss";
			} case bloomage_e::SpectralBush: {
				return "spectral bush";
			} case bloomage_e::SpectralFlowers: {
				return "spectral flowers";
			} case bloomage_e::SpectralFern: {
				return "spectral fern";
			} case bloomage_e::SpectralMoss: {
				return "spectral moss";
			} case bloomage_e::WretchedBush: {
				return "wretched bush";
			} case bloomage_e::WretchedFlowers: {
				return "wretched flowers";
			} case bloomage_e::WretchedFern: {
				return "wretched fern";
			} case bloomage_e::WretchedMoss: {
				return "wretched moss";
			} case bloomage_e::InfernalBush: {
				return "infernal bush";
			} case bloomage_e::InfernalFlowers: {
				return "infernal flowers";
			} case bloomage_e::InfernalFern: {
				return "infernal fern";
			} case bloomage_e::InfernalMoss: {
				return "infernal moss";
			}
		}
	}

	constexpr color_t to_color(bloomage_e bloomage) noexcept {
		switch (bloomage) {
			case bloomage_e::SanguineBush: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::light::Green);
			} case bloomage_e::SanguineFlowers: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::light::Red);
			} case bloomage_e::SanguineFern: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::Green);
			} case bloomage_e::SanguineMoss: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::dark::Green);
			} case bloomage_e::SpectralBush: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::light::Green);
			} case bloomage_e::SpectralFlowers: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::light::Red);
			} case bloomage_e::SpectralFern: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::Green);
			} case bloomage_e::SpectralMoss: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::dark::Green);
			} case bloomage_e::WretchedBush: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::light::Green);
			} case bloomage_e::WretchedFlowers: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::light::Red);
			} case bloomage_e::WretchedFern: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::Green);
			} case bloomage_e::WretchedMoss: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::dark::Green);
			} case bloomage_e::InfernalBush: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::light::Green);
			} case bloomage_e::InfernalFlowers: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::light::Red);
			} case bloomage_e::InfernalFern: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::Green);
			} case bloomage_e::InfernalMoss: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::dark::Green);
			}
		}
	}

	constexpr runes_t to_colored_string(bloomage_e bloomage) noexcept { return runes_t{ to_string(bloomage), to_color(bloomage) }; }
} // namespace necrowarp
