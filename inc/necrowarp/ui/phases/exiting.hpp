#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<game_phase_t::Exiting> {
		static inline label_t confirm_quit_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Are you sure you want to quit?", colors::White },
				embedded_box_t{ colors::Red, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t confirm_quit_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ -1, 2 } }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "Yes", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t cancel_quit_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 1, 2 } }, cardinal_e::West },
			embedded_label_t{
				runes_t{ "No", colors::Red },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Exiting) {
				return false;
			}

			return
				confirm_quit_label.is_hovered() ||
				confirm_quit_button.is_hovered() ||
				cancel_quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::Exiting) {
				return;
			}

			confirm_quit_button.update(Mouse::button_t::Left);
			cancel_quit_button.update(Mouse::button_t::Left);

			if (confirm_quit_button.is_active()) {
				window.close();
			} else if (cancel_quit_button.is_active()) {
				phase.revert();
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			confirm_quit_label.draw(renderer, confirm_quit_label.box.background.dimmed(sine_wave.current_value()));

			confirm_quit_button.draw(renderer);
			cancel_quit_button.draw(renderer);
		}
	};
} // necrowarp
