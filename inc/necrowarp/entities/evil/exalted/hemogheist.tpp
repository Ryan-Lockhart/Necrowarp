#pragma once

#include <necrowarp/entities/evil/exalted/hemogheist.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t hemogheist_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Eviscerate, position };
		}

		cauto descent_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void hemogheist_t::die(offset_t position) noexcept {
		spill_fluid<MapType>(position, fluid_type<hemogheist_t>::type);
	}
} // namespace necrowarp
