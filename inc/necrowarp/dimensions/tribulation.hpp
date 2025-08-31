#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<> inline void game_s::load<dimension_e::Tribulation>() noexcept {
		error_log.add("[ERROR]: tribulation dimension should not be loaded into!");

		terminate_prematurely();
	}

	template<> inline void game_s::descend<dimension_e::Tribulation>() noexcept {
		error_log.add("[ERROR]: tribulation dimension should not be descended into!");

		terminate_prematurely();
	}

	template<> inline void game_s::plunge<dimension_e::Tribulation>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Tribulation>();

		constexpr map_cell_t open_state{ cell_e::Open, cell_e::Transperant, cell_e::Seen, cell_e::Explored };
		constexpr map_cell_t closed_state{ cell_e::Solid, cell_e::Opaque, cell_e::Seen, cell_e::Explored };

		constexpr binary_applicator_t<map_cell_t> cell_applicator{ closed_state, open_state };

		game_map<MapType>
			.dependent set<region_e::Border>(closed_state)
			.dependent generate<region_e::Interior>(
				map_engine,
				globals::CavernPreset.fill_percent,
				globals::CavernPreset.automata_iterations,
				globals::CavernPreset.automata_threshold,
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
		
		const disposition_e current_disposition{ get_patron_disposition(player.patron) };
		
		const i16 gateway_count{ game_stats.current_gateways() };

		for (i16 i{ 0 }; i < gateway_count; ++i) {
			cauto gateway_position{ object_goal_map<MapType, gateway_t>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, globals::MinimumGatewayDistance) };

			if (!gateway_position.has_value()) {
				if (i == 0) {
					error_log.add("[ERROR]: could not find open position for gateway!");
					terminate_prematurely();
				} else {
					break;
				}
			}

			static std::uniform_int_distribution entity_dis{ static_cast<u16>(entity_e::Adventurer), static_cast<u16>(entity_e::Paladin) };
			
			object_registry<MapType>.add(gateway_position.value(), gateway_t{ static_cast<entity_e>(entity_dis(random_engine)), current_disposition });
		}

		object_registry<MapType>.dependent spawn<bones_t>(
			static_cast<usize>(globals::StartingBones / 2),
			static_cast<u32>(globals::MinimumBoneDistance),

			decay_e::Animate
		);

		tribulation_resolved = false;
	}

	template<> inline void game_s::process_turn<dimension_e::Tribulation>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Tribulation>();

		if (tribulation_resolved) {
			return;
		}

		if (!tribulation_resolved && entity_registry<MapType>.dependent empty<ALL_GOOD_NPCS>() && object_registry<MapType>.dependent empty<gateway_t>()) {
			tribulation_resolved = true;

			cauto portal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

			if (!portal_pos.has_value() || !object_registry<MapType>.add(portal_pos.value(), portal_t{ stability_e::Collapsing })) {
				error_log.add("[ERROR]: could not find open position for return portal!");
				terminate_prematurely();
			}

			const std::optional<grimoire_e> bounty{ literature::random_unacquired_invocation(random_engine) };

			if (bounty.has_value()) {
				cauto pedestal_pos{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open) };

				if (!pedestal_pos.has_value() || !object_registry<MapType>.add(pedestal_pos.value(), pedestal_t{ bounty.value() })) {
					error_log.add("[ERROR]: could not find open position for pedestal!");
					terminate_prematurely();
				}
			}
		}

		if (!object_registry<MapType>.dependent empty<gateway_t>()) {
			for (rauto [position, gateway] : object_registry_storage<gateway_t>) {
				if (!gateway.has_stability()) {
					object_registry<MapType>.dependent remove<gateway_t>(position);

					continue;
				}

				gateway.spawn<MapType>(position);
			}
		}
	}

	template<> inline void game_s::unload<dimension_e::Tribulation>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::Tribulation>();

		terminate_process_turn();

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
