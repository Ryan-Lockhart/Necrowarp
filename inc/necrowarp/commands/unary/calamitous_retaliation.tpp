#pragma once

#include <necrowarp/commands/unary/calamitous_retaliation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, calamitous_retaliation_t>::process() const noexcept {
		if (!player.can_perform(discount_e::CalamitousRetaliation)) {
			player_turn_invalidated = true;

			return;
		}

		++steam_stats::stats<steam_stat_e::CalamitousRetaliations>;

		player.pay_cost(discount_e::CalamitousRetaliation);

		magic_enum::enum_for_each<entity_e>([](auto val) {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None && cval != entity_e::Player) {
				using entity_type = typename to_entity_type<cval>::type;

				entity_registry<MapType>.dependent execute<entity_type>([&](offset_t position, rauto entity) -> void {
					entity.dependent die<MapType, death_e::Eradicated>(position);

					entity_registry<MapType>.dependent remove<entity_type>(position);
				});
			}
		});

		++steam_stats::stats<steam_stat_e::MetersMoved>;

		descent_flag = true;
	}
} // namespace necrowarp
