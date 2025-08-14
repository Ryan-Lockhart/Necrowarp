#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	template<> struct phase_state_t<phase_e::Credits> {
		static inline label_t credits_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{
					"                 Bjarne Stroustrup and the C++ standards committee for the C++ programming language                \n\n\n"
					"Chris Lattner, Vikram Adve, and team responsible for the LLVM backend, Clang C++ compiler, clangd, and clang-format\n\n\n"
					"   Sam Lantinga and the team responsible for the following libraries: SDL2, SDL2_image, SDL2_mixer, and SDL2_net   \n\n\n"
					"                       Valve and the team responsible for the Steamworks API and Storefront                        \n\n\n\n\n"
					"             Mateus Santos de Oliveira, the pixel artist responsible for the game's wonderful artwork              \n\n\n"
					"             Tejon, the author of the 8x8 Potash bitmap font from the Dwarf Fortress Tileset Repository            \n\n\n\n\n"
					"                Entity design contributions by Zachary Shelton (bonespur) and Humza Malik (skulker)                \n\n\n"
					"             The support of my father, Jeff, and my friends Jackson, Donald, Robert, Beaches and Hunter            \n\n\n\n\n"
					"  Dedicated to my late cousin, Gaige Gilbert, the man that introduced me to programming almost two decades ago...  "
				},
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 2, 2 }
			}
		};

		static inline labeled_button_t back_button{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() / 2 + offset_t{ 0, credits_label.calculate_size().h / 2 + 1 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Credits) {
				return false;
			}

			return credits_label.is_hovered() || back_button.is_hovered();
		}

		static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::Credits) {
				return;
			}

			back_button.update(button_e::Left);

			if (back_button.is_active()) {
				phase.revert();
			}
		}

		static inline void draw(ref<renderer_t> renderer) noexcept {
			credits_label.draw(renderer);

			back_button.draw(renderer);
		}
	};
} // namespace necrowarp
