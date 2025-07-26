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
				const entity_e target{ determine_target<thetwo_t>(entity_registry<MapType>.at(current_position)) };

				if (!can_devour(target) || !is_hungry) {
					return command_pack_t{ command_e::Clash, position, current_position };
				} else {
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

			cauto meal_pos{ object_goal_map<MapType, flesh_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (meal_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, meal_pos.value() };
			}
		}

		cauto descent_pos{ non_neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		if (is_skittish || is_injured) {
			const i8 distance{ static_cast<i8>(std::round(non_neutral_goal_map<MapType>.at(descent_pos.value()))) };

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

	template<map_type_e MapType> inline void thetwo_t::killed(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::ThetwoSlain>;

		const u8 droppings{ get_droppings() };

		object_registry<MapType>.spill(position, bones_t{}, droppings);
		object_registry<MapType>.spill(position, flesh_t{}, droppings * 2);

		player.receive_death_boon<thetwo_t>(droppings);

		scorekeeper.add(entity_e::Thetwo, droppings);
	}

	template<map_type_e MapType> inline i8 thetwo_t::devoured(offset_t position) noexcept {
		++steam_stats::stats<steam_stat_e::ThetwoSlain>;

		const u8 droppings{ get_droppings() };

		object_registry<MapType>.spill(position, bones_t{}, droppings);

		player.receive_death_boon<thetwo_t>(droppings);

		scorekeeper.add(entity_e::Thetwo, droppings);

		return droppings * 2;
	}
} // namespace necrowarp
