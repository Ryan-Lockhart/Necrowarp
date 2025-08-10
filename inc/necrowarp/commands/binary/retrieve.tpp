#pragma once

#include <necrowarp/commands/binary/retrieve.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>
#include <necrowarp/literature.hpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, retrieve_t>::process() const noexcept {
		if constexpr (std::is_same<EntityType, player_t>::value) {
			ptr<pedestal_t> maybe_pedestal{ object_registry<MapType>.dependent at<pedestal_t>(target_position) };

			if (maybe_pedestal == nullptr || !maybe_pedestal->can_loot() || !literature::can_acquire(maybe_pedestal->grimoire) || !maybe_pedestal->loot()) {
				player_turn_invalidated = true;

				return;
			}
			
			literature::acquire(maybe_pedestal->grimoire);
		} else if constexpr (std::is_same<EntityType, ranger_t>::value) {
			ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

			if (maybe_ranger == nullptr || maybe_ranger->ammunition_full()) {
				return;
			}
			
			ptr<arrow_t> maybe_arrow{ object_registry<MapType>.dependent at<arrow_t>(target_position) };

			if (maybe_arrow == nullptr) {
				return;
			}

			const i8 capacity{ maybe_ranger->get_capacity() };

			if (maybe_arrow->has_singular() && !object_registry<MapType>.dependent remove<arrow_t>(target_position)) {
				maybe_ranger->retrieve();
			} else if (maybe_arrow->has_multiple()) {
				if (capacity >= maybe_arrow->stack_size()) {
					object_registry<MapType>.dependent remove<arrow_t>(target_position);
				} else {
					(*maybe_arrow) -= capacity;
				}

				maybe_ranger->retrieve(capacity);
			}
		}
	}
} // namespace necrowarp
