#pragma once

#include <necrowarp/commands/binary/anoint.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, anoint_t>::process() const noexcept {
		if (!entity_registry<MapType>.template contains<priest_t>(source_position)) {
			return;
		}

		if (!entity_registry<MapType>.template contains<adventurer_t>(target_position)) {
			return;
		}

		auto priest{ entity_registry<MapType>.template at<priest_t>(source_position) };

		if (priest == nullptr || !priest->can_anoint()) {
			return;
		}

		entity_registry<MapType>.template remove<adventurer_t>(target_position);

		entity_registry<MapType>.add(paladin_t{ target_position });

		++steam_stats::stats<steam_stat_e::PaladinsOrdained, i32>;

		priest->pay_ordain_cost();
	}
} // namespace necrowarp
