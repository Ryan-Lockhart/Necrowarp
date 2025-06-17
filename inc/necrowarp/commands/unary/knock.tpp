#pragma once

#include <necrowarp/commands/unary/knock.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, knock_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.template at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr) {
			return;
		}
		
		maybe_ranger->knock();
	}
} // namespace necrowarp
