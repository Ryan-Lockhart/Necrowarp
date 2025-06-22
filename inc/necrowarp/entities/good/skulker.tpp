#pragma once

#include <necrowarp/entities/good/skulker.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t skulker_t::think() const noexcept {
		concealment = skulker_t::determine_concealment(skulker_goal_map<MapType>.at(position));

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.template contains<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map<MapType>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void skulker_t::die() noexcept {
		object_registry<MapType>.template add<true>(skull_t{ position });
		object_registry<MapType>.template add<true>(flesh_t{ position });

		fluid_map<MapType>[position] += fluid_type<skulker_t>::type;

		player.receive_death_boon<skulker_t>();

		++steam_stats::stats<steam_stat_e::SkulkersSlain, i32>;

		scorekeeper.add(entity_e::Skulker);
	}
} // namespace necrowarp
