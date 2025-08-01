#pragma once

#include <necrowarp/constants/enums/base.hpp>

#include <bleak/text.hpp>
#include <bleak/color.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct grimoire_e : u8 {
		ChaoticWarp,
		PreciseWarp,
		
		Annihilate,
		Repulse,
		Calcify,
		Incorporealize,

		CalciticInvocation,
		SpectralInvocation,
		SanguineInvocation,
		GalvanicInvocation,
		RavenousInvocation,
		WretchedInvocation,
		CerebralInvocation,
		InfernalInvocation,

		NecromanticAscendance,
		CalamitousRetaliation
	};

	static constexpr bool is_invocation(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::CalciticInvocation:
			case grimoire_e::SpectralInvocation:
			case grimoire_e::SanguineInvocation:
			case grimoire_e::GalvanicInvocation:
			case grimoire_e::RavenousInvocation:
			case grimoire_e::WretchedInvocation:
			case grimoire_e::CerebralInvocation:
			case grimoire_e::InfernalInvocation: {
				return true;
			} default: {
				return false;
			}
		}
	}

	static constexpr bool is_spell(grimoire_e grimoire) noexcept { return !is_invocation(grimoire); }

	static constexpr cstr to_string(grimoire_e grimoire) noexcept;

	static constexpr color_t to_color(grimoire_e grimoire) noexcept;

	static constexpr runes_t to_colored_string(grimoire_e grimoire) noexcept;
} // namespace necrowarp
