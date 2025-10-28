#pragma once

#include <necrowarp/commands/binary/pet.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, pet_t>::process() const noexcept {
		const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(target_position) };

		if (!maybe_entity.has_value() || maybe_entity.value() != entity_e::Plonzo) {
			player_turn_invalidated = true;

			return;
		}

		ptr<plonzo_t> maybe_plonzo{ entity_registry<MapType>.dependent at<plonzo_t>(target_position) };

		if (maybe_plonzo == nullptr) {
			player_turn_invalidated = true;

			return;
		}

		maybe_plonzo->randomize(random_engine);
	}
} // namespace necrowarp
