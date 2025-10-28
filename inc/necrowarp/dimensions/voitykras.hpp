#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<> inline void game_s::load<dimension_e::Voitykras>() noexcept {}

	template<> inline void game_s::descend<dimension_e::Voitykras>() noexcept {}

	template<> inline void game_s::plunge<dimension_e::Voitykras>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Voitykras>();

		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		game_map<MapType>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::ChamberPreset.fill_percent,
				globals::ChamberPreset.automata_iterations,
				globals::ChamberPreset.automata_threshold,
				cell_applicator
			)
			.dependent collapse<region_e::Interior>(cell_e::Solid, 0x00, cell_e::Open);

		std::vector<area_t> areas{ area_t::partition(game_map<MapType>, cell_e::Open) };

		if (areas.size() > 1) {
			cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

			for (crauto area : areas) {
				if (area != largest_area) {
					area.apply(game_map<MapType>, cell_e::Solid);
				}
			}
		}

		for (offset_t::scalar_t y{ 0 }; y < globals::MapSize<MapType>.h; ++y) {
			for (offset_t::scalar_t x{ 0 }; x < globals::MapSize<MapType>.w; ++x) {
				const offset_t pos{ x, y };

				game_map<MapType>[pos].recalculate_index(game_map<MapType>, pos, cell_e::Solid);
			}
		}

		cauto player_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

		if (!player_pos.has_value() || !entity_registry<MapType>.dependent add<player_t>(player_pos.value())) {
			error_log.add("[ERROR]: could not find open position for player!");

			terminate_prematurely();
		}

		if (api_state.user_id == globals::PlonzoGuaranteedID || globals::plonzo_chance(random_engine)) {
			cauto plonzo_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!plonzo_pos.has_value() || !entity_registry<MapType>.dependent add<plonzo_t>(plonzo_pos.value())) {
				error_log.add("[ERROR]: could not find open position for plonzo!");

				terminate_prematurely();
			}
		}

		cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

		if (!portal_pos.has_value() || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Collapsing })) {
			error_log.add("[ERROR]: could not find open position for return portal!");

			terminate_prematurely();
		}

		const std::optional<grimoire_e> boon{ literature::random_unacquired_spell(random_engine) };

		if (boon.has_value()) {
			cauto pedestal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!pedestal_pos.has_value() || !object_registry<MapType>.add(pedestal_pos.value(), pedestal_t{ boon.value() })) {
				error_log.add("[ERROR]: could not find open position for pedestal!");
				terminate_prematurely();
			}
		}
	}

	template<> inline void game_s::process_turn<dimension_e::Voitykras>() noexcept {}

	template<> inline void game_s::unload<dimension_e::Voitykras>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Voitykras>();

		suspend_process_turn();

		game_map<MapType>.dependent reset<region_e::All>();
		fluid_map<MapType>.dependent reset<region_e::All>();
		
		entity_registry<MapType>.clear();
		object_registry<MapType>.clear();

#if !defined(STEAMLESS)
		steam_stats::store();

		stat_store_timer.record();
#endif
	}
} // namespace necrowarp
