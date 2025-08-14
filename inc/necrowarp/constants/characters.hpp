#pragma once

#include <bleak/constants/characters.hpp>

namespace necrowarp::characters {
	using namespace bleak;
	using namespace bleak::characters;

	constexpr const u8 Floor{ 0x00 };

	constexpr const u8 CavernTileOrigin{ 0x00 };	
	constexpr const u8 FluidTileOrigin{ 0x10 };

	constexpr const u8 FreshSkeleton{ 0x20 };
	constexpr const u8 AnimateSkeleton{ FreshSkeleton + 1 };
	constexpr const u8 RottedSkeleton{ AnimateSkeleton + 1 };

	constexpr const u8 Cultist{ RottedSkeleton + 1 };
	constexpr const u8 Bloodhound{ Cultist + 1 };
	constexpr const u8 Abomination{ Bloodhound + 1 };
	constexpr const u8 Draugaz{ Abomination + 1 };

	constexpr const u8 FleshGolem{ Draugaz + 1 };
	constexpr const u8 Dreadwurm{ FleshGolem + 1 };

	constexpr const u8 UnnockedFullRanger{ Dreadwurm + 1 };
	constexpr const u8 NockedFullRanger{ UnnockedFullRanger + 1 };
	constexpr const u8 UnnockedEmptyRanger{ NockedFullRanger + 1 };
	constexpr const u8 NockedEmptyRanger{ UnnockedEmptyRanger + 1 };

	constexpr const u8 MistLady{ NockedEmptyRanger + 1 };
	constexpr const u8 BannerBearer{ MistLady + 1 };

	constexpr const u8 StockedMedicus{ BannerBearer + 1 };
	constexpr const u8 DepletedMedicus{ StockedMedicus + 1 };

	constexpr const u8 BattleMonk{ DepletedMedicus + 1 };

	constexpr const u8 NeonatalThetwo{ BattleMonk + 1 };
	constexpr const u8 YoungThetwo{ NeonatalThetwo + 1 };
	constexpr const u8 MatureThetwo{ YoungThetwo + 1 };
	constexpr const u8 BulkyThetwo{ MatureThetwo + 1 };
	constexpr const u8 GrossThetwo{ BulkyThetwo + 1 };
	constexpr const u8 TitanicThetwo{ GrossThetwo + 1 };

	constexpr const u8 FreshBones{ TitanicThetwo + 1 };
	constexpr const u8 AnimateBones{ FreshBones + 1 };
	constexpr const u8 RottedBones{ AnimateBones + 1 };

	constexpr const u8 Flesh{ RottedBones + 1 };

	constexpr const u8 HealthyCerebrum{ Flesh + 1 };
	constexpr const u8 StrickenCerebrum{ HealthyCerebrum + 1 };

	constexpr const u8 Crevice{ StrickenCerebrum + 1 };

	constexpr const u8 UpLadder{ Crevice + 1 };
	constexpr const u8 DownLadder{ UpLadder + 1 };

	constexpr const u8 Pedestal{ DownLadder + 1 };

	constexpr const u8 RightPatch{ Pedestal + 1 };
	constexpr const u8 LeftPatch{ RightPatch + 1 };

	constexpr const u8 AltRightPatch{ LeftPatch + 1 };
	constexpr const u8 AltLeftPatch{ AltRightPatch + 1 };

	constexpr const u8 CroppedRightPatch{ AltLeftPatch + 1 };
	constexpr const u8 CroppedLeftPatch{ CroppedRightPatch + 1 };

	constexpr const u8 Count{ CroppedLeftPatch + 1 };

	constexpr const u8 Rows{ Count / 16 + 1 };
} // namespace necrowarp::characters
