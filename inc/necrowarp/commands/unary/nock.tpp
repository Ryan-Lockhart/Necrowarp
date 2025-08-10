#pragma once

#include <necrowarp/commands/unary/nock.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, nock_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr) {
			return;
		}
		
		maybe_ranger->nock();
	}
} // namespace necrowarp
