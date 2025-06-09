#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::NewGame> {
		static inline label_t header_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, globals::grid_size<grid_type_e::UI>().h / 4 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Please select your desired patron...", colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t akurakhaithan_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 2 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::light::Magenta, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_toggle_t akurakhaithan_toggle{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 0, 2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{
					"Akurakhaithan",
					colors::light::Magenta
				},
				embedded_box_t{ colors::Charcoal, { colors::light::Magenta, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t rathghul_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ (akurakhaithan_label.calculate_size().w * 2) * globals::cell_size<grid_type_e::UI>.w + 12, 2 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::light::Green, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_toggle_t rathghul_toggle{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ (akurakhaithan_label.calculate_size().w * 2) * globals::cell_size<grid_type_e::UI>.w + 12, -2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{
					"Rathghul",
					colors::light::Green
				},
				embedded_box_t{ colors::Charcoal, { colors::light::Green, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t merirfin_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ (akurakhaithan_label.calculate_size().w * 2) * globals::cell_size<grid_type_e::UI>.w + 12, -2 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::light::Red, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_toggle_t merirfin_toggle{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ (akurakhaithan_label.calculate_size().w * 2) * globals::cell_size<grid_type_e::UI>.w + 12, 2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{
					"Merirfin",
					colors::light::Red
				},
				embedded_box_t{ colors::Charcoal, { colors::light::Red, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t no_patron_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ (akurakhaithan_label.calculate_size().w * 4) * globals::cell_size<grid_type_e::UI>.w + 24, 2 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::light::Grey, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_toggle_t no_patron_toggle{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ (akurakhaithan_label.calculate_size().w * 4) * globals::cell_size<grid_type_e::UI>.w + 24, -2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{
					"None",
					colors::light::Grey
				},
				embedded_box_t{ colors::Charcoal, { colors::light::Grey, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t saeiligarkeuss_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ (akurakhaithan_label.calculate_size().w * 4) * globals::cell_size<grid_type_e::UI>.w + 25, -2 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::dark::Orange, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_toggle_t saeiligarkeuss_toggle{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ (akurakhaithan_label.calculate_size().w * 4) * globals::cell_size<grid_type_e::UI>.w + 25, 2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{
					"Saeiligarkeuss",
					colors::dark::Orange
				},
				embedded_box_t{ colors::Charcoal, { colors::dark::Orange, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t play_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2 - 1, globals::grid_size<grid_type_e::UI>().h - globals::grid_size<grid_type_e::UI>().h / 3 }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "Play", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t back_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2 + 1, globals::grid_size<grid_type_e::UI>().h - globals::grid_size<grid_type_e::UI>().h / 3 }, cardinal_e::West },
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::NewGame) {
				return false;
			}

			return
				header_label.is_hovered() ||
				no_patron_label.is_hovered() ||
				no_patron_toggle.is_hovered() ||
				rathghul_label.is_hovered() ||
				rathghul_toggle.is_hovered() ||
				akurakhaithan_label.is_hovered() ||
				akurakhaithan_toggle.is_hovered() ||
				play_button.is_hovered() ||
				back_button.is_hovered();
		}

		static inline bool any_environ_selected() noexcept {
			return false;
		}

		static inline bool any_patron_selected() noexcept {
			return
				no_patron_toggle.is_toggled() ||
				rathghul_toggle.is_toggled() ||
				akurakhaithan_toggle.is_toggled() ||
				merirfin_toggle.is_toggled() ||
				saeiligarkeuss_toggle.is_toggled();
		}

		static inline patron_e get_selected_patron() noexcept {
			if (no_patron_toggle.is_toggled()) {
				return patron_e::None;
			} else if (rathghul_toggle.is_toggled()) {
				return patron_e::Rathghul;
			} else if (akurakhaithan_toggle.is_toggled()) {
				return patron_e::Akurakhaithan;
			} else if (merirfin_toggle.is_toggled()) {
				return patron_e::Merirfin;
			} else if (saeiligarkeuss_toggle.is_toggled()) {
				return patron_e::Saeiligarkeuss;
			}

			return patron_e::None;
		}

		static inline void reset_patron_toggles() noexcept {
			no_patron_toggle.toggle_off();
			rathghul_toggle.toggle_off();
			akurakhaithan_toggle.toggle_off();
			merirfin_toggle.toggle_off();
			saeiligarkeuss_toggle.toggle_off();
		}

		static inline void update(mouse_s::button_t button) noexcept {
			if (phase.current_phase != phase_e::NewGame) {
				return;
			}

			no_patron_toggle.update(mouse_s::button_t::Left,
				&rathghul_toggle,
				&akurakhaithan_toggle,
				&merirfin_toggle,
				&saeiligarkeuss_toggle
			);
			
			no_patron_label.text = to_colored_string<patron_e::None>();

			rathghul_toggle.update(mouse_s::button_t::Left,
				&no_patron_toggle,
				&akurakhaithan_toggle,
				&merirfin_toggle,
				&saeiligarkeuss_toggle
			);
			
			rathghul_label.text = to_colored_string<patron_e::Rathghul>();

			akurakhaithan_toggle.update(mouse_s::button_t::Left,
				&no_patron_toggle,
				&rathghul_toggle,
				&merirfin_toggle,
				&saeiligarkeuss_toggle
			);

			akurakhaithan_label.text = to_colored_string<patron_e::Akurakhaithan>();

			merirfin_toggle.update(mouse_s::button_t::Left,
				&no_patron_toggle,
				&rathghul_toggle,
				&akurakhaithan_toggle,
				&saeiligarkeuss_toggle
			);
			
			merirfin_label.text = to_colored_string<patron_e::Merirfin>();

			saeiligarkeuss_toggle.update(mouse_s::button_t::Left,
				&no_patron_toggle,
				&rathghul_toggle,
				&akurakhaithan_toggle,
				&merirfin_toggle
			);

			saeiligarkeuss_label.text = to_colored_string<patron_e::Saeiligarkeuss>();

			no_patron_toggle.box.background = no_patron_toggle.is_toggled() ? colors::Marble : colors::Charcoal;
			rathghul_toggle.box.background = rathghul_toggle.is_toggled() ? colors::Marble : colors::Charcoal;
			akurakhaithan_toggle.box.background = akurakhaithan_toggle.is_toggled() ? colors::Marble : colors::Charcoal;
			merirfin_toggle.box.background = merirfin_toggle.is_toggled() ? colors::Marble : colors::Charcoal;
			saeiligarkeuss_toggle.box.background = saeiligarkeuss_toggle.is_toggled() ? colors::Marble : colors::Charcoal;

			play_button.box.background = any_patron_selected() ? colors::Grey : colors::Black;

			if (any_patron_selected()) {
				play_button.update(mouse_s::button_t::Left);
			}

			back_button.update(mouse_s::button_t::Left);

			if (play_button.is_active() && any_patron_selected()) {
				desired_patron = get_selected_patron();

				reset_patron_toggles();

				phase.transition(phase_e::Loading);
			} else if (back_button.is_active()) {
				reset_patron_toggles();

				phase.transition(phase_e::MainMenu);
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			header_label.draw(renderer);

			no_patron_label.draw(renderer);
			no_patron_toggle.draw(renderer);

			rathghul_label.draw(renderer);
			rathghul_toggle.draw(renderer);

			akurakhaithan_label.draw(renderer);
			akurakhaithan_toggle.draw(renderer);

			merirfin_label.draw(renderer);
			merirfin_toggle.draw(renderer);

			saeiligarkeuss_label.draw(renderer);
			saeiligarkeuss_toggle.draw(renderer);

			play_button.draw(renderer);
			back_button.draw(renderer);
		}
	};
} //namespace necrowarp
