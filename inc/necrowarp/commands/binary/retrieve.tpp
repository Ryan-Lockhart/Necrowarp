#pragma once

#include <necrowarp/commands/binary/retrieve.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, retrieve_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr || maybe_ranger->ammunition_full()) {
			return;
		}
		
		ptr<arrow_t> maybe_arrow{ object_registry<MapType>.dependent at<arrow_t>(target_position) };

		if (maybe_arrow == nullptr) {
			return;
		}

		const i8 capacity{ maybe_ranger->get_capacity() };
		const i8 disparity{ maybe_arrow->stack_size() - capacity };

		if (maybe_arrow->has_singular() && !object_registry<MapType>.dependent remove<arrow_t>(target_position)) {
			maybe_ranger->retrieve();
		} else if (m)

	}
} // namespace necrowarp
