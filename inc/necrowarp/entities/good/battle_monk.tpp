#pragma once

#include <necrowarp/entities/good/battle_monk.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t battle_monk_t::think(offset_t position) const noexcept {
		if (!entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_EVIL>(position) && !object_registry<MapType>.dependent contains<ladder_t>(position) && !is_zen()) {
			return command_pack_t{ command_e::Meditate, position };
		} else if (entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_EVIL>(position)){
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (entity_registry<MapType>.dependent empty<ALL_EVIL>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Batter, position, current_position };
			}
		}		

		cauto descent_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void battle_monk_t::die(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, flesh_t{});
		object_registry<MapType>.spill(position, cerebra_t{ entity_e::BattleMonk });

		player.receive_death_boon<battle_monk_t>();

		++steam_stats::stats<steam_stat_e::BattleMonksSlain>;

		scorekeeper.add(entity_e::BattleMonk);
	}
} // namespace necrowarp
