#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	constexpr cstr help_hidden_text{ "        F1: Show Controls        " };

	constexpr cstr help_expanded_text{
		"        F1: Hide Controls        \n\n\n"
		"Player Movement:    WASD / Numpad\n\n\n"
		"Camera Movement:       Arrow Keys\n\n"
		"Toggle Camera Lock:           Tab\n\n\n"
		"Hasten Timestep:    Right Bracket\n\n"
		"Harry Timestep:      Left Bracket\n\n\n"
		"Resize Minimap:                 M\n\n\n"
		"Chaotic Warp:                   Q\n\n"
		"Precise Warp:                   E\n\n\n"
		"Annihilate:                     Z\n\n"
		"Repulse:                        X\n\n"
		"Calcify:                        C\n\n"
		"Incorporealize:                 V\n\n\n"
		"Calcitic Invocation:            1\n\n"
		"Spectral Invocation:            2\n\n"
		"Sanguine Invocation:            3\n\n"
		"Galvanic Invocation:            4\n\n\n"
		"Ravenous Invocation:            5\n\n"
		"Wretched Invocation:            6\n\n"
		"Cerebral Invocation:            7\n\n"
		"Infernal Invocation:            8\n\n\n"
		"Necromantic Ascendance:         R\n\n"
		"Calamitous Retaliation:         F\n\n\n"
		"Ignore Objects:      Left Control\n\n\n"
		"Toggle Fullscreen:            F10"
	};

	template<> struct phase_state_t<phase_e::Paused> {
		static inline offset_t resume_button_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() - 1 };
		}
		
		static inline labeled_button_t resume_button{
			anchor_t{ resume_button_position(), cardinal_e::Southeast },
			embedded_label_t{
				runes_t{ "Resume", colors::Green },
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

		static inline bool show_help{ false };

		static inline offset_t help_label_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>().w - 1, 2 };
		}
		
		static inline label_t help_label{
			anchor_t{ help_label_position(), cardinal_e::Northeast },
			embedded_label_t{
				runes_t{ show_help ? help_expanded_text : help_hidden_text, colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
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

		static inline void resize() noexcept {
			resume_button.position = resume_button_position();
			quit_button.position = quit_button_position();
			help_label.position = help_label_position();
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			resume_button.draw(renderer);
			quit_button.draw(renderer);
			help_label.draw(renderer);
		}
	};
} // namespace necrowarp
