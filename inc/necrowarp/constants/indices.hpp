#pragma once

#include <bleak/constants/indices.hpp>

namespace necrowarp::indices {
	using namespace bleak;
	using namespace bleak::indices;

	constexpr const u8 TwistedSuit{ 0x00 };
	constexpr const u8 ShimmeringSuit{ TwistedSuit + 1 };
	constexpr const u8 WrigglingSuit{ ShimmeringSuit + 1 };
	constexpr const u8 WrithingSuit{ WrigglingSuit + 1 };

	constexpr const u8 AdventurerHamaz{ WrithingSuit + 1 };
	constexpr const u8 MercenaryHamaz{ AdventurerHamaz + 1 };
	constexpr const u8 RangerHamaz{ MercenaryHamaz + 1 };
	constexpr const u8 SkulkerHamaz{ RangerHamaz + 1 };
	constexpr const u8 MistLadyHamaz{ SkulkerHamaz + 1 };
	constexpr const u8 BannerBearerHamaz{ MistLadyHamaz + 1 };
	constexpr const u8 BattleMonkHamaz{ BannerBearerHamaz + 1 };
	constexpr const u8 BerserkerHamaz{ BattleMonkHamaz + 1 };
	constexpr const u8 PaladinHamaz{ BerserkerHamaz + 1 };

	constexpr const u8 Chromalese{ PaladinHamaz + 1 };

	constexpr const u8 Bonespur{ Chromalese + 1 };
	constexpr const u8 Hemogheist{ Bonespur + 1 };

	constexpr const u8 TwistedKnight{ Hemogheist + 1 };
	constexpr const u8 ShimmeringKnight{ TwistedKnight + 1 };
	constexpr const u8 WrigglingKnight{ ShimmeringKnight + 1 };
	constexpr const u8 WrithingKnight{ WrigglingKnight + 1 };

	constexpr const u8 FurtiveHorror{ WrithingKnight + 1 };
	constexpr const u8 Isoscol{ FurtiveHorror + 1 };

	constexpr const u8 Berserker{ Isoscol + 1 };

	constexpr const u8 TwistedMetal{ Berserker + 1 };
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

	constexpr const u8 Pedestal{ EchoingPortal + 1 };

	constexpr const u8 Arrow{ Pedestal + 1 };
} // namespace necrowarp::indices
