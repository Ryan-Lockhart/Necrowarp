#pragma once

#include <necrowarp/commands/binary/depart.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, depart_t>::process() const noexcept {
		ptr<EntityType> maybe_entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

		if (maybe_entity == nullptr) {
			return;
		}

		ptr<ladder_t> maybe_ladder{ object_registry<MapType>.dependent at<ladder_t>(target_position) };
		
		if (maybe_ladder == nullptr || maybe_ladder->is_down_ladder() || maybe_ladder->has_shackle()) {
			return;
		}

		entity_registry<MapType>.dependent remove<EntityType>(source_position);

		offmap_reinforcements<EntityType>.emplace(game_stats.current_departure_epoch());
	}
} // namespace necrowarp
