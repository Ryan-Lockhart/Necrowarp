#pragma once

#include <bleak.hpp>

#include <necrowarp/ui.hpp>
#include <necrowarp/ui/phases.hpp>

#include <necrowarp/game_state.hpp>
#include <necrowarp/entity_state.tpp>
#include <necrowarp/object_state.tpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	static inline label_t title_label{
		anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, 1 }, cardinal_e::North },
		embedded_label_t{
			runes_t{ globals::GameTitle, colors::Marble },
			embedded_box_t{ colors::Black,
			border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline label_t fps_label{
		anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w, 1 }, cardinal_e::Northeast },
		embedded_label_t{
			runes_t{ "FPS: 0000", colors::Green },
			embedded_box_t{ colors::Black, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline bool any_hovered() noexcept {
		if (title_label.is_hovered() || fps_label.is_hovered()) {
			return true;
		}

		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr game_phase_t cval{ val };

			if constexpr (cval != game_phase_t::None) {
				return phase_state_t<cval>::any_hovered();
			}

			return false;
		}, phase.current_phase);
	}

	struct ui_registry_t {
		inline void update() noexcept {
			if (window.is_closing()) {
				return;
			}

			fps_label.text = {
				std::format("FPS: {:4}", static_cast<u32>(Clock::frame_time())),
				colors::Green
			};

			draw_cursor = any_hovered() || !(((globals::map_bounds() + extent_t{ 1, 1 }) * globals::cell_size<grid_type_e::Game>) + globals::grid_origin<grid_type_e::Game>()).within(ui_cursor.get_position());

			ui_cursor.update();
			
			grid_cursor.update(camera);

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr game_phase_t cval{ val };

				if constexpr (cval != game_phase_t::None) {
					phase_state_t<cval>::update(Mouse::button_t::Left);
				}
			}, phase.current_phase);
		}

		inline void render() const noexcept {
			if (window.is_closing()) {
				return;
			}

			if (phase.current_phase == game_phase_t::Playing) {
				if (!draw_cursor) {
					grid_cursor.draw(camera, globals::grid_origin<grid_type_e::Game>());
				}

				if (draw_warp_cursor) {
					warp_cursor.draw(camera, globals::grid_origin<grid_type_e::Game>());
				}
			}

			magic_enum::enum_switch([&](auto val){
				constexpr game_phase_t cval{ val };

				if constexpr (cval != game_phase_t::None) {
					phase_state_t<cval>::draw(renderer);
				}				
			}, phase.current_phase);

			title_label.draw(renderer);
			fps_label.draw(renderer);

			if (phase.current_phase != game_phase_t::Playing || draw_cursor) {
				ui_cursor.draw();
			}
		}
	} static inline ui_registry;
} // namespace necrowarp
