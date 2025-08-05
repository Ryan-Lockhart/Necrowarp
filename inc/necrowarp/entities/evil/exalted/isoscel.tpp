#pragma once

#include <necrowarp/entities/evil/exalted/isoscel.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t isoscel_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Eviscerate, position };
		}

		if (cauto victim_pos{ entity_registry<MapType>.dependent nearest<distance_function_e::Chebyshev, ALL_GOOD>(position) }; victim_pos.has_value()) {
			for (cauto offset : shuffled_offsets<distance_function_e::Chebyshev>(random_engine)) {
				cauto current_pos{ victim_pos.value() + offset };

				if (!game_map<MapType>.dependent within<region_e::Interior>(current_pos) || game_map<MapType>[current_pos].solid || !entity_registry<MapType>.empty(current_pos)) {
					continue;
				}

				return command_pack_t{ command_e::Eradicate, position, current_pos };
			}
		}

		if (cauto victim_pos{ entity_registry<MapType>.dependent nearest<distance_function_e::Chebyshev, ALL_NEUTRAL>(position) }; victim_pos.has_value()) {
			for (cauto offset : shuffled_offsets<distance_function_e::Chebyshev>(random_engine)) {
				cauto current_pos{ victim_pos.value() + offset };

				if (!game_map<MapType>.dependent within<region_e::Interior>(current_pos) || game_map<MapType>[current_pos].solid || !entity_registry<MapType>.empty(current_pos)) {
					continue;
				}

				return command_pack_t{ command_e::Eradicate, position, current_pos };
			}
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> isoscel_t::die(offset_t position) noexcept {
		return death_info_t<Death>{ true };
	}
} // namespace necrowarp
