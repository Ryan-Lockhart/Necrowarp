#pragma once

#include <necrowarp/constants/enums/grimoire.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::ChaoticWarp: {
				return "chaotic warp";
			} case grimoire_e::PreciseWarp: {
				return "precise warp";
			} case grimoire_e::Annihilate: {
				return "annihilate";
			} case grimoire_e::Repulse: {
				return "repulse";
			} case grimoire_e::Calcify: {
				return "calcify";
			} case grimoire_e::Incorporealize: {
				return "incorporealize";
			} case grimoire_e::CalciticInvocation: {
				return "calcitic invocation";
			} case grimoire_e::SpectralInvocation: {
				return "spectral invocation";
			} case grimoire_e::SanguineInvocation: {
				return "sanguine invocation";
			} case grimoire_e::GalvanicInvocation: {
				return "galvanic invocation";
			} case grimoire_e::RavenousInvocation: {
				return "ravenous invocation";
			} case grimoire_e::WretchedInvocation: {
				return "wretched invocation";
			} case grimoire_e::CerebralInvocation: {
				return "cerebral invocation";
			} case grimoire_e::InfernalInvocation: {
				return "infernal invocation";
			} case grimoire_e::NecromanticAscendance: {
				return "necromantic ascendance";
			} case grimoire_e::CalamitousRetaliation: {
				return "calamitous retaliation";
			}
		}
	}

	constexpr color_t to_color(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::ChaoticWarp: {
				return colors::dark::Blue;
			} case grimoire_e::PreciseWarp: {
				return colors::Yellow;
			} case grimoire_e::Annihilate: {
				return colors::dark::Green;
			} case grimoire_e::Repulse: {
				return colors::light::Blue;
			} case grimoire_e::Calcify: {
				return colors::Marble;
			} case grimoire_e::Incorporealize: {
				return colors::light::Grey;
			} case grimoire_e::CalciticInvocation: {
				return colors::dark::Marble;
			} case grimoire_e::SpectralInvocation: {
				return colors::materials::Fluids<fluid_e::Ichor>;
			} case grimoire_e::SanguineInvocation: {
				return colors::materials::Fluids<fluid_e::Blood>;
			} case grimoire_e::GalvanicInvocation: {
				return colors::metals::Steel;
			} case grimoire_e::RavenousInvocation: {
				return mix(colors::Red, colors::Orange);
			} case grimoire_e::WretchedInvocation: {
				return colors::materials::Fluids<fluid_e::Filth>;
			} case grimoire_e::CerebralInvocation: {
				return mix(colors::Red, colors::light::Magenta);
			} case grimoire_e::InfernalInvocation: {
				return colors::materials::Fluids<fluid_e::Ectoplasm>;
			} case grimoire_e::NecromanticAscendance: {
				return colors::metals::Gold;
			} case grimoire_e::CalamitousRetaliation: {
				return colors::light::Red;
			}
		}
	}

	constexpr runes_t to_colored_string(grimoire_e grimoire) noexcept { return runes_t{ to_string(grimoire), to_color(grimoire) }; }
} // namespace necrowarp
