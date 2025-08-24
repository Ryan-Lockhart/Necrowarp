#pragma once

#include <necrowarp/dimensions/dimension.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<> inline void game_s::load<dimension_e::AncientVault>() noexcept {}

	template<> inline void game_s::descend<dimension_e::AncientVault>() noexcept {}

	template<> inline void game_s::plunge<dimension_e::AncientVault>() noexcept {}

	template<> inline void game_s::process_turn<dimension_e::AncientVault>() noexcept {}

	template<> inline void game_s::unload<dimension_e::AncientVault>() noexcept {
		constexpr map_type_e MapType = determine_map<dimension_e::AncientVault>();

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
