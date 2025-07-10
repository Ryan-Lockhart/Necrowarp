#pragma once

#include <necrowarp/entities/good/mist_lady.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline command_pack_t mist_lady_t::think(offset_t position) const noexcept {
		cauto descent_pos{ good_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!descent_pos.has_value()) {
			return command_pack_t{ command_e::None };
		}

		return command_pack_t{ command_e::Move, position, descent_pos.value() };
	}

	template<map_type_e MapType> inline void mist_lady_t::die(offset_t position) noexcept {
		object_registry<MapType>.spill(position, bones_t{});
		object_registry<MapType>.spill(position, flesh_t{});
		object_registry<MapType>.spill(position, cerebra_t{ entity_e::MistLady });

		player.receive_death_boon<mist_lady_t>();

		++steam_stats::stats<steam_stat_e::MistLadiesSlain>;

		scorekeeper.add(entity_e::MistLady);
	}

	template command_pack_t mist_lady_t::think<map_type_e::Pocket>(offset_t position) const noexcept;
	template command_pack_t mist_lady_t::think<map_type_e::Standard>(offset_t position) const noexcept;

	template void mist_lady_t::die<map_type_e::Pocket>(offset_t position) noexcept;
	template void mist_lady_t::die<map_type_e::Standard>(offset_t position) noexcept;
} // namespace necrowarp
