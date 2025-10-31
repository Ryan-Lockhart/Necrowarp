#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::MainMenu> {
		static inline offset_t new_game_button_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() - 1 };
		}
		
		static inline labeled_button_t new_game_button{
			anchor_t{ new_game_button_position(), cardinal_e::Southeast },
			embedded_label_t{
				runes_t{ "New Game", colors::Green },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t credits_button_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>().w - 1, 2 };
		}
		
		static inline labeled_button_t credits_button{
			anchor_t{ credits_button_position(), cardinal_e::Northeast },
			embedded_label_t{
				runes_t{ "Credits", colors::Orange },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t quit_button_position() noexcept {
			return offset_t{ 2, globals::grid_size<grid_type_e::UI>().h - 1 };
		}
		
		static inline labeled_button_t quit_button{
			anchor_t{ quit_button_position(), cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ "Quit", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
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

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::MainMenu) {
				return;
			}

			new_game_button.update(button_e::Left);
			credits_button.update(button_e::Left);
			quit_button.update(button_e::Left);

			if (new_game_button.is_active()) {
				phase.transition(phase_e::NewGame);
			} else if (credits_button.is_active()) {
				phase.transition(phase_e::Credits);
			} else if (quit_button.is_active()) {
				phase.transition(phase_e::Exiting);
			}
		}

		static inline void resize() noexcept {
			new_game_button.position = new_game_button_position();
			credits_button.position = credits_button_position();
			quit_button.position = quit_button_position();
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			new_game_button.draw(renderer);
			credits_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};
} // namespace necrowarp
