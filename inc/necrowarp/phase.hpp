#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct phase_e : u8 { None, MainMenu, Credits, NewGame, Loading, Playing, Paused, GameOver, Exiting };

	struct phase_t {
		phase_e previous_phase{ phase_e::None };
		phase_e current_phase{ phase_e::MainMenu };

		constexpr phase_t() noexcept{};
		
		constexpr phase_t(phase_e phase) noexcept : previous_phase{ phase_e::None }, current_phase{ phase }{}

		template<bool Sever = false> constexpr void transition(phase_e phase) noexcept {
			if constexpr (Sever) {
				previous_phase = phase;
			} else {
				previous_phase = current_phase;
			}

			current_phase = phase;
		}

		constexpr void revert() noexcept {
			current_phase = previous_phase;
			previous_phase = phase_e::None;
		}
	};
} // namespace necrowarp
