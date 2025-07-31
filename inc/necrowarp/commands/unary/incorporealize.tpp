#pragma once

#include <necrowarp/commands/unary/incorporealize.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, incorporealize_t>::process() const noexcept {
		if (player.is_incorporeal() || !player.can_perform(discount_e::Incorporealize)) {
			player_turn_invalidated = true;

			return;
		}

		player.max_out_phantasm();
		freshly_incorporeal = true;

		++steam_stats::stats<steam_stat_e::Incorporealizations>;

		player.pay_cost(discount_e::Incorporealize);
	}
} // namespace necrowarp
