#pragma once

#include <bleak/constants/indices.hpp>

namespace necrowarp::indices {
	using namespace bleak;
	using namespace bleak::indices;

	constexpr const u8 Player{ 0x00 };
	constexpr const u8 ArmoredPlayer{ Player + 1 };
	constexpr const u8 IncorporealPlayer{ ArmoredPlayer + 1 };
	constexpr const u8 ArmoredIncorporealPlayer{ IncorporealPlayer + 1 };
	constexpr const u8 AscendedPlayer{ ArmoredIncorporealPlayer + 1 };
	constexpr const u8 ArmoredAscendedPlayer{ AscendedPlayer + 1 };
	constexpr const u8 IncorporealAscendedPlayer{ ArmoredAscendedPlayer + 1 };
	constexpr const u8 ArmoredIncorporealAscendedPlayer{ IncorporealAscendedPlayer + 1 };

	constexpr const u8 RogueNecromancer{ ArmoredIncorporealAscendedPlayer + 1 };

	constexpr const u8 TwistedSuit{ RogueNecromancer + 1 };
	constexpr const u8 ShimmeringSuit{ TwistedSuit + 1 };
	constexpr const u8 WrigglingSuit{ ShimmeringSuit + 1 };
	constexpr const u8 WrithingSuit{ WrigglingSuit + 1 };

	constexpr const u8 AdventurerHamaz{ WrithingSuit + 1 };
	constexpr const u8 MercenaryHamaz{ AdventurerHamaz + 1 };
	constexpr const u8 RangerHamaz{ MercenaryHamaz + 1 };
	constexpr const u8 SkulkerHamaz{ RangerHamaz + 1 };
	constexpr const u8 ManslingHamaz{ SkulkerHamaz + 1 };
	constexpr const u8 MistLadyHamaz{ ManslingHamaz + 1 };
	constexpr const u8 BannerBearerHamaz{ MistLadyHamaz + 1 };
	constexpr const u8 MedicusHamaz{ BannerBearerHamaz + 1 };
	constexpr const u8 BattleMonkHamaz{ MedicusHamaz + 1 };
	constexpr const u8 BerserkerHamaz{ BattleMonkHamaz + 1 };
	constexpr const u8 HexeaterHamaz{ BerserkerHamaz + 1 };
	constexpr const u8 PaladinHamaz{ HexeaterHamaz + 1 };

	constexpr const u8 Chromalese{ PaladinHamaz + 1 };

	constexpr const u8 Bonespur{ Chromalese + 1 };
	constexpr const u8 Wraith{ Bonespur + 1 };
	constexpr const u8 Hemogheist{ Wraith + 1 };

	constexpr const u8 TwistedKnight{ Hemogheist + 1 };
	constexpr const u8 ShimmeringKnight{ TwistedKnight + 1 };
	constexpr const u8 WrigglingKnight{ ShimmeringKnight + 1 };
	constexpr const u8 WrithingKnight{ WrigglingKnight + 1 };

	constexpr const u8 FurtiveHorror{ WrithingKnight + 1 };
	constexpr const u8 Isoscel{ FurtiveHorror + 1 };

	constexpr const u8 Fauna{ Isoscel + 1 };

	constexpr const u8 Adventurer{ Fauna + 1 };
	constexpr const u8 Mercenary{ Adventurer + 1 };

	constexpr const u8 VisibleSkulker{ Mercenary + 1 };
	constexpr const u8 ShroudedSkulker{ VisibleSkulker + 1 };
	constexpr const u8 ImperceptibleSkulker{ ShroudedSkulker + 1 };

	constexpr const u8 Mansling{ ImperceptibleSkulker + 1 };

	constexpr const u8 Berserker{ Mansling + 1 };

	constexpr const u8 ExtinguishedHexeater{ Berserker + 1 };
	constexpr const u8 SmolderingHexeater{ ExtinguishedHexeater + 1 };
	constexpr const u8 SwellingHexeater{ SmolderingHexeater + 1 };
	constexpr const u8 BlazingHexeater{ SwellingHexeater + 1 };

	constexpr const u8 VengeantPaladin{ BlazingHexeater + 1 };
	constexpr const u8 DowntroddenPaladin{ VengeantPaladin + 1 };
	constexpr const u8 FallenPaladin{ DowntroddenPaladin + 1 };
	constexpr const u8 AlacritousPaladin{ FallenPaladin + 1 };
	constexpr const u8 RighteousPaladin{ AlacritousPaladin + 1 };
	constexpr const u8 ZealousPaladin{ RighteousPaladin + 1 };
	constexpr const u8 AscendantPaladin{ ZealousPaladin + 1 };

	constexpr const u8 TwistedMetal{ AscendantPaladin + 1 };
	constexpr const u8 ShimmeringMetal{ TwistedMetal + 1 };
	constexpr const u8 WrigglingMetal{ ShimmeringMetal + 1 };
	constexpr const u8 WrithingMetal{ WrigglingMetal + 1 };

	constexpr const u8 CalciticShackle{ WrithingMetal + 1 };
	constexpr const u8 SpectralShackle{ CalciticShackle + 1 };
	constexpr const u8 SanguineShackle{ SpectralShackle + 1 };
	constexpr const u8 GalvanicShackle{ SanguineShackle + 1 };
	constexpr const u8 RavenousShackle{ GalvanicShackle + 1 };
	constexpr const u8 WretchedShackle{ RavenousShackle + 1 };
	constexpr const u8 CerebralShackle{ WretchedShackle + 1 };
	constexpr const u8 InfernalShackle{ CerebralShackle + 1 };

	constexpr const u8 CalmPortal{ InfernalShackle + 1 };
	constexpr const u8 VocalPortal{ CalmPortal + 1 };
	constexpr const u8 TurbulentPortal{ VocalPortal + 1 };
	constexpr const u8 InsightfulPortal{ TurbulentPortal + 1 };
	constexpr const u8 CollapsingPortal{ InsightfulPortal + 1 };
	constexpr const u8 YawningPortal{ CollapsingPortal + 1 };
	constexpr const u8 EchoingPortal{ YawningPortal + 1 };

	constexpr const u8 Gateway{ EchoingPortal + 1 };

	constexpr const u8 Pedestal{ Gateway + 1 };

	constexpr const u8 Arrow{ Pedestal + 1 };
	constexpr const u8 Flora{ Arrow + 1 };

	constexpr const u8 Count{ Flora + 1 };
} // namespace necrowarp::indices
