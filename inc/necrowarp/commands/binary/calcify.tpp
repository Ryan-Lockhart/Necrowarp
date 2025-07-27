#pragma once

#include <necrowarp/commands/binary/calcify.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, calcify_t>::process() const noexcept {
		if ((!game_map<MapType>[target_position].solid && object_registry<MapType>.dependent empty<bones_t>(target_position)) || !player.can_perform(discount_e::Calcify)) {
			player_turn_invalidated = true;

			return;
		}

		if (game_map<MapType>[target_position].solid) {
			game_map<MapType>[target_position].solid = false;

			object_registry<MapType>.add(target_position, bones_t{ decay_e::Animate });
		} else if (!object_registry<MapType>.dependent empty<bones_t>(target_position)) {
			object_registry<MapType>.remove(target_position);

			magic_enum::enum_for_each<entity_e>([&](auto val) {
				constexpr entity_e cval{ val };

				if constexpr (cval != entity_e::None) {
					using entity_type = typename to_entity_type<cval>::type;

					if constexpr (is_incorporeal<entity_type>::value && !is_incorporeal<entity_type>::conditional) {
						return;
					}

					ptr<entity_type> entity{ entity_registry<MapType>.dependent at<entity_type>(target_position) };

					if (entity == nullptr) {
						return;
					}

					if constexpr (is_incorporeal<entity_type>::value && is_incorporeal<entity_type>::conditional) {
						if (entity->is_incorporeal()) {
							return;
						}
					}

					if constexpr (is_player<entity_type>::value) {
						entity->dependent die<MapType, death_e::Crushed>();
					} else {
						entity->dependent die<MapType, death_e::Crushed>(target_position);

						entity_registry<MapType>.dependent remove<entity_type>(target_position);
					}
				}
			});

			game_map<MapType>[target_position].solid = true;
		}

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			cauto pos{ target_position + offset };

			game_map<MapType>[pos].recalculate_index(game_map<MapType>, pos, cell_e::Solid);
		}

		game_map<MapType>[target_position].recalculate_index(game_map<MapType>, target_position, cell_e::Solid);

		++steam_stats::stats<steam_stat_e::Calcifications>;

		player.pay_cost(discount_e::Calcify);
	}
} // namespace necrowarp
