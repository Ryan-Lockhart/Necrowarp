#pragma once

#include <necrowarp/commands/binary/descend.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, descend_t>::process() const noexcept {
		++steam_stats::stats<steam_stat_e::MetersMoved>;

		descent_flag = true;
	}
} // namespace necrowarp
