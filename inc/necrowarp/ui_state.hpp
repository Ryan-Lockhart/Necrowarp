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

	template<map_type_e MapType> static inline bool any_hovered() noexcept {
		if (title_label.is_hovered() || fps_label.is_hovered()) {
			return true;
		}

		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr phase_e cval{ val };

			if constexpr (cval != phase_e::None) {
				if constexpr (cval == phase_e::Playing) {
					return phase_state_t<cval>::template any_hovered<MapType>();
				} else {
					return phase_state_t<cval>::any_hovered();
				}				
			}

			return false;
		}, phase.current_phase);
	}

	struct ui_registry_t {
		template<map_type_e MapType> inline void update() noexcept {
			if (window.is_closing()) {
				return;
			}

			fps_label.text = {
				std::format("FPS: {:4}", static_cast<u32>(Clock::frame_time())),
				colors::Green
			};

			draw_cursor = any_hovered<MapType>() || !(((globals::map_bounds<MapType>() + extent_t{ 1, 1 }) * globals::cell_size<grid_type_e::game_s>) + globals::grid_origin<grid_type_e::game_s>()).within(ui_cursor.get_position());

			ui_cursor.update();
			
			grid_cursor<MapType>.update(camera<MapType>);

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr phase_e cval{ val };

				if constexpr (cval != phase_e::None) {
					if constexpr (cval == phase_e::Playing) {
						phase_state_t<cval>::template update<MapType>(button_e::Left);
					} else {
						phase_state_t<cval>::update(button_e::Left);
					}				
				}
			}, phase.current_phase);
		}

		template<map_type_e MapType> inline void render() const noexcept {
			if (window.is_closing()) {
				return;
			}

			if (phase.current_phase == phase_e::Playing) {
				if (!draw_cursor) {
					grid_cursor<MapType>.draw(grid_cursor_texture, camera<MapType>, globals::grid_origin<grid_type_e::game_s>());
				}

				if (draw_warp_cursor) {
					warp_cursor<MapType>.draw(grid_cursor_texture, camera<MapType>, globals::grid_origin<grid_type_e::game_s>());
				}
			}

			magic_enum::enum_switch([&](auto val){
				constexpr phase_e cval{ val };

				if constexpr (cval != phase_e::None) {
					if constexpr (cval == phase_e::Playing) {
						phase_state_t<cval>::template draw<MapType>(renderer);
					} else {
						phase_state_t<cval>::draw(renderer);
					}
				}
			}, phase.current_phase);

			title_label.draw(renderer);
			fps_label.draw(renderer);

			if (phase.current_phase != phase_e::Playing || draw_cursor) {
				ui_cursor.draw(ui_cursor_texture);
			}
		}
	} static inline ui_registry;
} // namespace necrowarp
