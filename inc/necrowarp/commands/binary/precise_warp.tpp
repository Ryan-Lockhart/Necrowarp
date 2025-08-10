#pragma once

#include <necrowarp/commands/binary/precise_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	extern player_t player;

	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, precise_warp_t>::process() const noexcept {
		if (!player.can_perform(grimoire_e::PreciseWarp)) {
			player_turn_invalidated = true;

			return;
		}

		entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

		++steam_stats::stats<steam_stat_e::PreciseWarps>;

		steam_stats::stats<steam_stat_e::MetersWarped> += offset_t::distance<f32>(source_position, player.position);

		player.pay_cost(grimoire_e::PreciseWarp);

		literature::use(grimoire_e::PreciseWarp);

		warped_from = std::nullopt;		
	}
} // namespace necrowarp
