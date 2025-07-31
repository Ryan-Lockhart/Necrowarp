#pragma once

#include <necrowarp/commands/binary/annihilate.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, annihilate_t>::process() const noexcept {
		if (!player.can_perform(discount_e::Annihilate) || !literature::can_use(grimoire_e::Annihilate)) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<steam_stat_e::Annihilations>;

		player.pay_cost(discount_e::Annihilate);

		literature::use(grimoire_e::Annihilate);
	}
} // namespace necrowarp
