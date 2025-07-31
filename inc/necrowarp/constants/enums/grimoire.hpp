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

	static constexpr cstr to_string(grimoire_e grimoire) noexcept;

	static constexpr color_t to_color(grimoire_e grimoire) noexcept;

	static constexpr runes_t to_colored_string(grimoire_e grimoire) noexcept;
} // namespace necrowarp
