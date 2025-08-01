#pragma once

#include <necrowarp/commands/unary/repulse.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>
#include <necrowarp/literature.hpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, repulse_t>::process() const noexcept {
		if (!player.can_perform(grimoire_e::Repulse)) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<steam_stat_e::Repulsions>;

		player.pay_cost(grimoire_e::Repulse);

		literature::use(grimoire_e::Repulse);
	}
} // namespace necrowarp
