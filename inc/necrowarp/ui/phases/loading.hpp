#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::Loading> {
		static inline label_t loading_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Loading...", colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Loading) {
				return false;
			}

			return loading_label.is_hovered();
		}

		static inline void update(mouse_s::button_t button) noexcept {
			if (phase.current_phase != phase_e::Loading) {
				return;
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			loading_label.draw(renderer);
		}
	};
} // necrowarp
