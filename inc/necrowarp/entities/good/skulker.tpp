#pragma once

#include <necrowarp/entities/good/skulker.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t skulker_t::think(offset_t position) const noexcept {
		concealment = skulker_t::determine_concealment(skulker_goal_map<MapType>.at(position));

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent empty<ALL_NON_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto descent_pos{
			[&]() -> std::optional<offset_t> {
				if (concealment == concealment_e::Imperceptible) {
					return entity_goal_map<MapType, player_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>);
				} else {
					return evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>);
				}
			}()
		};

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::Wander, position };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> skulker_t::die(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::SkulkersSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Skulker });
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Skulker });
		}

		player.receive_death_boon<skulker_t>();

		scorekeeper.add(entity_e::Skulker);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
