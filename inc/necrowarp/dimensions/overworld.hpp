#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<> inline void game_s::plunge<dimension_e::Overworld>(rval<player_t> vagrant) noexcept {}
} // namespace necrowarp
