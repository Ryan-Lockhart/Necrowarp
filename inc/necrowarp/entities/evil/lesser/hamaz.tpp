#pragma once

#include <necrowarp/entities/evil/lesser/hamaz.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/patronage.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t hamaz_t::think(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t current_position{ position + offset };

			if (!entity_registry<MapType>.dependent contains<ALL_NON_EVIL>(current_position)) {
				continue;
			}

			return command_pack_t{ command_e::Clash, position, current_position };
		}

		cauto doppleganger_pos{
			magic_enum::enum_switch([&](auto val) -> std::optional<offset_t> {
				constexpr entity_e cval{ val };

				using entity_type = typename to_entity_type<cval>::type;

				if constexpr (is_good<entity_type>::value) {
					return entity_goal_map<MapType, entity_type>.dependent descend<region_e::Interior>(position, entity_registry<MapType>);
				} else {
					return std::nullopt;
				}
			}, entity)
		};

		if (doppleganger_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, doppleganger_pos.value() };
		}

		if (cauto good_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; good_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, good_pos.value() };
		}

		if (cauto neutral_pos{ neutral_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; neutral_pos.has_value()) {
			return command_pack_t{ command_e::Move, position, neutral_pos.value() };
		}

		return command_pack_t{ command_e::Wander, position };
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> hamaz_t::die(offset_t position) noexcept {
		if constexpr (Death != death_e::Crushed || Death != death_e::Eradicated) {
			reincarnate<MapType>(position);
		}

		return death_info_t<Death>{ true };
	}

	template<map_type_e MapType> inline bool hamaz_t::reincarnate(offset_t position) noexcept {
		if (get_patron_disposition(player.patron) != disposition_e::Sadistic && !reincarnation_dis(random_engine)) {
			return false;
		}

		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr entity_e cval{ val };

			using entity_type = typename to_entity_type<cval>::type;

			if constexpr (is_good<entity_type>::value) {
				return entity_registry<MapType>.dependent add<true>(position, entity_type{});
			} else {
				return false;
			}			
		}, entity);
	}
} // namespace necrowarp
