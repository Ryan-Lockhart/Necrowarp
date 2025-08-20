#pragma once

#include <necrowarp/objects/locomotion/ladder.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

namespace necrowarp {
	inline void ladder_t::enshackle() noexcept {
		if (has_shackle()) {
			return;
		}
		
		shackle = random_shackle(random_engine);

		sync_animation();

		if (is_up_ladder()) {
			departure_goals_dirty = true;
		}

		++steam_stats_s::stats<stat_e::LaddersShackled>;
	}

	inline void ladder_t::enshackle(shackle_e type) noexcept {
		if (has_shackle() || type == shackle_e::Unshackled) {
			return;
		}
		
		shackle = type;

		sync_animation();

		if (is_up_ladder()) {
			departure_goals_dirty = true;
		}

		++steam_stats_s::stats<stat_e::LaddersShackled>;
	}

	inline void ladder_t::unshackle() noexcept {
		if (!has_shackle()) {
			return;
		}

		shackle = shackle_e::Unshackled;

		sync_animation();

		if (is_up_ladder()) {
			departure_goals_dirty = true;
		}

		++steam_stats_s::stats<stat_e::LaddersUnshackled>;
	};
} // namespace necrowarp
