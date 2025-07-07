#pragma once

#include <bleak/constants/characters.hpp>

namespace necrowarp::characters {
	using namespace bleak;
	using namespace bleak::characters;

	constexpr const u8 Floor{ 0x00 };

	constexpr const u8 UnarmoredPlayer{ 0x10 };
	constexpr const u8 ArmoredPlayer{ UnarmoredPlayer + 1 };

	constexpr const u8 FreshSkeleton{ ArmoredPlayer + 1 };
	constexpr const u8 AnimateSkeleton{ FreshSkeleton + 1 };
	constexpr const u8 RottedSkeleton{ AnimateSkeleton + 1 };

	constexpr const u8 Cultist{ RottedSkeleton + 1 };
	constexpr const u8 Bloodhound{ Cultist + 1 };
	constexpr const u8 Abomination{ Bloodhound + 1 };
	constexpr const u8 Draugaz{ Abomination + 1 };

	constexpr const u8 Wraith{ Draugaz + 1 };
	constexpr const u8 FleshGolem{ Wraith + 1 };
	constexpr const u8 Dreadwurm{ FleshGolem + 1 };

	constexpr const u8 Adventurer{ Dreadwurm + 1 };
	constexpr const u8 Mercenary{ Adventurer + 1 };

	constexpr const u8 UnnockedFullRanger{ Mercenary + 1 };
	constexpr const u8 NockedFullRanger{ UnnockedFullRanger + 1 };
	constexpr const u8 UnnockedEmptyRanger{ NockedFullRanger + 1 };
	constexpr const u8 NockedEmptyRanger{ UnnockedEmptyRanger + 1 };

	constexpr const u8 Skulker{ NockedEmptyRanger + 1 };
	constexpr const u8 BattleMonk{ Skulker + 1 };
	constexpr const u8 MistLady{ BattleMonk + 1 };
	constexpr const u8 BannerBearer{ MistLady + 1 };

	constexpr const u8 NeonatalThetwo{ BannerBearer + 1 };
	constexpr const u8 YoungThetwo{ NeonatalThetwo + 1 };
	constexpr const u8 MatureThetwo{ YoungThetwo + 1 };
	constexpr const u8 GrossThetwo{ MatureThetwo + 1 };
	constexpr const u8 TitanicThetwo{ GrossThetwo + 1 };

	constexpr const u8 Paladin{ TitanicThetwo + 1 };

	constexpr const u8 FreshBones{ Paladin + 1 };
	constexpr const u8 AnimateBones{ FreshBones + 1 };
	constexpr const u8 RottedBones{ AnimateBones + 1 };

	constexpr const u8 Flesh{ RottedBones + 1 };

	constexpr const u8 HealthyCerebrum{ Flesh + 1 };
	constexpr const u8 StrickenCerebrum{ HealthyCerebrum + 1 };

	constexpr const u8 UpLadder{ StrickenCerebrum + 1 };
	constexpr const u8 DownLadder{ UpLadder + 1 };

	constexpr const u8 EmptyPedestal{ DownLadder + 1 };
	constexpr const u8 GrimoirePedestal{ EmptyPedestal + 1 };

	constexpr const u8 RightPatch{ GrimoirePedestal + 1 };
	constexpr const u8 LeftPatch{ RightPatch + 1 };

	constexpr const u8 AltRightPatch{ LeftPatch + 1 };
	constexpr const u8 AltLeftPatch{ AltRightPatch + 1 };
} // namespace necrowarp::characters
