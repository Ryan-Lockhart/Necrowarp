#pragma once

#include <necrowarp/commands/unary/calamitous_retaliation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, calamitous_retaliation_t>::process() const noexcept {}
} // namespace necrowarp
