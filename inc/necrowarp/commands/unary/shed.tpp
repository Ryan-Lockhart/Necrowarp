#pragma once

#include <necrowarp/commands/unary/shed.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, shed_t>::process() const noexcept {
		ptr<thetwo_t> maybe_thetwo{ entity_registry<MapType>.dependent at<thetwo_t>(source_position) };

		if (maybe_thetwo == nullptr || (!maybe_thetwo->is_shedding() && !maybe_thetwo->can_shed())) {
			return;
		}
		
		maybe_thetwo->shed();
	}
} // namespace necrowarp
