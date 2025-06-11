#pragma once

#include <necrowarp/entities/good/priest.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t priest_t::think() const noexcept {
		if (!can_resurrect() && !can_anoint() && object_registry<MapType>.template empty<skull_t>()) {
			cauto flock_to_paladin_pos{ entity_goal_map<MapType, paladin_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (flock_to_paladin_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_paladin_pos.value() };
			}
			cauto flock_to_mercenary_pos{ entity_goal_map<MapType, mercenary_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (flock_to_mercenary_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_mercenary_pos.value() };
			}

			cauto flock_to_adventurer_pos { entity_goal_map<MapType, adventurer_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (flock_to_adventurer_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_adventurer_pos.value() };
			}

			cauto flee_from_evil_pos{ good_goal_map<MapType>.template ascend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (flee_from_evil_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flee_from_evil_pos.value() };
			}
			
			return command_pack_t{ command_e::Suicide, position };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t offset_pos{ position + offset };

			if (object_registry<MapType>.template contains<skull_t>(offset_pos)) {
				cauto skull{ object_registry<MapType>.template at<skull_t>(offset_pos) };

				if (skull == nullptr) {
					continue;
				}

				if (!can_resurrect() && skull->state == decay_e::Fresh) {
					continue;
				}

				if (skull->state == decay_e::Fresh && can_resurrect()) {
					return command_pack_t{ command_e::Resurrect, position, offset_pos };
				} else if (skull->state != decay_e::Fresh) {
					return command_pack_t{ command_e::Exorcise, position, offset_pos }; 
				}
			} else if (entity_registry<MapType>.template contains<adventurer_t>(offset_pos)) {
				if (!can_anoint()) {
					continue;
				}

				return command_pack_t{ command_e::Anoint, position, offset_pos };
			}
		}

		cauto descent_pos = [&]() -> std::optional<offset_t> {
			cauto skull_pos = object_goal_map<MapType, skull_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>);

			cauto adventurer_pos = entity_goal_map<MapType, adventurer_t>.template descend<zone_region_e::Interior>(position, entity_registry<MapType>);

			if (skull_pos.has_value() && adventurer_pos.has_value()) {
				cauto skull_distance{ object_goal_map<MapType, skull_t>.at(skull_pos.value()) };
				cauto adventurer_distance{ entity_goal_map<MapType, adventurer_t>.at(adventurer_pos.value()) };

				return skull_distance <= adventurer_distance ? skull_pos : adventurer_pos;
			} else if (skull_pos.has_value()) {
				return skull_pos;
			} else if (adventurer_pos.has_value()) {
				return adventurer_pos;
			}

			cauto flee_from_evil_pos{ good_goal_map<MapType>.template ascend<zone_region_e::Interior>(position, entity_registry<MapType>) };

			if (!flee_from_evil_pos.has_value()) {
				return std::nullopt;
			}

			return flee_from_evil_pos;
		}();

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::Suicide, position };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void priest_t::die() noexcept {
		object_registry<MapType>.template add<true>(skull_t{ position });

		fluid_map<MapType>[position] += fluid_type<priest_t>::type;

		++steam_stats::stats<steam_stat_e::PriestsSlain, i32>;

		scorekeeper.add(entity_e::Priest);
	}
} // namespace necrowarp
