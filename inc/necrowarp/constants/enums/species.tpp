#pragma once

#include <necrowarp/constants/enums/species.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(species_e species) noexcept {
		switch (species) {
			case species_e::SanguineTetrapod: {
				return "sanguine tetrapod";
			} case species_e::SanguineHexapod: {
				return "sanguine hexapod";
			} case species_e::SpectralTetrapod: {
				return "spectral tetrapod";
			} case species_e::SpectralHexapod: {
				return "spectral hexapod";
			} case species_e::WretchedTetrapod: {
				return "wretched tetrapod";
			} case species_e::WretchedHexapod: {
				return "wretched hexapod";
			} case species_e::InfernalTetrapod: {
				return "infernal tetrapod";
			} case species_e::InfernalHexapod: {
				return "infernal hexapod";
			}
		}
	}

	constexpr color_t to_color(species_e species) noexcept {
		switch (species) {
			case species_e::SanguineTetrapod: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::light::Blue);
			} case species_e::SanguineHexapod: {
				return mix(colors::materials::Fluids<fluid_e::Blood>, colors::light::Orange);
			} case species_e::SpectralTetrapod: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::light::Blue);
			} case species_e::SpectralHexapod: {
				return mix(colors::materials::Fluids<fluid_e::Ichor>, colors::light::Orange);
			} case species_e::WretchedTetrapod: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::light::Blue);
			} case species_e::WretchedHexapod: {
				return mix(colors::materials::Fluids<fluid_e::Filth>, colors::light::Orange);
			} case species_e::InfernalTetrapod: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::light::Blue);
			} case species_e::InfernalHexapod: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::light::Orange);
			}
		}
	}

	constexpr runes_t to_colored_string(species_e species) noexcept { return runes_t{ to_string(species), to_color(species) }; }
} // namespace necrowarp
