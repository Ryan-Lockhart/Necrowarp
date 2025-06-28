#pragma once

#include <necrowarp/entities/good/ranger.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t ranger_t::think() const noexcept {
		const offset_t::product_t current_distance{ ranger_goal_map<MapType>.at(position) };

		const bool in_range{ ranger_t::in_range(current_distance) };

		if (can_loose() && in_range) {
			const std::optional<offset_t> maybe_position{ entity_registry<MapType>.dependent nearest<distance_function_e::Chebyshev, ALL_EVIL>(position) };

			if (!maybe_position.has_value()) {
				return command_pack_t{ command_e::None };
			}

			if (game_map<MapType>.linear_blockage(position, maybe_position.value(), cell_e::Solid, ranger_t::MaximumRange)) {
				cauto approach_pos{ good_goal_map<MapType>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

				if (!approach_pos.has_value()) {
					return command_pack_t{ command_e::None };
				}

				return command_pack_t{ command_e::Move, position, approach_pos.value() };
			}

			const entity_e target { determine_target<ranger_t>(entity_registry<MapType>.at(maybe_position.value())) };

			if (target == entity_e::None) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Loose, position, maybe_position.value() };
		} else if (can_nock() && (in_range || current_distance > ranger_t::MaximumRange)) {
			return command_pack_t{ command_e::Nock, position };
		} else if (!can_loose() && !can_nock() && !object_registry<MapType>.dependent empty<arrow_t>()) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (!object_registry<MapType>.dependent contains<arrow_t>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Retrieve, position, current_position };
			}

			cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!descent_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		} else if (entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_EVIL>(position) || (!can_nock() && !can_loose() && object_registry<MapType>.dependent empty<arrow_t>())) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (entity_registry<MapType>.dependent empty<ALL_EVIL>(current_position)) {
					continue;
				}

				return command_pack_t{ command_e::Clash, position, current_position };
			}

			cauto descent_pos{ good_goal_map<MapType>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!descent_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, descent_pos.value() };
		}

		if (current_distance > OptimalRange) {
			if (get_ammunition() < ranger_t::QuiverCapacity / 2 && !object_registry<MapType>.dependent empty<arrow_t>()) {
				for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
					const offset_t current_position{ position + offset };

					if (!object_registry<MapType>.dependent contains<arrow_t>(current_position)) {
						continue;
					}

					return command_pack_t{ command_e::Retrieve, position, current_position };
				}

				cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

				if (!descent_pos.has_value()) {
					return command_pack_t{ command_e::None };
				}

				return command_pack_t{ command_e::Move, position, descent_pos.value() };
			}

			cauto approach_pos{ good_goal_map<MapType>.dependent descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!approach_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, approach_pos.value() };
		} else if (current_distance < OptimalRange) {
			cauto retreat_pos{ good_goal_map<MapType>.dependent ascend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!retreat_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, retreat_pos.value() };
		}

		return command_pack_t{ command_e::None };
	}

	template<map_type_e MapType> inline void ranger_t::die() noexcept {
		object_registry<MapType>.spill(skull_t{ position });
		object_registry<MapType>.spill(flesh_t{ position });

		if (has_ammunition()) {
			for (usize i{ 0 }; i < get_ammunition(); ++i) {
				object_registry<MapType>.spill(arrow_t{ position });
			}
		}

		spill_fluid<MapType>(position, fluid_type<ranger_t>::type);

		player.receive_death_boon<ranger_t>();

		++steam_stats::stats<steam_stat_e::RangersSlain, i32>;

		scorekeeper.add(entity_e::Ranger);
	}
} // namespace necrowarp
