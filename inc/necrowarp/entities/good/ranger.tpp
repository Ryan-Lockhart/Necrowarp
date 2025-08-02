#pragma once

#include <necrowarp/entities/good/ranger.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t ranger_t::think(offset_t position) const noexcept {
		const f32 current_distance{ std::ceil(non_good_goal_map<MapType>.average(position)) };

		const bool in_range{ ranger_t::in_range(current_distance) };

		const bool ammo_nearby{ object_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, arrow_t>(position) };

		const bool on_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

		if (current_distance < MinimumRange) {
			cauto retreat_pos{ non_good_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (!retreat_pos.has_value()) {
				return command_pack_t{ command_e::Wander, position };
			}

			return command_pack_t{ command_e::Move, position, retreat_pos.value() };
		}

		if (on_ladder) {
			return command_pack_t{ command_e::Wander, position };
		}

		if (can_loose() && in_range) {
			const std::optional<offset_t> maybe_position{ entity_registry<MapType>.dependent nearest<distance_function_e::Chebyshev, ALL_NON_GOOD>(position) };

			if (!maybe_position.has_value()) {
				return command_pack_t{ command_e::Wander };
			}

			if (game_map<MapType>.linear_blockage(position, maybe_position.value(), cell_e::Solid, ranger_t::MaximumRange)) {
				cauto approach_pos{ non_good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

				if (!approach_pos.has_value()) {
					return command_pack_t{ command_e::Wander };
				}

				return command_pack_t{ command_e::Move, position, approach_pos.value() };
			}

			const entity_e target{ determine_target<ranger_t>(entity_registry<MapType>.at(maybe_position.value())) };

			if (target == entity_e::None) {
				return command_pack_t{ command_e::Wander };
			}

			return command_pack_t{ command_e::Loose, position, maybe_position.value() };
		}

		if (can_nock()) {
			return command_pack_t{ command_e::Nock, position };
		}

		if (!object_registry<MapType>.dependent empty<arrow_t>()) {
			if (ammo_nearby) {
				for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
					const offset_t current_position{ position + offset };

					if (!object_registry<MapType>.dependent contains<arrow_t>(current_position)) {
						continue;
					}

					return command_pack_t{ command_e::Retrieve, position, current_position };
				}
			}

			if (cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; descent_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, descent_pos.value() };
			}
		}

		if (cauto paladin_pos{ entity_goal_map<MapType, paladin_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; paladin_pos.has_value() && entity_goal_map<MapType, paladin_t>[paladin_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, paladin_pos.value() };
		}

		if (cauto mercenary_pos{ entity_goal_map<MapType, mercenary_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; mercenary_pos.has_value() && entity_goal_map<MapType, mercenary_t>[mercenary_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, mercenary_pos.value() };
		}

		if (cauto adventurer_pos{ entity_goal_map<MapType, adventurer_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; adventurer_pos.has_value() && entity_goal_map<MapType, adventurer_t>[adventurer_pos.value()] > LeashRange) {
			return command_pack_t{ command_e::Move, position, adventurer_pos.value() };
		}

		if (current_distance > MaximumRange) {
			if (get_ammunition() < ranger_t::QuiverCapacity / 2 && !object_registry<MapType>.dependent empty<arrow_t>()) {
				if (ammo_nearby) {
					for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
						const offset_t current_position{ position + offset };

						if (!object_registry<MapType>.dependent contains<arrow_t>(current_position)) {
							continue;
						}

						return command_pack_t{ command_e::Retrieve, position, current_position };
					}
				}

				if (cauto descent_pos{ object_goal_map<MapType, arrow_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; descent_pos.has_value()) {
					return command_pack_t{ command_e::Move, position, descent_pos.value() };
				}
			}

			if (cauto evil_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; evil_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, evil_pos.value() };
			}

			if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, neutral_pos.value() };
			}
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> ranger_t::die(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::RangersSlain>;

		if constexpr (Death != death_e::Crushed || Death != death_e::Eradicated) {
			object_registry<MapType>.spill(position, bones_t{});
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
		}

		if constexpr (Death != death_e::Devoured) {
			object_registry<MapType>.spill(position, flesh_t{});
		}

		player.receive_death_boon<ranger_t>();

		scorekeeper.add(entity_e::Ranger);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
