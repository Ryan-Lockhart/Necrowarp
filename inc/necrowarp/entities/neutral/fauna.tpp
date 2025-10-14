#pragma once

#include <necrowarp/entities/neutral/fauna.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline bool fauna_t::defecate(offset_t position) noexcept {
		if (!can_defecate()) {
			return false;
		}

		spill_fluid<MapType>(position, fluid_e::Filth, max<usize>(static_cast<usize>(bowels * BowelsRatio), 1));

		set_bowels(0);

		return true;
	}

	template<map_type_e MapType, RandomEngine Generator> inline bool fauna_t::defecate(offset_t position, ref<Generator> engine) noexcept {
		if (!can_defecate() || !std::bernoulli_distribution{ bowels * DefecationChancePerBowels }(engine)) {
			return false;
		}

		spill_fluid<MapType>(position, crimson_shart_dis(engine) ? fluid_e::BloodyFilth : fluid_e::Filth, max<usize>(static_cast<usize>(bowels * BowelsRatio), 1));

		set_bowels(0);

		return true;
	}

	template<map_type_e MapType> inline command_pack_t fauna_t::think(offset_t position) const noexcept {
		const bool is_hungry{ protein < max_protein() };
		
		const bool threat_nearby{
			entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ALL_NON_NEUTRAL>(position) ||
			entity_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, thetwo_t>(position)
		};

		if (threat_nearby) {
			cauto flee_pos{ non_neutral_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

			if (flee_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flee_pos.value() };
			}
		}

		if (!object_registry<MapType>.dependent empty<flora_t>() && is_hungry) {
			if (object_registry<MapType>.dependent contains<flora_t>(position)) {
				return command_pack_t{ command_e::Graze, position, position };
			}

			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t current_position{ position + offset };

				if (object_registry<MapType>.dependent contains<flora_t>(current_position)) {
					return command_pack_t{ command_e::Graze, position, current_position };
				}
			}

			cauto flora_pos{ object_goal_map<MapType, flora_t>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

			if (flora_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, flora_pos.value() };
			}
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> fauna_t::die(offset_t position) noexcept {
		const i8 droppings{ max<i8>(protein_value(), 1) };

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{}, droppings);
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
		}

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, droppings };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
