#pragma once

#include <necrowarp/entities/good/adventurer.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t adventurer_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void adventurer_t::die(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, flesh_t{});

		spill_fluid<MapType>(position, fluid_type<adventurer_t>::type);

		player.receive_death_boon<adventurer_t>();

		++steam_stats::stats<steam_stat_e::AdventurersSlain>;

		scorekeeper.add(entity_e::Adventurer);

		death_sounds<adventurer_t>.delay(random_epoch_interval(random_engine), random_engine);
	}
} // namespace necrowarp
