#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::NewGame> {
		static inline offset_t header_label_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, globals::grid_size<grid_type_e::UI>().h / 5 };
		}
		
		static inline label_t header_label{
			anchor_t{ header_label_position(), cardinal_e::South },
			embedded_label_t{
				runes_t{ "Please select your desired patron...", colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t patron_label_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 2 } };
		}
		
		static inline label_t patron_label{
			anchor_t{ patron_label_position(), cardinal_e::South },
			embedded_label_t{
				to_colored_string<StartingPatron>(),
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t patron_selector_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 };
		}
		
		static inline enum_dropdown_t<patron_e> patron_selector{
			transform_t{
				anchor_t{ patron_selector_position(), cardinal_e::North },
				extent_t{ 25, 3 }
			},
			embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } }
		};

		static inline offset_t play_button_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 20, 1 } };
		}
		
		static inline labeled_button_t play_button{
			anchor_t{
				play_button_position(),
				cardinal_e::Northeast
			},
			embedded_label_t{
				runes_t{ "Play", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t back_button_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ -20, 1 } };
		}
		
		static inline labeled_button_t back_button{
			anchor_t{
				back_button_position(),
				cardinal_e::Northwest
			},
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline offset_t rush_toggle_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() - offset_t{ 1, 1 } };
		}
		
		static inline labeled_toggle_t rush_toggle{
			anchor_t{
				rush_toggle_position(),
				cardinal_e::Southeast
			},
			embedded_label_t{
				runes_t{ "Rush Mode", colors::Orange },
				embedded_box_t{ colors::White, border_t{ colors::Red, 1 } },
				extent_t{ 1, 1 }
			},
			globals::rush_mode_toggle
		};

		static inline offset_t warning_label_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 0, 5 } };
		}
		
		static inline label_t warning_label{
			anchor_t{ warning_label_position(), cardinal_e::North },
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

			if constexpr (globals::EnableRushMode) {
				if (rush_toggle.is_hovered()) {
					return true;
				}
			}

			return
				header_label.is_hovered() ||
				patron_label.is_hovered() ||
				patron_selector.is_hovered() ||
				play_button.is_hovered() ||
				back_button.is_hovered() ||
				warning_label.is_hovered();
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

			if constexpr (globals::EnableRushMode) {
				rush_toggle.update(button_e::Left);
			}

			if (play_button.is_active()) {
				desired_patron = patron_selector.get_selected();
				globals::rush_mode_toggle = rush_toggle.is_active();

				phase.transition(phase_e::Loading);
			} else if (back_button.is_active()) {
				patron_selector.set_selected(desired_patron);

				update_label();

				phase.transition(phase_e::MainMenu);
			}
		}

		static inline void resize() noexcept {
			header_label.position = header_label_position();

			patron_label.position = patron_label_position();
			patron_selector.position = patron_selector_position();

			play_button.position = play_button_position();

			back_button.position = back_button_position();

			if constexpr (globals::EnableRushMode) {
				rush_toggle.position = rush_toggle_position();
			}

			warning_label.position = warning_label_position();
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

			if constexpr (globals::EnableRushMode) {
				rush_toggle.draw(renderer);
			}
		}
	};
} //namespace necrowarp
