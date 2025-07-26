#pragma once

#include <bleak/constants/glyphs.hpp>

#include <necrowarp/constants/characters.hpp>
#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace icons {
		using namespace bleak::icons;

		constexpr const glyph_t ChaoticWarp{ 0x00, colors::White };
		constexpr const glyph_t PreciseWarp{ 0x01, colors::White };

		constexpr const glyph_t Annihilate{ 0x02, colors::White };
		constexpr const glyph_t Repulse{ 0x03, colors::White };
		constexpr const glyph_t Calcify{ 0x04, colors::White };
		constexpr const glyph_t Incorporealize{ 0x05, colors::White };

		constexpr const glyph_t CalciticInvocation{ 0x06, colors::White };
		constexpr const glyph_t SpectralInvocation{ 0x07, colors::White };
		constexpr const glyph_t SanguineInvocation{ 0x08, colors::White };
		constexpr const glyph_t GalvanicInvocation{ 0x09, colors::White };
		constexpr const glyph_t RavenousInvocation{ 0x0A, colors::White };
		constexpr const glyph_t WretchedInvocation{ 0x0B, colors::White };
		constexpr const glyph_t CerebralInvocation{ 0x0C, colors::White };
		constexpr const glyph_t InfernalInvocation{ 0x0D, colors::White };

		constexpr const glyph_t NecromanticAscendance{ 0x0E, colors::White };
		constexpr const glyph_t CalamitousRetaliation{ 0x0F, colors::White };
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
		constexpr const glyph_t Abomination{ characters::Abomination, colors::White };
		constexpr const glyph_t Draugaz{ characters::Draugaz, colors::White };

		constexpr const glyph_t Wraith{ characters::Wraith, colors::White };
		constexpr const glyph_t FleshGolem{ characters::FleshGolem, colors::White };
		constexpr const glyph_t Dreadwurm{ characters::Dreadwurm, colors::White };

		constexpr const glyph_t Adventurer{ characters::Adventurer, colors::White };
		constexpr const glyph_t Mercenary{ characters::Mercenary, colors::White };

		constexpr const glyph_t UnnockedFullRanger{ characters::UnnockedFullRanger, colors::White };
		constexpr const glyph_t NockedFullRanger{ characters::NockedFullRanger, colors::White };
		constexpr const glyph_t UnnockedEmptyRanger{ characters::UnnockedEmptyRanger, colors::White };
		constexpr const glyph_t NockedEmptyRanger{ characters::NockedEmptyRanger, colors::White };
	
		constexpr const glyph_t Skulker{ characters::Skulker, colors::White };
		constexpr const glyph_t BattleMonk{ characters::BattleMonk, colors::White };
		constexpr const glyph_t MistLady{ characters::MistLady, colors::White };
		constexpr const glyph_t BannerBearer{ characters::BannerBearer, colors::White };

		constexpr const glyph_t NeonatalThetwo{ characters::NeonatalThetwo, colors::White };
		constexpr const glyph_t YoungThetwo{ characters::YoungThetwo, colors::White };
		constexpr const glyph_t MatureThetwo{ characters::MatureThetwo, colors::White };
		constexpr const glyph_t BulkyThetwo{ characters::BulkyThetwo, colors::White };
		constexpr const glyph_t GrossThetwo{ characters::GrossThetwo, colors::White };
		constexpr const glyph_t TitanicThetwo{ characters::TitanicThetwo, colors::White };

		constexpr const glyph_t Paladin{ characters::Paladin, colors::White };

		constexpr const glyph_t FreshBones{ characters::FreshBones, colors::White };
		constexpr const glyph_t AnimateBones{ characters::AnimateBones, colors::White };
		constexpr const glyph_t RottedBones{ characters::RottedBones, colors::White };

		constexpr const glyph_t Flesh{ characters::Flesh, colors::White };

		constexpr const glyph_t HealthyCerebrum{ characters::HealthyCerebrum, colors::White };
		constexpr const glyph_t StrickenCerebrum{ characters::StrickenCerebrum, colors::White };

		constexpr const glyph_t Crevice{ characters::Crevice, colors::White };

		constexpr const glyph_t UpLadder{ characters::UpLadder, colors::White };
		constexpr const glyph_t DownLadder{ characters::DownLadder, colors::White };

		constexpr const glyph_t Pedestal{ characters::Pedestal, colors::White };

		constexpr const glyph_t RightPatch{ characters::RightPatch, colors::White };
		constexpr const glyph_t LeftPatch{ characters::LeftPatch, colors::White };

		constexpr const glyph_t AltRightPatch{ characters::AltRightPatch, colors::White };
		constexpr const glyph_t AltLeftPatch{ characters::AltLeftPatch, colors::White };
	} // namespace glyphs
} // namespace necrowarp
