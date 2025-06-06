#pragma once

#include <necrowarp/commands/binary/resurrect.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, resurrect_t>::process() const noexcept {
		if (!entity_registry.contains<priest_t>(source_position)) {
			return;
		}

		if (!object_registry.contains<skull_t>(target_position)) {
			return;
		}

		cauto skull{ object_registry.at<skull_t>(target_position) };

		if (skull == nullptr || skull->state != decay_e::Fresh ) {
			return;
		}

		auto priest{ entity_registry.at<priest_t>(source_position) };

		if (priest == nullptr || !priest->can_resurrect()) {
			return;
		}

		object_registry.remove<skull_t>(target_position);

		entity_registry.add(adventurer_t{ target_position });

		++steam_stats::stats<steam_stat_e::AdventurersResurrected, i32>;

		priest->pay_resurrect_cost();
	}
} // namespace necrowarp
