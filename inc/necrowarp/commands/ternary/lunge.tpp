#pragma once

#include <necrowarp/commands/ternary/lunge.hpp>
#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, lunge_t>::process() const noexcept {
		entity_registry<MapType>.template update<EntityType>(source_position, intermediate_position);

		entity_command_t<EntityType, clash_t>{ intermediate_position, target_position }.template process<MapType>();
	}
} // namespace necrowarp
