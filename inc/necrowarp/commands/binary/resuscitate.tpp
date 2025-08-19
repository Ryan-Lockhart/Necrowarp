#pragma once

#include <necrowarp/commands/binary/resuscitate.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, resuscitate_t>::process() const noexcept {
		ptr<medicus_t> maybe_medicus{ entity_registry<MapType>.dependent at<medicus_t>(source_position) };

		if (maybe_medicus == nullptr) {
			return;
		}

		maybe_medicus->resuscitate<MapType>(target_position);
	}
} // namespace necrowarp
