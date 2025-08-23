#pragma once

#include <necrowarp/commands/unary/calamitous_retaliation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, calamitous_retaliation_t>::process() const noexcept {
		if (!player.can_perform<MapType>(grimoire_e::CalamitousRetaliation)) {
			player_turn_invalidated = true;

			return;
		}

		magic_enum::enum_for_each<entity_e>([](auto val) {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::Player) {
				using entity_type = typename to_entity_type<cval>::type;

				entity_registry<MapType>.dependent execute<entity_type>([&](offset_t position, rauto entity) -> void {
					entity.dependent die<MapType, death_e::Eradicated>(position);

					entity_registry<MapType>.dependent remove<entity_type>(position);
					
					++game_stats.player_kills;
					++steam_stats::stats<stat_e::PlayerKills>;
				});
			}
		});

		++steam_stats::stats<stat_e::CalamitousRetaliations>;

		player.pay_cost<MapType>(grimoire_e::CalamitousRetaliation);

		literature::use(grimoire_e::CalamitousRetaliation);

		descent_flag = true;

		++steam_stats::stats<stat_e::MetersMoved>;
	}
} // namespace necrowarp
