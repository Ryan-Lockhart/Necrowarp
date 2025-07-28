#pragma once

#include <necrowarp/entities/evil/exalted/bonespur.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t bonespur_t::think(offset_t position) const noexcept {
		if (cauto good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; good_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, good_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> bonespur_t::die(offset_t position) noexcept {
		return death_info_t<Death>{ true };
	}
} // namespace necrowarp
