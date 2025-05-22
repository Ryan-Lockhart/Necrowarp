#pragma once

#include <necrowarp/entities/animate/good/priest.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline command_pack_t priest_t::think() const noexcept {
		if (!has_piety()) {
			cauto flock_to_paladin_pos{ entity_goal_map<paladin_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_paladin_pos.has_value() && entity_registry.empty<adventurer_t>()) {
				return command_pack_t{ command_e::None };
			} else if (flock_to_paladin_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flock_to_paladin_pos.value() };
			}

			cauto flock_to_adventurer_pos { entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_adventurer_pos.has_value()) {
				cauto flee_from_evil_pos{ good_goal_map.ascend<zone_region_t::Interior>(position, entity_registry) };

				if (!flee_from_evil_pos.has_value()) {
					return command_pack_t{ command_e::Suicide, position };
				}

				return command_pack_t{ command_e::Move, position, flee_from_evil_pos.value() };
			}

			return command_pack_t{ command_e::Move, position, flock_to_adventurer_pos.value() };
		}

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t offset_pos{ position + offset };

			if (entity_registry.contains<skull_t>(offset_pos)) {
				cauto skull{ entity_registry.at<skull_t>(offset_pos) };

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
			cauto skull_pos = entity_goal_map<skull_t>.descend<zone_region_t::Interior>(position, entity_registry);

			cauto adventurer_pos = entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry);

			if (skull_pos.has_value() && adventurer_pos.has_value()) {
				cauto skull_distance{ entity_goal_map<skull_t>.at(skull_pos.value()) };
				cauto adventurer_distance{ entity_goal_map<adventurer_t>.at(adventurer_pos.value()) };

				return skull_distance < adventurer_distance ? skull_pos : adventurer_pos;
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
		entity_registry.add<true>(skull_t{ position });

		fluid_map[position] += fluid_type<priest_t>::type;

		++steam_stats::stats<steam_stat_e::PriestsSlain, i32>;
	}
} // namespace necrowarp
