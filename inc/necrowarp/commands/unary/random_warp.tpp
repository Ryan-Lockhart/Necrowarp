#pragma once

#include <necrowarp/commands/unary/random_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, random_warp_t>::process() const noexcept {
		if (!player.can_perform(discount_e::RandomWarp)) {
			return;
		}

		player.pay_cost(discount_e::RandomWarp);

		entity_registry.random_warp(source_position);
	}
} // namespace necrowarp
