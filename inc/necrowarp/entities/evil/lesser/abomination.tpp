#pragma once

#include <necrowarp/entities/evil/lesser/abomination.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline bool abomination_t::can_devour(offset_t position) const noexcept {
		const object_e target_object{ determine_target<abomination_t>(object_registry<MapType>.at(position)) };
		const entity_e target_entity{ determine_target<abomination_t>(entity_registry<MapType>.at(position)) };

		if (!can_devour(target_object) && !can_devour(target_entity) && target_entity == entity_e::Thetwo) {
			ptr<thetwo_t> maybe_thetwo{ entity_registry<MapType>.dependent at<thetwo_t>(position) };

			return maybe_thetwo != nullptr && can_devour(maybe_thetwo->get_bulk());
		}

		return can_devour(target_object) || can_devour(target_entity);
	}

	template<map_type_e MapType> inline bool abomination_t::can_mitose(offset_t position) const noexcept {
		return can_mitose() && game_map<MapType>.dependent nearby<region_e::Interior, distance_function_e::Manhattan>(position, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline command_pack_t abomination_t::think(offset_t position) const noexcept {
		if (can_mitose<MapType>(position)) {
			return command_pack_t{ command_e::Mitose, position };
		}

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

	template<map_type_e MapType> inline void abomination_t::mitose(offset_t position) noexcept {
		if (!can_mitose<MapType>(position)) {
			return;
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Manhattan>) {
			cauto current_pos{ position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(current_pos) || game_map<MapType>[current_pos].solid || !entity_registry<MapType>.empty(current_pos) || !entity_registry<MapType>.add(current_pos, abomination_t{})) {
				continue;
			}
			
			set_protein(0);

			return;
		}
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> abomination_t::die(offset_t position) noexcept {
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
