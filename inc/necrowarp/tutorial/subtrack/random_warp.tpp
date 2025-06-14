#pragma once

#include <necrowarp/tutorial/subtrack/random_warp.hpp>

namespace necrowarp::tutorial {
	using namespace bleak;

	ref<subtrack_s<track_e::RandomWarp>::subtrack_e> static inline operator++(ref<subtrack_s<track_e::RandomWarp>::subtrack_e> subtrack) noexcept {
		return subtrack = static_cast<subtrack_s<track_e::RandomWarp>::subtrack_e>(static_cast<subtrack_s<track_e::RandomWarp>::subtrack_base>(subtrack) + 1), subtrack;
	}

	ref<subtrack_s<track_e::RandomWarp>::subtrack_e> static inline operator--(ref<subtrack_s<track_e::RandomWarp>::subtrack_e> subtrack) noexcept {
		return subtrack = static_cast<subtrack_s<track_e::RandomWarp>::subtrack_e>(static_cast<subtrack_s<track_e::RandomWarp>::subtrack_base>(subtrack) - 1), subtrack;
	}

	inline void subtrack_s<track_e::RandomWarp>::advance() noexcept {
		if (on_ending_subtrack()) {
			return;
		}

		++current_subtrack;
	}

	inline void subtrack_s<track_e::RandomWarp>::reverse() noexcept {
		if (on_starting_subtrack()) {
			return;
		}

		--current_subtrack;
	}
} // namespace necrowarp::tutorial
