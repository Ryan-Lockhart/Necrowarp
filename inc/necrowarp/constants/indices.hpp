#pragma once

#include <bleak/constants/indices.hpp>

namespace necrowarp::indices {
	using namespace bleak;
	using namespace bleak::indices;

	constexpr const u8 TwistedSuit{ 0x00 };
	constexpr const u8 ShimmeringSuit{ TwistedSuit + 1 };
	constexpr const u8 WrigglingSuit{ ShimmeringSuit + 1 };
	constexpr const u8 WrithingSuit{ WrigglingSuit + 1 };

	constexpr const u8 Bonespur{ WrithingSuit + 1 };
	constexpr const u8 Hemogheist{ Bonespur + 1 };

	constexpr const u8 TwistedKnight{ Hemogheist + 1 };
	constexpr const u8 ShimmeringKnight{ TwistedKnight + 1 };
	constexpr const u8 WrigglingKnight{ ShimmeringKnight + 1 };
	constexpr const u8 WrithingKnight{ WrigglingKnight + 1 };

	constexpr const u8 Berserker{ WrithingKnight + 1 };

	constexpr const u8 TwistedMetal{ Berserker + 1 };
	constexpr const u8 ShimmeringMetal{ TwistedMetal + 1 };
	constexpr const u8 WrigglingMetal{ ShimmeringMetal + 1 };
	constexpr const u8 WrithingMetal{ WrigglingMetal + 1 };

	constexpr const u8 Arrow{ WrithingMetal + 1 };

	constexpr const u8 CalciticShackle{ Arrow + 1 };
	constexpr const u8 SpectralShackle{ CalciticShackle + 1 };
	constexpr const u8 SanguineShackle{ SpectralShackle + 1 };
	constexpr const u8 GalvanicShackle{ SanguineShackle + 1 };
	constexpr const u8 RavenousShackle{ GalvanicShackle + 1 };
	constexpr const u8 WretchedShackle{ RavenousShackle + 1 };
	constexpr const u8 InfernalShackle{ WretchedShackle + 1 };
	constexpr const u8 CerebralShackle{ InfernalShackle + 1 };

	constexpr const u8 CalmPortal{ CerebralShackle + 1 };
	constexpr const u8 VocalPortal{ CalmPortal + 1 };
	constexpr const u8 TurbulentPortal{ VocalPortal + 1 };
	constexpr const u8 InsightfulPortal{ TurbulentPortal + 1 };
	constexpr const u8 CollapsingPortal{ InsightfulPortal + 1 };
	constexpr const u8 YawningPortal{ CollapsingPortal + 1 };
	constexpr const u8 EchoingPortal{ YawningPortal + 1 };
} // namespace necrowarp::indices
