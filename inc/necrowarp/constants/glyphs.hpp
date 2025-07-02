#pragma once

#include <bleak/constants/glyphs.hpp>

#include <necrowarp/constants/characters.hpp>
#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace icons {
		using namespace bleak::icons;

		constexpr const glyph_t RandomWarp{ 0x00, colors::White };
		constexpr const glyph_t TargetWarp{ 0x01, colors::White };

		constexpr const glyph_t CalciticInvocation{ 0x02, colors::White };
		constexpr const glyph_t SpectralInvocation{ 0x03, colors::White };
		constexpr const glyph_t SanguineInvocation{ 0x04, colors::White };
		constexpr const glyph_t GalvanicInvocation{ 0x05, colors::White };

		constexpr const glyph_t NecromanticAscendance{ 0x06, colors::White };
	} // namespace icons

	namespace glyphs {
		using namespace bleak::glyphs;

		constexpr const glyph_t Floor{ characters::Floor, colors::Transparent };

		constexpr const glyph_t UnarmoredPlayer{ characters::UnarmoredPlayer, colors::White };
		constexpr const glyph_t ArmoredPlayer{ characters::ArmoredPlayer, colors::White };

		constexpr const glyph_t FreshSkeleton{ characters::FreshSkeleton, colors::White };
		constexpr const glyph_t AnimateSkeleton{ characters::AnimateSkeleton, colors::White };
		constexpr const glyph_t RottedSkeleton{ characters::RottedSkeleton, colors::White };

		constexpr const glyph_t Cultist{ characters::Cultist, colors::White };
		constexpr const glyph_t Bloodhound{ characters::Bloodhound, colors::White };

		constexpr const glyph_t Bonespur{ characters::Bonespur, colors::White };
		constexpr const glyph_t Wraith{ characters::Wraith, colors::White };
		constexpr const glyph_t FleshGolem{ characters::FleshGolem, colors::White };

		constexpr const glyph_t Adventurer{ characters::Adventurer, colors::White };
		constexpr const glyph_t Mercenary{ characters::Mercenary, colors::White };

		constexpr const glyph_t NeonatalThetwo{ characters::NeonatalThetwo, colors::White };
		constexpr const glyph_t YoungThetwo{ characters::YoungThetwo, colors::White };
		constexpr const glyph_t MatureThetwo{ characters::MatureThetwo, colors::White };
		constexpr const glyph_t GrossThetwo{ characters::GrossThetwo, colors::White };
		constexpr const glyph_t TitanicThetwo{ characters::TitanicThetwo, colors::White };

		constexpr const glyph_t Ranger{ characters::Ranger, colors::White };
		constexpr const glyph_t Skulker{ characters::Skulker, colors::White };
		constexpr const glyph_t BattleMonk{ characters::BattleMonk, colors::White };
		constexpr const glyph_t MistLady{ characters::MistLady, colors::White };

		constexpr const glyph_t Berserker{ characters::Berserker, colors::White };
		constexpr const glyph_t BloodyBerserker{ characters::BloodyBerserker, colors::White };
		constexpr const glyph_t IchorousBerserker{ characters::IchorousBerserker, colors::White };
		constexpr const glyph_t BloodyIchorousBerserker{ characters::BloodyIchorousBerserker, colors::White };

		constexpr const glyph_t Paladin{ characters::Paladin, colors::White };

		constexpr const glyph_t FreshSkull{ characters::FreshSkull, colors::White };
		constexpr const glyph_t AnimateSkull{ characters::AnimateSkull, colors::White };
		constexpr const glyph_t RottedSkull{ characters::RottedSkull, colors::White };

		constexpr const glyph_t Flesh{ characters::Flesh, colors::White };

		constexpr const glyph_t HealthyCerebrum{ characters::HealthyCerebrum, colors::White };
		constexpr const glyph_t StrickenCerebrum{ characters::StrickenCerebrum, colors::White };

		constexpr const glyph_t UpLadder{ characters::UpLadder, colors::White };
		constexpr const glyph_t DownLadder{ characters::DownLadder, colors::White };

		constexpr const glyph_t RightPatch{ characters::RightPatch, colors::White };
		constexpr const glyph_t LeftPatch{ characters::LeftPatch, colors::White };

		constexpr const glyph_t AltRightPatch{ characters::AltRightPatch, colors::White };
		constexpr const glyph_t AltLeftPatch{ characters::AltLeftPatch, colors::White };
	} // namespace glyphs
} // namespace necrowarp
