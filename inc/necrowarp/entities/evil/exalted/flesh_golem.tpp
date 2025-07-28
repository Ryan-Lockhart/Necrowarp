#pragma once

#include <necrowarp/entities/evil/exalted/flesh_golem.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	constexpr bool flesh_golem_t::can_devour(entity_e entity) const noexcept {
		switch (entity) {
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

	template<map_type_e MapType> inline bool flesh_golem_t::can_devour(offset_t position) const noexcept {
		const object_e target_object{ determine_target<flesh_golem_t>(object_registry<MapType>.at(position)) };
		const entity_e target_entity{ determine_target<flesh_golem_t>(entity_registry<MapType>.at(position)) };

		if (!can_devour(target_object) && !can_devour(target_entity) && target_entity == entity_e::Thetwo) {
			ptr<thetwo_t> maybe_thetwo{ entity_registry<MapType>.dependent at<thetwo_t>(position) };

			return maybe_thetwo != nullptr && can_devour(maybe_thetwo->get_bulk());
		}

		return can_devour(target_object) || can_devour(target_entity);
	}

	template<map_type_e MapType> inline command_pack_t flesh_golem_t::think(offset_t position) const noexcept {
		const bool threat_nearby{ entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_EVIL>(position) };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				if (can_devour<MapType>(position)) {
					return command_pack_t{ command_e::Devour, position, current_position };
				} else {
					return command_pack_t{ command_e::Clash, position, current_position };
				}
			} else if (!threat_nearby && object_registry<MapType>.dependent contains<flesh_t>(current_position)) {
				return command_pack_t{ command_e::Devour, position, current_position };
			}
		}

		if (cauto good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; good_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, good_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		if (cauto flesh_pos{ object_goal_map<MapType, flesh_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; flesh_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, flesh_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> flesh_golem_t::die(offset_t position) noexcept {
		const i8 droppings{ static_cast<i8>(ProteinValue + protein * ProteinRatio) };

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, flesh_t{}, droppings);
		}

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, droppings };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
