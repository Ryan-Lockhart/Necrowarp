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

		globals::randomize_map_config(random_engine);

		game_map<map_type>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::map_config.fill_percent,
				globals::map_config.automata_iterations,
				globals::map_config.automata_threshold,
				cell_applicator
			)
			.dependent collapse<region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

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

		if (cauto player_pos{ game_map<map_type>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !player_pos.has_value() || !entity_registry<map_type>.dependent add<player_t>(player_pos.value())) {
			error_log.add("could not find open position for player!");
			terminate_prematurely();
		}

		if constexpr (globals::SpawnTutorialPortal) {
			if (cauto portal_pos{ game_map<map_type>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; !portal_pos.has_value() || !object_registry<map_type>.add(portal_pos.value(), portal_t{ stability_e::Insightful })) {
				error_log.add("could not find open position for tutorial portal!");
				terminate_prematurely();
			}
		}

		if constexpr (globals::EnableBoonPortal) {
			if (cauto portal_pos{ game_map<map_type>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value()) {
				object_registry<map_type>.add(portal_pos.value(), portal_t{ stability_e::Calm });
			}
		}

		if constexpr (globals::EnableTribulationPortal) {
			if (cauto portal_pos{ game_map<map_type>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value()) {
				object_registry<map_type>.add(portal_pos.value(), portal_t{ stability_e::Turbulent });
			}
		}

		if constexpr (globals::EnableAudiencePortal) {
			if (cauto portal_pos{ game_map<map_type>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) }; portal_pos.has_value()) {
				object_registry<map_type>.add(portal_pos.value(), portal_t{ stability_e::Vocal });
			}
		}

		object_registry<map_type>.dependent spawn<crevice_t>(
			static_cast<usize>(globals::StartingCrevices),
			static_cast<u32>(globals::MinimumCreviceDistance)
		);

		object_registry<map_type>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingUpLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Up
		);

		object_registry<map_type>.dependent spawn<ladder_t>(
			static_cast<usize>(globals::StartingDownLadders),
			static_cast<u32>(globals::MinimumLadderDistance),

			verticality_e::Down, random_engine
		);

		object_registry<map_type>.dependent spawn<bones_t>(
			static_cast<usize>(globals::StartingBones),
			static_cast<u32>(globals::MinimumBoneDistance),

			decay_e::Animate
		);
	}
} // namespace necrowarp
