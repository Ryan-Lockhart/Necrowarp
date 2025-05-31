#pragma once

#include <necrowarp/entities/good/priest.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t priest_t::think() const noexcept {
		if (!can_resurrect() && !can_anoint() && object_registry.empty<skull_t>()) {
			cauto flock_to_paladin_pos{ entity_goal_map<paladin_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (flock_to_paladin_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_paladin_pos.value() };
			}
			cauto flock_to_mercenary_pos{ entity_goal_map<mercenary_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (flock_to_mercenary_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_mercenary_pos.value() };
			}

			cauto flock_to_adventurer_pos { entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (flock_to_adventurer_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_adventurer_pos.value() };
			}

			cauto flee_from_evil_pos{ good_goal_map.ascend<zone_region_t::Interior>(position, entity_registry) };

			if (flee_from_evil_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flee_from_evil_pos.value() };
			}
			
			return command_pack_t{ command_e::Suicide, position };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t offset_pos{ position + offset };

			if (object_registry.contains<skull_t>(offset_pos)) {
				cauto skull{ object_registry.at<skull_t>(offset_pos) };

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
			} else if (entity_registry.contains<adventurer_t>(offset_pos)) {
				if (!can_anoint()) {
					continue;
				}

				return command_pack_t{ command_e::Anoint, position, offset_pos };
			}
		}

		cauto descent_pos = [&]() -> std::optional<offset_t> {
			cauto skull_pos = object_goal_map<skull_t>.descend<zone_region_t::Interior>(position, entity_registry);

			cauto adventurer_pos = entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry);

			if (skull_pos.has_value() && adventurer_pos.has_value()) {
				cauto skull_distance{ object_goal_map<skull_t>.at(skull_pos.value()) };
				cauto adventurer_distance{ entity_goal_map<adventurer_t>.at(adventurer_pos.value()) };

				return skull_distance <= adventurer_distance ? skull_pos : adventurer_pos;
			} else if (skull_pos.has_value()) {
				return skull_pos;
			} else if (adventurer_pos.has_value()) {
				return adventurer_pos;
			}

			cauto flee_from_evil_pos{ good_goal_map.ascend<zone_region_t::Interior>(position, entity_registry) };

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

	inline void priest_t::die() noexcept {
		object_registry.add<true>(skull_t{ position });

		fluid_map[position] += fluid_type<priest_t>::type;

		++steam_stats::stats<steam_stat_e::PriestsSlain, i32>;
	}
} // namespace necrowarp
