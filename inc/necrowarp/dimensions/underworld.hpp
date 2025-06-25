#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<> inline void game_s::plunge<dimension_e::Underworld>() noexcept {
		constexpr map_type_e map_type = map_type_e::Standard;
#if !defined(STEAMLESS)
		steam_stats::store();

		stat_store_timer.record();
#endif
		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		game_map<map_type>
			.template set<zone_region_e::Border>(closed_state)
			.template generate<zone_region_e::Interior>(
				random_engine,
				globals::map_config.fill_percent,
				globals::map_config.automata_iterations,
				globals::map_config.automata_threshold,
				cell_applicator
			)
			.template collapse<zone_region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

		std::vector<area_t> areas{ area_t::partition(game_map<map_type>, cell_e::Open) };

		if (areas.size() > 1) {
			cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

			for (crauto area : areas) {
				if (area != largest_area) {
					area.apply(game_map<map_type>, cell_e::Solid);
				}
			}
		}

		for (offset_t::scalar_t y{ 0 }; y < globals::MapSize<map_type>.h; ++y) {
			for (offset_t::scalar_t x{ 0 }; x < globals::MapSize<map_type>.w; ++x) {
				const offset_t pos{ x, y };

				game_map<map_type>[pos].recalculate_index(game_map<map_type>, pos, cell_e::Solid);
			}
		}

		cauto player_pos{ game_map<map_type>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

		if (!player_pos.has_value()) {
			error_log.add("could not find open position for player!");
			terminate_prematurely();
		}

		player.position = player_pos.value();

		entity_goal_map<map_type, player_t>.add(player.position);

		good_goal_map<map_type>.add(player.position);
		skulker_goal_map<map_type>.add(player.position);

		cauto portal_pos{ game_map<map_type>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open) };

		if (!portal_pos.has_value()) {
			error_log.add("could not find open position for return portal!");
			terminate_prematurely();
		}

		object_registry<map_type>.add(portal_t{ portal_pos.value(), stability_e::Insightful });

		object_registry<map_type>.template spawn<ladder_t>(
			static_cast<usize>(globals::map_config.number_of_up_ladders),
			static_cast<u32>(globals::map_config.minimum_ladder_distance),

			verticality_e::Up
		);

		object_registry<map_type>.template spawn<ladder_t>(
			static_cast<usize>(globals::map_config.number_of_down_ladders),
			static_cast<u32>(globals::map_config.minimum_ladder_distance),

			verticality_e::Down, random_engine
		);

		object_registry<map_type>.template spawn<skull_t>(
			static_cast<usize>(globals::map_config.starting_skulls),
			static_cast<u32>(globals::map_config.minimum_skull_distance),

			decay_e::Animate
		);
	}
} // namespace necrowarp
