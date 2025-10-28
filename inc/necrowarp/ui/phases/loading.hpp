#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::Loading> {
		static inline offset_t loading_label_position() noexcept {
			return offset_t{ globals::grid_size<grid_type_e::UI>() / 2 };
		}

		static inline label_t loading_label{
			anchor_t{ loading_label_position(), cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Loading...", colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Loading) {
				return false;
			}

			return loading_label.is_hovered();
		}

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::Loading) {
				return;
			}
		}

		static inline void resize() noexcept {
			loading_label.position = loading_label_position();
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			loading_label.draw(renderer);
		}
	};
} // namespace necrowarp
