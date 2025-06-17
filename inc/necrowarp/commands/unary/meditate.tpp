#pragma once

#include <necrowarp/commands/unary/meditate.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, meditate_t>::process() const noexcept {
		ptr<battle_monk_t> maybe_monk{ entity_registry<MapType>.template at<battle_monk_t>(source_position) };

		if (maybe_monk == nullptr || maybe_monk->max_qi()) {
			return;
		}
		
		maybe_monk->harmonize();
	}
} // namespace necrowarp
