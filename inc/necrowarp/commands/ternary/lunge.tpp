#pragma once

#include <necrowarp/commands/ternary/lunge.hpp>
#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, lunge_t>::process() noexcept {
		entity_registry.update<EntityType>(source_position, intermediate_position);

		entity_registry.process_command(entity_command_t<EntityType, clash_t>{ intermediate_position, target_position });
	}
} // namespace necrowarp
