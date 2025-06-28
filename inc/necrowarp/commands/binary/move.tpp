#pragma once

#include <necrowarp/commands/binary/move.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, move_t>::process() const noexcept {
		if constexpr (is_berker<EntityType>::value) {
			ptr<EntityType> entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

			if (entity == nullptr) {
				return;
			}

			entity->exhaust();
		}

		entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

		if constexpr (is_player<EntityType>::value) {
			steam_stats::stats<steam_stat_e::MetersMoved, f32> += offset_t::distance<f32>(source_position, target_position);
		}
	}
} // namespace necrowarp
