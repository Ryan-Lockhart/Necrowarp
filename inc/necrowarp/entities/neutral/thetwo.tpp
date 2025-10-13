#pragma once

#include <necrowarp/entities/neutral/thetwo.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<> inline bool thetwo_t::can_devour<bulk_e::Titanic>(entity_e entity) const noexcept {
		switch (entity) {
			case entity_e::Player: {
				return !player.no_hit_enabled() && !player.has_ascended();
			} case entity_e::Abomination:
			  case entity_e::Skulker:
			  case entity_e::Ranger:
			  case entity_e::MistLady:
			  case entity_e::BannerBearer:
			  case entity_e::Adventurer:
			  case entity_e::Mercenary:
			  case entity_e::BattleMonk: {
				return true;
			} default: {
				return false;
			}
		}
	}

	inline bool thetwo_t::can_devour(entity_e entity) const noexcept {
		if (protein >= max_protein()) {
			return false;
		}
		
		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr bulk_e cval{ val };

			return can_devour<cval>(entity);
		}, bulk);
	}

	template<map_type_e MapType> inline command_pack_t thetwo_t::think(offset_t position) const noexcept {
		if (can_shed() || is_shedding()) {
			return command_pack_t{ command_e::Shed, position };
		}

		const bool is_healthy{ health > static_cast<i8>(max_health() * 0.75) };
		const bool is_injured{ !is_healthy && health < static_cast<i8>(max_health() * 0.25) };

		const bool is_hungry{ protein < static_cast<i8>(max_protein() * 0.50) };
		
		const bool threat_nearby{ entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_NEUTRAL>(position) };

		const bool is_skittish{ bulk == bulk_e::Neonatal || bulk == bulk_e::Young || bulk == bulk_e::Mature };

		const bool is_dauntless{ bulk == bulk_e::Gross || bulk == bulk_e::Titanic };

		const bool is_growing{ bulk != bulk_e::Titanic };

		if (threat_nearby && (is_skittish || is_injured)) {
			cauto flee_pos{ non_neutral_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (flee_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flee_pos.value() };
			}
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (object_registry<MapType>.dependent contains<flesh_t>(current_position) && (!threat_nearby || is_hungry)) {
				return command_pack_t{ command_e::Devour, position, current_position };
			}

			if (entity_registry<MapType>.dependent contains<ALL_NON_NEUTRAL>(current_position)) {
				if (!is_hungry) {
					return command_pack_t{ command_e::Clash, position, current_position };
				}

				const std::optional<entity_e> maybe_target{ entity_registry<MapType>.at(current_position) };

				if (maybe_target.has_value() && can_devour(maybe_target.value())) {
					return command_pack_t{ command_e::Devour, position, current_position };
				}
			}
		}

		if (can_metabolise()) {
			return command_pack_t{ command_e::Metabolise, position };
		}

		const bool is_hunting{ is_dauntless && is_healthy && !entity_registry<MapType>.dependent empty<flesh_golem_t>() };

		if (is_hunting) {
			cauto stalk_pos{ entity_goal_map<MapType, flesh_golem_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (stalk_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, stalk_pos.value() };
			}
		}

		if (!object_registry<MapType>.dependent empty<flesh_t>() && (is_growing || is_hungry)) {
			if (object_registry<MapType>.dependent contains<flesh_t>(position)) {
				return command_pack_t{ command_e::Devour, position, position };
			}

			cauto flesh_pos{ object_goal_map<MapType, flesh_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (flesh_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flesh_pos.value() };
			}
		}

		cauto descent_pos{ non_neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		if (is_skittish || is_injured) {
			cauto maybe_distance{ non_neutral_goal_map<MapType>.at(descent_pos.value()) };

			if (!maybe_distance.has_value()) {

			}

			const i8 distance{ static_cast<i8>(std::floor(maybe_distance.value())) };

			if (distance < SkittishApproachDistance) {
				cauto flee_pos{ non_neutral_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

				if (!flee_pos.has_value()) {
					return command_pack_t{ command_e::None };
				}

				return command_pack_t{ command_e::Move, position, flee_pos.value() };
			} else if (distance == SkittishApproachDistance) {
				return command_pack_t{ command_e::None };
			}
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> thetwo_t::die(offset_t position) noexcept {
		++steam_stats::stats<stat_e::ThetwoSlain>;

		const i8 droppings{ protein_value() };

		if (bulk != bulk_e::Neonatal) {
			if constexpr (Death == death_e::Killed) {
				object_registry<MapType>.spill(position, bones_t{});
				object_registry<MapType>.spill(position, flesh_t{}, droppings);
			} else if constexpr (Death == death_e::Devoured) {
				object_registry<MapType>.spill(position, bones_t{});
			}

			player.receive_death_boon<thetwo_t>(droppings);

			scorekeeper.add(entity_e::Thetwo, droppings);
		}

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, droppings };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
