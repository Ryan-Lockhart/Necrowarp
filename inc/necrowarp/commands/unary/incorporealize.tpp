#pragma once

#include <necrowarp/commands/unary/incorporealize.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, incorporealize_t>::process() const noexcept {
		if (!player.can_perform<MapType>(grimoire_e::Incorporealize)) {
			player_turn_invalidated = true;

			return;
		}

		player.max_out_phantasm();
		freshly_incorporeal = true;

		++steam_stats::stats<stat_e::Incorporealizations>;

		player.pay_cost<MapType>(grimoire_e::Incorporealize);

		literature::use(grimoire_e::Incorporealize);
	}
} // namespace necrowarp
