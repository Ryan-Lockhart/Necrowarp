#pragma once

#include <necrowarp/entities/good/adventurer.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline void adventurer_t::embolden(offset_t position) noexcept {
		if (has_shattered() || affliction == AfflictionCeiling) {
			return;
		}

		const bool embolden_amped{ entity_goal_map<MapType, banner_bearer_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(position) <= banner_bearer_t::EffectRadius };

		if (!std::bernoulli_distribution{ BaseEmboldenChance * (embolden_amped ? banner_bearer_t::EmboldenMultiplier : 1.0f) }(random_engine)) {
			return;
		}

		affliction = static_cast<affliction_e>(static_cast<i8>(affliction) + 1);
	}

	template<map_type_e MapType> inline void adventurer_t::demoralize(offset_t position) noexcept {
		if (has_shattered() && heart_attack_dis(random_engine)) {
			const death_info_t<death_e::Killed> info{ die<MapType, death_e::Killed>(position) };

			if (!info.perished) {
				return;
			}
			
			entity_registry<MapType>.dependent remove<adventurer_t>(position);

			return;
		}

		if (affliction == AfflictionFloor) {
			shattered = shatter(random_engine, affliction);

			return;
		}

		const bool demoralize_softened{ entity_goal_map<MapType, mist_lady_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(position) <= mist_lady_t::EffectRadius };

		if (!std::bernoulli_distribution{ BaseDemoralizeChance * (demoralize_softened ? mist_lady_t::DemoralizeMultiplier : 1.0f) }(random_engine)) {
			return;
		}

		affliction = static_cast<affliction_e>(static_cast<i8>(affliction) - 1);

		shattered = shatter(random_engine, affliction);
	}

	template<map_type_e MapType> inline command_pack_t adventurer_t::think(offset_t position) const noexcept {
		if (has_shattered()) {
			if (object_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ladder_t>(position)) {
				for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
					const offset_t current_position{ position + offset };

					if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || object_registry<MapType>.dependent empty<ladder_t>(current_position)) {
						continue;
					}

					cptr<ladder_t> maybe_ladder{ object_registry<MapType>.dependent at<ladder_t>(current_position) };

					if (maybe_ladder == nullptr || maybe_ladder->is_down_ladder() || maybe_ladder->has_shackle()) {
						continue;
					}

					return command_pack_t{ command_e::Depart, position, current_position };
				}
			}

			if (cauto departure_pos{ departure_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; departure_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, departure_pos.value() };
			}

			cauto flee_pos{ non_good_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (!flee_pos.has_value()) {
				return command_pack_t{ command_e::Wander, position };
			}

			return command_pack_t{ command_e::Move, position, flee_pos.value() };
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || game_map<MapType>[current_position].solid || entity_registry<MapType>.dependent empty<ALL_NON_GOOD>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		if (cauto paladin_pos{ entity_goal_map<MapType, paladin_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; paladin_pos.has_value() && entity_goal_map<MapType, paladin_t>[paladin_pos.value()] > MaxLeashRange) {
			return command_pack_t{ command_e::Move, position, paladin_pos.value() };
		}

		if (cauto mercenary_pos{ entity_goal_map<MapType, mercenary_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; mercenary_pos.has_value() && entity_goal_map<MapType, mercenary_t>[mercenary_pos.value()] > MaxLeashRange) {
			return command_pack_t{ command_e::Move, position, mercenary_pos.value() };
		}

		if (cauto evil_pos{ evil_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; evil_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, evil_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> adventurer_t::die(offset_t position) noexcept {
		++steam_stats::stats<stat_e::AdventurersSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Adventurer });
	
			medicus_goals_dirty = true;
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Adventurer });
		}

		player.receive_death_boon<adventurer_t>();

		scorekeeper.add(entity_e::Adventurer);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
