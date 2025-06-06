#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::MainMenu> {
		static inline labeled_button_t credits_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Credits", colors::Orange },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t new_game_button{
			anchor_t{ credits_button.position - offset_t{ credits_button.calculate_size().w / 2 + 2, 0 }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "New Game", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ credits_button.position + offset_t{ credits_button.calculate_size().w / 2 + 2, 0 }, cardinal_e::West },
			embedded_label_t{
				runes_t{ "Quit", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::MainMenu) {
				return false;
			}

			return
				new_game_button.is_hovered() ||
				credits_button.is_hovered() ||
				quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != phase_e::MainMenu) {
				return;
			}

			new_game_button.update(Mouse::button_t::Left);
			credits_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (new_game_button.is_active()) {
				phase.transition(phase_e::NewGame);
			} else if (credits_button.is_active()) {
				phase.transition(phase_e::Credits);
			} else if (quit_button.is_active()) {
				phase.transition(phase_e::Exiting);
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			new_game_button.draw(renderer);
			credits_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};
} // namespace necrowarp
