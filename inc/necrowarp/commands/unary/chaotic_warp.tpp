#pragma once

#include <necrowarp/commands/unary/chaotic_warp.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline bool chaotic_warp_t::execute(offset_t position, bool free) noexcept {
		if (!free && !player.can_perform(discount_e::ChaoticWarp)) {
			return false;
		}

		if (!free) {
			player.pay_cost(discount_e::ChaoticWarp);
		}
		
		cauto random_safe_position{ evil_goal_map<MapType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>, 8) };

		if (!random_safe_position.has_value()) {
			cauto random_unsafe_position{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>) };

			if (!random_unsafe_position.has_value()) {
				if (!free) {
					player.receive_failed_warp_boon();
				}

				warped_from = std::nullopt;

				return false;
			}

			if (!free) {
				player.receive_unsafe_warp_boon();
			}

			entity_registry<MapType>.dependent update<player_t>(position, random_unsafe_position.value());

			++steam_stats::stats<steam_stat_e::RandomWarps>;

			steam_stats::stats<steam_stat_e::MetersWarped> += offset_t::distance<f32>(position, player.position);

			warp_cursor<MapType>.set(player.position);
			warped_from = position;

			return true;
		}

		entity_registry<MapType>.dependent update<player_t>(position, random_safe_position.value());

		++steam_stats::stats<steam_stat_e::RandomWarps>;

		steam_stats::stats<steam_stat_e::MetersWarped> += offset_t::distance<f32>(position, player.position);

		warp_cursor<MapType>.set(player.position);
		warped_from = position;

		return true;
	}

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, chaotic_warp_t>::process() const noexcept {
		chaotic_warp_t::execute<MapType>(source_position);
	}
} // namespace necrowarp
