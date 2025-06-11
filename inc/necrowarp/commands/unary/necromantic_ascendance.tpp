#pragma once

#include <necrowarp/commands/unary/necromantic_ascendance.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, necromantic_ascendance_t>::process() const noexcept {
		if (player.has_ascended() || !player.can_perform(discount_e::NecromanticAscendance)) {
			player_turn_invalidated = true;

			return;
		}

		player.max_out_divinity();
		freshly_divine = true;

		++steam_stats::stats<steam_stat_e::NecromanticAscensions, i32>;

		player.pay_cost(discount_e::NecromanticAscendance);
	}
} // namespace necrowarp
