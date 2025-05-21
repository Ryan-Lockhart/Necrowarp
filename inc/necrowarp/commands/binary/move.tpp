#pragma once

#include <necrowarp/commands/binary/move.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, move_t>::process() noexcept {
		entity_registry.update<EntityType>(source_position, target_position);

		if constexpr (!is_player<EntityType>::value) {
			return;
		}

		steam_stats::stats<steam_stat_e::MetersMoved, f32> += offset_t::distance<f32>(source_position, target_position);
	}
} // namespace necrowarp
