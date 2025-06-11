#pragma once

#include <necrowarp/commands/binary/exorcise.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, exorcise_t>::process() const noexcept {
		if (!entity_registry<MapType>.template contains<priest_t>(source_position)) {
			return;
		}

		if (!object_registry<MapType>.template contains<skull_t>(target_position)) {
			return;
		}

		auto priest{ entity_registry<MapType>.template at<priest_t>(source_position) };

		if (priest == nullptr) {
			return;
		}

		object_registry<MapType>.template remove<skull_t>(target_position);

		++steam_stats::stats<steam_stat_e::SkullsExorcised, i32>;

		priest->receive_exorcism_boon();
	}
} // namespace necrowarp
