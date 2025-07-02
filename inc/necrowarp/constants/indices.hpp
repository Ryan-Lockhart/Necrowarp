#pragma once

#include <bleak/constants/indices.hpp>

namespace necrowarp::indices {
	using namespace bleak;
	using namespace bleak::indices;

	constexpr const u8 TwistedSuit{ 0x00 };
	constexpr const u8 ShimmeringSuit{ 0x01 };
	constexpr const u8 WrigglingSuit{ 0x02 };
	constexpr const u8 WrithingSuit{ 0x03 };

	constexpr const u8 TwistedKnight{ 0x04 };
	constexpr const u8 ShimmeringKnight{ 0x05 };
	constexpr const u8 WrigglingKnight{ 0x06 };
	constexpr const u8 WrithingKnight{ 0x07 };

	constexpr const u8 TwistedMetal{ 0x08 };
	constexpr const u8 ShimmeringMetal{ 0x09 };
	constexpr const u8 WrigglingMetal{ 0x0A };
	constexpr const u8 WrithingMetal{ 0x0B };

	constexpr const u8 Arrow{ 0x0C };

	constexpr const u8 CalciticShackle{ 0x0D };
	constexpr const u8 SpectralShackle{ 0x0E };
	constexpr const u8 SanguineShackle{ 0x0F };
	constexpr const u8 GalvanicShackle{ 0x10 };
	constexpr const u8 RavenousShackle{ 0x11 };
	constexpr const u8 WretchedShackle{ 0x12 };
	constexpr const u8 AbyssalShackle{ 0x13 };

	constexpr const u8 CalmPortal{ 0x14 };
	constexpr const u8 VocalPortal{ 0x15 };
	constexpr const u8 TurbulentPortal{ 0x16 };
	constexpr const u8 InsightfulPortal{ 0x17 };
	constexpr const u8 CollapsingPortal{ 0x18 };
	constexpr const u8 YawningPortal{ 0x19 };
	constexpr const u8 EchoingPortal{ 0x1A };
} // namespace necrowarp::indices
