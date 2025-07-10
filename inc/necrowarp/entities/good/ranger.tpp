#pragma once

#include <necrowarp/entities/good/ranger.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t ranger_t::think(offset_t position) const noexcept {
		const f32 current_distance{ std::ceil(ranger_goal_map<MapType>.at(position)) };

		const bool in_range{ ranger_t::in_range(current_distance) };

		if (can_loose() && in_range) {
			const std::optional<offset_t> maybe_position{ entity_registry<MapType>.dependent nearest<distance_function_e::Chebyshev, ALL_EVIL>(position) };

			if (!maybe_position.has_value()) {
				return command_pack_t{ command_e::None };
			}

			if (game_map<MapType>.linear_blockage(position, maybe_position.value(), cell_e::Solid, ranger_t::MaximumRange)) {
				cauto approach_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

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

			cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

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

			cauto descent_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

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

				cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

				if (!descent_pos.has_value()) {
					return command_pack_t{ command_e::None };
				}

				return command_pack_t{ command_e::Move, position, descent_pos.value() };
			}

			cauto approach_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (!approach_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, approach_pos.value() };
		} else if (current_distance < OptimalRange) {
			cauto retreat_pos{ good_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (!retreat_pos.has_value()) {
				return command_pack_t{ command_e::None };
			}

			return command_pack_t{ command_e::Move, position, retreat_pos.value() };
		}

		return command_pack_t{ command_e::None };
	}

	template<map_type_e MapType> inline void ranger_t::die(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, flesh_t{});
		object_registry<MapType>.spill(position, cerebra_t{ entity_e::Ranger });

		if (has_ammunition()) {
			i8 dropped_ammunition{ get_ammunition() };

			if (nocked) {
				++dropped_ammunition;
			}

			if (object_registry<MapType>.dependent contains<arrow_t>(position)) {
				ptr<arrow_t> maybe_arrow{ object_registry<MapType>.dependent at<arrow_t>(position) };

				if (maybe_arrow != nullptr) {
					const i8 stack_capacity{ static_cast<i8>(arrow_t::MaximumArrows - maybe_arrow->stack_size()) };

					if (dropped_ammunition <= stack_capacity) {
						(*maybe_arrow) += dropped_ammunition;
						dropped_ammunition = 0;
					} else {
						(*maybe_arrow) += stack_capacity;
						dropped_ammunition -= stack_capacity;
					}
				}
			}

			if (dropped_ammunition > 0) {
				object_registry<MapType>.spill(position, arrow_t{ dropped_ammunition });
			}
		}

		player.receive_death_boon<ranger_t>();

		++steam_stats::stats<steam_stat_e::RangersSlain>;

		scorekeeper.add(entity_e::Ranger);
	}
} // namespace necrowarp
