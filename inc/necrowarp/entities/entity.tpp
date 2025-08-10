#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	namespace globals {
		static constexpr entity_e OopsAllEnum{ entity_e::Ranger };

		template<> inline constexpr bool OopsAll<OopsAllEnum>{ false };
		
		template<GoodEntity... Entities> static constexpr usize OopsAllCount{ (OopsAll<to_entity_enum<Entities>::value> + ...) };

		static_assert(OopsAllCount<ALL_GOOD_NPCS> <= 1, "cannot force multiple enemy types!");

		static constexpr bool OopsAllEnabled{ OopsAllCount<ALL_GOOD_NPCS> == 1 };
	}
} // namespace necrowarp
