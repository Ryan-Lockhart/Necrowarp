#pragma once

#include <necrowarp/commands/binary/nuzzle.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, nuzzle_t>::process() const noexcept {
		ptr<plonzo_t> maybe_plonzo{ entity_registry<MapType>.dependent at<plonzo_t>(source_position) };

		if (maybe_plonzo == nullptr || player.position != target_position) {
			return;
		}

		maybe_plonzo->randomize(random_engine);

		player.reinvigorate();
	}
} // namespace necrowarp
