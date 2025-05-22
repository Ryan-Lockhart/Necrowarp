#pragma once

#include <necrowarp/commands/unary/suicide.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, suicide_t>::process() const noexcept {
		if (!entity_registry.contains<priest_t>(source_position)) {
			return;
		}

		entity_registry.remove<priest_t>(source_position);

		entity_registry.add(skull_t{ source_position });

		fluid_map[source_position] += fluid_type<priest_t>::type;

		++steam_stats::stats<steam_stat_e::PlayerKills, i32>;

		++game_stats.player_kills;
	}
} // namespace necrowarp
