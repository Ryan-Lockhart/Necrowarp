#pragma once

#include <necrowarp/commands/unary/mitose.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, mitose_t>::process() const noexcept {
		ptr<abomination_t> maybe_abomination{ entity_registry<MapType>.dependent at<abomination_t>(source_position) };

		if (maybe_abomination == nullptr || !maybe_abomination->can_mitose<MapType>(source_position)) {
			return;
		}
		
		maybe_abomination->mitose<MapType>(source_position);
	}
} // namespace necrowarp
