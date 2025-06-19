#pragma once

#include <necrowarp/entities/good/ranger.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t ranger_t::think() const noexcept {
		const f32 current_distance{ ranger_goal_map<MapType>.at(position) };

		const bool in_range{ between<f32>(current_distance, MinimumRange, MaximumRange) };

		if (can_loose() && in_range) {
			const std::optional<offset_t> maybe_position{ entity_registry<MapType>.template nearest<distance_function_e::Chebyshev, ALL_EVIL>(position) };

			if (!maybe_position.has_value()) {
				return command_pack_t{ command_e::None };
			}

			const entity_e target { determine_target<ranger_t>(entity_registry<MapType>.at(maybe_position.value())) };

			if (target == entity_e::None) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Loose, position, maybe_position.value() };
		} else if (can_knock() && in_range) {
			return command_pack_t{ command_e::Knock, position };
		} else if (!can_knock() && !object_registry<MapType>.template empty<arrow_t>()) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (!object_registry<MapType>.template contains<arrow_t>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Retrieve, position, current_position };
			}

			cauto descent_pos{ object_goal_map<MapType, arrow_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!descent_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		} else if (!can_knock() && !can_loose() && object_registry<MapType>.template empty<arrow_t>()) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (!entity_registry<MapType>.template contains<ALL_EVIL>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Clash, position, current_position };
			}

			cauto descent_pos{ good_goal_map<MapType>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!descent_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		if (current_distance > MaximumRange) {
			cauto approach_pos{ ranger_goal_map<MapType>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!approach_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, approach_pos.value() };
		} else if (current_distance < MinimumRange) {
			cauto retreat_pos{ ranger_goal_map<MapType>.template ascend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!retreat_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, retreat_pos.value() };
		}

		return command_pack_t{ command_e::None };
	}

	template<map_type_e MapType> inline void ranger_t::die() noexcept {
		object_registry<MapType>.template add<true>(skull_t{ position });
		object_registry<MapType>.template add<true>(flesh_t{ position });

		if (has_ammunition()) {
			object_registry<MapType>.template add<true>(arrow_t{ position });
		}

		fluid_map<MapType>[position] += fluid_type<ranger_t>::type;

		player.receive_death_boon<ranger_t>();

		++steam_stats::stats<steam_stat_e::RangersSlain, i32>;

		scorekeeper.add(entity_e::Ranger);
	}
} // namespace necrowarp
