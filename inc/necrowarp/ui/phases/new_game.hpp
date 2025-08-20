#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::NewGame> {
		static inline label_t header_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, globals::grid_size<grid_type_e::UI>().h / 5 }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Please select your desired patron...", colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t patron_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 2 } }, cardinal_e::South },
			embedded_label_t{
				to_colored_string<StartingPatron>(),
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline enum_dropdown_t<patron_e> patron_selector{
			transform_t{
				anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 },
				cardinal_e::North },
				extent_t{ 24, 3 }
			},
			embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } }
		};

		static inline labeled_button_t play_button{
			anchor_t{
				offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 20, 1 } },
				cardinal_e::Northeast
			},
			embedded_label_t{
				runes_t{ "Play", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t back_button{
			anchor_t{
				offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ -20, 1 } },
				cardinal_e::Northwest
			},
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t warning_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 0, 5 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{ "warning: sudoclor is not your ally!", colors::Red },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::NewGame) {
				return false;
			}

			return header_label.is_hovered() || patron_label.is_hovered() || patron_selector.is_hovered() || play_button.is_hovered() || back_button.is_hovered();
		}

		static inline void update_label() noexcept {
			if (phase.current_phase != phase_e::NewGame) {
				return;
			}

			patron_label.text = magic_enum::enum_switch([&](auto val) -> runes_t {
				constexpr patron_e cval{ val };

				return to_colored_string<cval>();
			}, patron_selector.get_selected());

			patron_label.box.border.color = to_color(patron_selector.get_selected());
		}

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::NewGame) {
				return;
			}

			patron_e last_patron{ patron_selector.get_selected() };

			patron_selector.update(button);

			if (last_patron != patron_selector.get_selected()) {
				update_label();
			}

			play_button.update(button_e::Left);

			back_button.update(button_e::Left);

			if (play_button.is_active()) {
				desired_patron = patron_selector.get_selected();

				phase.transition(phase_e::Loading);
			} else if (back_button.is_active()) {
				patron_selector.set_selected(desired_patron);

				update_label();

				phase.transition(phase_e::MainMenu);
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			header_label.draw(renderer);

			if (patron_selector.get_selected() == patron_e::Sudoclor) {
				warning_label.draw(renderer);
			}

			patron_selector.draw(renderer);

			patron_label.draw(renderer);

			play_button.draw(renderer);
			back_button.draw(renderer);
		}
	};
} //namespace necrowarp
