#pragma once

#include <necrowarp/entities/good/paladin.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	inline command_pack_t paladin_t::think() const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry.contains<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map.descend<zone_region_e::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	inline void paladin_t::die() noexcept {
		object_registry.add<true>(skull_t{ position });
		object_registry.add<true>(metal_t{ position, galvanisation_e::Shimmering });

		fluid_map[position] += fluid_type<paladin_t>::type;

		++steam_stats::stats<steam_stat_e::PaladinsSlain, i32>;

		scorekeeper.add(entity_e::Paladin);
	}
} // namespace necrowarp
