#pragma once

#include <necrowarp/commands/binary/graze.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, graze_t>::process() const noexcept {
		ptr<EntityType> maybe_grazer{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

		if (maybe_grazer == nullptr) {
			return;
		}

		ref<EntityType> grazeer{ *maybe_grazer };

		const object_e target_object{ determine_target<EntityType>(object_registry<MapType>.at(target_position)) };

		if (target_object != object_e::Flora) {
			return;
		}

		object_registry<MapType>.dependent remove<flora_t>(target_position);

		grazeer.fatten();
	}
} // namespace necrowarp
