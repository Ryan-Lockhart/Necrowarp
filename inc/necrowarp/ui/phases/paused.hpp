#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	constexpr cstr help_hidden_text{ " F1: Show Controls " };

	constexpr cstr help_expanded_text{
		" Movement:  WASD / Numpad \n\n\n"
		" Random Warp:           Q\n\n"
		" Target Warp:           E\n\n\n"
		" Calcitic Invocation:   1\n\n"
		" Spectral Invocation:   2\n\n"
		" Sanguine Invocation:   3\n\n"
		" Galvanic Invocation:   4\n\n\n"
		" Necromantic Ascension: R\n\n\n"
		" Ignore Objects:      Alt\n\n\n"
		" F1: Hide Controls "
	};

	template<> struct phase_state_t<phase_e::Paused> {
		static inline labeled_button_t resume_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Resume", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 0, 1 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{ "Quit", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool show_help{ true };

		static inline label_t help_label{
			anchor_t{ offset_t{ 1, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ show_help ? help_expanded_text : help_hidden_text, colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Paused) {
				return false;
			}

			return resume_button.is_hovered() || quit_button.is_hovered() || help_label.is_hovered();
		}

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::Paused) {
				return;
			}

			resume_button.update(button_e::Left);
			quit_button.update(button_e::Left);

			if (keyboard_s::is_key<input_e::Down>(bindings::ToggleControls)) {
				show_help = !show_help;

				help_label.text = runes_t{ show_help ? help_expanded_text : help_hidden_text, colors::White };
			}

			if (resume_button.is_active()) {
				phase.transition(phase_e::Playing);
			} else if (quit_button.is_active()) {
				phase.transition(phase_e::Exiting);
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			resume_button.draw(renderer);
			quit_button.draw(renderer);
			help_label.draw(renderer);
		}
	};
} // namespace necrowarp
