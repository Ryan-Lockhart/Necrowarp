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

	static inline offset_t background_image_position() noexcept {
		return offset_t{ globals::window_size().w / 2, globals::window_size().h / 2 };
	}

	static inline image_t base_start_background_image{
		anchor_t{ background_image_position(), cardinal_e::Central },
		embedded_image_t{ backgrounds.base_start }
	};

	static inline image_t base_end_background_image{
		anchor_t{ background_image_position(), cardinal_e::Central },
		embedded_image_t{ backgrounds.base_end }
	};

	static inline image_t expansion_start_background_image{
		anchor_t{ background_image_position(), cardinal_e::Central },
		embedded_image_t{ backgrounds.expansion_start }
	};

	static inline image_t expansion_end_background_image{
		anchor_t{ background_image_position(), cardinal_e::Central },
		embedded_image_t{ backgrounds.expansion_end }
	};

	static inline offset_t title_label_position() noexcept {
		return offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, 2 };
	}
	
	static inline label_t title_label{
		anchor_t{ title_label_position(), cardinal_e::North },
		embedded_label_t{
			runes_t{ globals::GameTitle, colors::Marble },
			embedded_box_t{ colors::Black,
			border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline offset_t fps_label_position() noexcept {
		return offset_t{ globals::grid_size<grid_type_e::UI>().w, 1 };
	}
	
	static inline label_t fps_label{
		anchor_t{ fps_label_position(), cardinal_e::Northeast },
		embedded_label_t{
			runes_t{ "FPS: 0000", colors::Green },
			embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline enum_dropdown_t<globals::resolution_e> resolution_selector{
		transform_t{
			anchor_t{ offset_t{ 1, 1 }, cardinal_e::Northwest },
			extent_t{ 39, 3 },
		},
		embedded_box_t{
			colors::Black, 
			border_t{ colors::White, 1 }
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
					return phase_state_t<cval>::dependent any_hovered<MapType>();
				} else {
					if constexpr (cval == phase_e::MainMenu || cval == phase_e::Paused) {
						if (resolution_selector.is_hovered()) {
							return true;
						}
					}

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

			draw_cursor = any_hovered<MapType>();

			ui_cursor.update();

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr phase_e cval{ val };

				if constexpr (cval != phase_e::None) {
					if constexpr (cval == phase_e::Playing) {
						phase_state_t<cval>::dependent update<MapType>(button_e::Left);
					} else {
						if constexpr (cval == phase_e::MainMenu || cval == phase_e::Paused) {
							globals::resolution_e last_resolution{ resolution_selector.get_selected() };

							resolution_selector.update(button_e::Left);

							if (last_resolution != resolution_selector.get_selected()) {
								globals::current_resolution = resolution_selector.get_selected();

								resize();
							}
						}

						phase_state_t<cval>::update(button_e::Left);
					}
				}
			}, phase.current_phase);

			grid_cursor<MapType>.update(camera<MapType>);

			grid_cursor<MapType>.color.set_alpha(sine_wave.current_value());
			warp_cursor<MapType>.color.set_alpha(sine_wave.current_value());
		}

		inline void resize() noexcept {
			base_start_background_image.position = background_image_position();
			base_end_background_image.position = background_image_position();

			expansion_start_background_image.position = background_image_position();
			expansion_end_background_image.position = background_image_position();

			title_label.position = title_label_position();

			fps_label.position = fps_label_position();

			magic_enum::enum_for_each<phase_e>([&](auto val) -> void {
				constexpr phase_e cval{ val };

				if constexpr (cval != phase_e::None) {
					if constexpr (cval == phase_e::Playing) {
						phase_state_t<cval>::dependent resize<map_type_e::Pocket>();
						phase_state_t<cval>::dependent resize<map_type_e::Standard>();
					} else {
						phase_state_t<cval>::resize();
					}
				}
			});

			window.set_size(globals::window_size());
			window.update_fullscreen();

			camera<map_type_e::Pocket>.resize(
				globals::grid_size<grid_type_e::Game>(),

				globals::camera_origin<map_type_e::Pocket>(),
				globals::camera_extent<map_type_e::Pocket>()
			);

			camera<map_type_e::Standard>.resize(
				globals::grid_size<grid_type_e::Game>(),

				globals::camera_origin<map_type_e::Standard>(),
				globals::camera_extent<map_type_e::Standard>()
			);
		}

		template<map_type_e MapType> inline void render() const noexcept {
			if (window.is_closing()) {
				return;
			}

			if (phase.current_phase == phase_e::Playing) {
				if (!draw_cursor) {
					grid_cursor<MapType>.draw(grid_cursor_texture, camera<MapType>, globals::grid_origin<grid_type_e::Game>() + globals::SparseTileNudge);
				}

				if (warped_from.has_value()) {
					warp_cursor<MapType>.draw(grid_cursor_texture, camera<MapType>, globals::grid_origin<grid_type_e::Game>() + globals::SparseTileNudge);
				}
			}

			magic_enum::enum_switch([&](auto val){
				constexpr phase_e cval{ val };

				if constexpr (cval != phase_e::None) {
					if constexpr (cval == phase_e::Playing) {
						phase_state_t<cval>::dependent draw<MapType>(renderer);
					} else {
						if constexpr (cval == phase_e::MainMenu || cval == phase_e::Paused) {
							if (api_state.owns_dlc) {
								if (steam_stats::is_unlocked(achievement_e::DefeatPraethornyn)) {
									expansion_end_background_image.draw();
								} else if (steam_stats::is_unlocked(achievement_e::CompleteAdvancedTutorial)) {
									expansion_start_background_image.draw();
								}
							} else {
								if (steam_stats::is_unlocked(achievement_e::DefeatElbikezzir)) {
									base_end_background_image.draw();
								} else {
									base_start_background_image.draw();
								}
							}
						}

						phase_state_t<cval>::draw(renderer);

						if constexpr (cval == phase_e::MainMenu || cval == phase_e::Paused) {
							resolution_selector.draw(renderer);
						}
					}
				}
			}, phase.current_phase);

			title_label.draw(renderer);

			ui_cursor.draw(ui_cursor_texture);
		}
	} static inline ui_registry;
} // namespace necrowarp
