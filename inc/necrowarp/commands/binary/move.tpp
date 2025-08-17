#pragma once

#include <necrowarp/commands/binary/move.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/entities/good/mist_lady.hpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, move_t>::process() const noexcept {
		if constexpr (is_afflictable<EntityType>::value || is_berker<EntityType>::value) {
			ptr<EntityType> entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

			if (entity == nullptr) {
				return;
			}

		 	if (entity_goal_map<MapType, mist_lady_t>[source_position] <= mist_lady_t::EffectRadius) {
		 		entity->soothe();
		 	}

			if constexpr (is_berker<EntityType>::value) {
				if (entity->is_calm()) {
					entity->recuperate();
				} else {
					entity->exhaust();
				}
			}
		}

		entity_registry<MapType>.dependent update<EntityType>(source_position, target_position);

		if constexpr (is_player<EntityType>::value) {
			if (player.is_incorporeal() && game_map<MapType>[target_position].solid && entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_EVIL>(source_position)) {
				steam_stats::unlock(achievement_e::ExceptionalIncorporealizeUsage);
			}

			steam_stats::stats<stat_e::MetersMoved> += offset_t::distance<f32>(source_position, target_position);
		}
	}
} // namespace necrowarp
