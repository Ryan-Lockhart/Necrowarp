#pragma once

#include <necrowarp/entities/evil/bonespur.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t bonespur_t::think() const noexcept {
		cauto descent_pos{ evil_goal_map.descend<zone_region_e::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	inline void bonespur_t::die() noexcept {}
} // namespace necrowarp
