#pragma once

#include <necrowarp/entities/good/mercenary.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t mercenary_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_NON_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ non_good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::Wander, position };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void mercenary_t::killed(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, flesh_t{});
		object_registry<MapType>.spill(position, metal_t{});
		object_registry<MapType>.spill(position, cerebra_t{ entity_e::Mercenary });

		player.receive_death_boon<mercenary_t>();

		++steam_stats::stats<steam_stat_e::MercenariesSlain>;

		scorekeeper.add(entity_e::Mercenary);
	}

	template<map_type_e MapType> inline i8 mercenary_t::devoured(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, metal_t{});
		object_registry<MapType>.spill(position, cerebra_t{ entity_e::Mercenary });

		player.receive_death_boon<mercenary_t>();

		++steam_stats::stats<steam_stat_e::MercenariesSlain>;

		scorekeeper.add(entity_e::Mercenary);

		return 1;
	}
} // namespace necrowarp
