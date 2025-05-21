#pragma once

#include <necrowarp/commands/binary/anoint.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, anoint_t>::process() noexcept {
		if (!entity_registry.contains<priest_t>(source_position)) {
			return;
		}

		if (!entity_registry.contains<adventurer_t>(target_position)) {
			return;
		}

		auto priest{ entity_registry.at<priest_t>(source_position) };

		if (priest == nullptr || !priest->can_anoint()) {
			return;
		}

		entity_registry.remove<adventurer_t>(target_position);

		entity_registry.add(paladin_t{ target_position });

		++steam_stats::stats<steam_stat_e::PaladinsOrdained, i32>;

		priest->pay_ordain_cost();
	}
} // namespace necrowarp
