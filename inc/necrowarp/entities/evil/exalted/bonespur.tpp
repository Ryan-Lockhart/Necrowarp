#pragma once

#include <necrowarp/entities/evil/exalted/bonespur.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t bonespur_t::think(offset_t position) const noexcept {
		cauto descent_pos{ non_evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::Wander, position };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> bonespur_t::die(offset_t position) noexcept {
		return death_info_t<Death>{ true };
	}
} // namespace necrowarp
