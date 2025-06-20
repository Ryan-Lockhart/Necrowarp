#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
template<> struct phase_state_t<phase_e::GameOver> {
		static inline bool show_statistics{ false };

		static inline label_t game_over_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{
					runes_t{
						"You were slain! Game over...",
						colors::White
					}.concatenate(
						runes_t{
							"\n\n        ...or is it?",
							colors::dark::Magenta
						}
					)
				},
				embedded_box_t{ colors::Red, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t retry_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ -1, 1 } }, cardinal_e::Northeast },
			embedded_label_t{
				runes_t{ "Retry", colors::Green },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 1, 1 } }, cardinal_e::Northwest },
			embedded_label_t{
				runes_t{ "Quit", colors::Red },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_hidden_label{
			anchor_t{ offset_t{ 1, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ " Statistics ", colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_expanded_label{
			anchor_t{ offset_t{ 1, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ " Statistics ", colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::GameOver) {
				return false;
			}

			return
				game_over_label.is_hovered() ||
				retry_button.is_hovered() ||
				quit_button.is_hovered() ||
				(show_statistics ? statistics_expanded_label.is_hovered() : statistics_hidden_label.is_hovered());
		}

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::GameOver) {
				return;
			}

			retry_button.update(button_e::Left);
			quit_button.update(button_e::Left);

			if (retry_button.is_active()) {
				phase.transition(phase_e::Loading);
			} else if (quit_button.is_active()) {
				phase.transition(phase_e::Exiting);
			}

			game_over_label.text = runes_t{ "You were slain! Game over...", colors::White };

			if (retry_button.is_hovered()) {
				game_over_label.text.concatenate(runes_t{ "\n\n        ...or is it?", colors::dark::Magenta });
			}

			show_statistics = show_statistics ? statistics_expanded_label.is_hovered() : statistics_hidden_label.is_hovered();

			if (show_statistics) {
				runes_t stats_str{ "\n" };

				magic_enum::enum_for_each<steam_stat_e>([&stats_str] (auto val) -> void {
					constexpr steam_stat_e Stat{ val };

					using Type = to_stat_type<Stat>::type;

					const Type current_value{ steam_stats::stats<Stat, Type>.get_value() };
					const Type delta_value{ steam_stats::stats<Stat, Type>.get_value() - steam_stats::stats<Stat, Type>.initial_value };

					stats_str.concatenate(std::format(" {}: {} ({}{}) \n\n",
						steam_stats::stats<Stat, Type>.display_name,
						std::to_string(current_value),
						delta_value > 0 ? "+" : "",
						std::to_string(delta_value)
					));
				});

				stats_str.concatenate(runes_t{"\n Statistics "});

				statistics_expanded_label.text = stats_str;
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			game_over_label.draw(renderer);
			retry_button.draw(renderer);
			quit_button.draw(renderer);

			show_statistics ?
				statistics_expanded_label.draw(renderer) :
				statistics_hidden_label.draw(renderer);
		}
	};
} // namespace necrowarp
