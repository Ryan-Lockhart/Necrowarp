#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	constexpr color_t ActiveEnergyColor{ colors::Magenta };
	constexpr color_t InactiveEnergyColor{ ActiveEnergyColor, u8{ 0x80 } };

	constexpr color_t ActiveArmorColor{ colors::Marble };
	constexpr color_t InactiveArmorColor{ ActiveArmorColor, u8{ 0x80 } };

	constexpr color_t ActiveDivinityColor{ colors::metals::Gold };
	constexpr color_t InactiveDivinityColor{ ActiveDivinityColor, u8{ 0x80 } };

	constexpr cstr depth_hidden_text{ "Depth: 000" };

	constexpr cstr depth_expanded_text{
		"Player Kills: 0000\n\n"
		"Minion Kills: 0000\n\n\n"
		"Total Kills:  0000\n\n\n"
		"    Depth: 000    "
	};

	constexpr cstr favor_hidden_text{ " Favor " };

	constexpr cstr favor_expanded_text{
		"Patron:                      \n\n\n"
		"Random Warp:             0000\n\n"
		"Target Warp:             0000\n\n\n"
		"Calcitic Invocation:     0000\n\n"
		"Spectral Invocation:     0000\n\n"
		"Sanguine Invocation:     0000\n\n\n"
		"Necromantic Ascendance:  0000\n\n\n"
		" Favor                       "
	};

	template<> struct phase_state_t<phase_e::Playing> {
		static inline status_bar_t<3> player_statuses{
			anchor_t{ offset_t{ 1, 1 }, cardinal_e::Northwest},
			std::array<status_t, 3>{
				status_t{ runes_t{ "  energy: ", colors::Magenta }, player_t::MaximumEnergy, ActiveEnergyColor, InactiveEnergyColor },
				status_t{ runes_t{ "   armor: ", colors::Marble }, player_t::MaximumArmor, ActiveArmorColor, InactiveArmorColor },
				status_t{ runes_t{ "divinity: ", colors::metals::Gold }, player_t::MaximumDivinity, ActiveDivinityColor, InactiveDivinityColor }
			},
			embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		};

		static inline command_bar_t<7> player_commands{
			anchor_t{ offset_t{ 0, 0 }, cardinal_e::West },
			std::array<embedded_command_icon_t, 7>{
				embedded_command_icon_t{ icons::RandomWarp, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },
				embedded_command_icon_t{ icons::TargetWarp, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },

				embedded_command_icon_t{ icons::CalciticInvocation, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },
				embedded_command_icon_t{ icons::SpectralInvocation, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },
				embedded_command_icon_t{ icons::SanguineInvocation, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },
				embedded_command_icon_t{ icons::GalvanicInvocation, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },

				embedded_command_icon_t{ icons::NecromanticAscendance, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } }
			},
			embedded_box_t{ colors::Black, border_t{ colors::White, 1} },
			extent_t{ 1, 1 }
		};

		static inline label_t command_label{
			anchor_t{ offset_t{}, cardinal_e::Northwest },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t depth_hidden_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ depth_hidden_text, colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t depth_expanded_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w / 2, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ depth_expanded_text, colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t favor_hidden_label{
			anchor_t{ offset_t{ 1, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ favor_hidden_text, colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t favor_expanded_label{
			anchor_t{ offset_t{ 1, globals::grid_size<grid_type_e::UI>().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ favor_expanded_text, colors::White },
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t advancement_label{
			anchor_t{},
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static constexpr extent_t MinimapPixelSize{ 2, 2 };

		template<map_type_e MapType> static inline minimap_t<MapType, MinimapPixelSize> minimap{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w * globals::cell_size<grid_type_e::UI>.w, 1 }, cardinal_e::Northeast },
			embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		};

		template<map_type_e MapType> static inline label_t tooltip_label{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() }, cardinal_e::Southeast },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool show_tooltip{ false };
		static inline bool show_command{ false };
		static inline bool show_advancement{ false };
		static inline bool show_depth{ false };
		static inline bool show_favor{ false };

		static constexpr offset_t random_warp_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 - 4 }; }
		static constexpr offset_t target_warp_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 - 3 }; }

		static constexpr offset_t calcitic_invocation_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 - 1 }; }
		static constexpr offset_t spectral_invocation_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 }; }
		static constexpr offset_t sanguine_invocation_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 + 1 }; }
		static constexpr offset_t galvanic_invocation_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 + 2 }; }

		static constexpr offset_t necromantic_ascendance_icon_position() { return offset_t{ 0, globals::grid_size<grid_type_e::Icon>().h / 2 + 4 }; }

		static constexpr bool any_icon_hovered() noexcept {
			if (mouse_s::is_inside(random_warp_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(target_warp_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(calcitic_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(spectral_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(sanguine_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(galvanic_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			} else if (mouse_s::is_inside(necromantic_ascendance_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
				return true;
			}

			return false;
		}

		template<map_type_e MapType> static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Playing) {
				return false;
			}
			
			return player_statuses.is_hovered() || any_icon_hovered() || advancement_label.is_hovered() || (show_depth ? depth_expanded_label.is_hovered() : depth_hidden_label.is_hovered()) || (show_favor ? favor_expanded_label.is_hovered() : favor_hidden_label.is_hovered()) || minimap<MapType>.is_hovered();
		}

		template<map_type_e MapType> static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::Playing) {
				return;
			}

			player_statuses[0].current_value = player.get_energy();
			player_statuses[0].max_value = player.max_energy();

			player_statuses[1].current_value = player.get_armor();
			player_statuses[1].max_value = player.max_armor();

			player_statuses[2].current_value = player.get_divinity();
			player_statuses[2].max_value = player.max_divinity();

			const i8 kills_energy{ game_stats.kills_until_next_energy_slot() };
			const i8 kills_armor{ game_stats.kills_until_next_armor_slot() };
			const i8 kills_divinity{ game_stats.kills_until_next_divinity_turn() };

			show_advancement = player_statuses.is_hovered() || advancement_label.is_hovered();

			if (show_advancement) {
				advancement_label.position = player_statuses.position + extent_t{ player_statuses.external_size().w, 0 };
				advancement_label.alignment = player_statuses.alignment;

				advancement_label.text = runes_t{
					std::format(
						"[{}/{}]{}\n\n"
						"[{}/{}]{}\n\n"
						"[{}/{}]{}",

						player.get_energy(), player.max_energy(), player.max_energy() >= player_t::MaximumEnergy ? "" : std::format(" (+1 energy slot in {} kill{})", kills_energy == 0 ? globals::KillsPerEnergySlot : kills_energy, kills_energy == 1 ? "" : "s"),
						player.get_armor(), player.max_armor(), player.max_armor() >= player_t::MaximumArmor ? "" : std::format(" (+1 armor slot in {} kill{})", kills_armor == 0 ? globals::KillsPerArmorSlot : kills_armor, kills_armor == 1 ? "" : "s"),
						player.get_divinity(), player.max_divinity(), player.max_divinity() >= player_t::MaximumDivinity ? "" : std::format(" (+1 divinity turn in {} kill{})", kills_divinity == 0 ? globals::KillsPerDivinityTurn : kills_divinity, kills_divinity == 1 ? "" : "s")
					)
				};
			}

			show_command = any_icon_hovered();

			if (show_command) {
				const offset_t mouse_pos{ mouse_s::get_position() };

				if (mouse_s::is_inside(random_warp_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::RandomWarp) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::RandomWarp) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::RandomWarp)) })
						.concatenate(runes_t{ "]" });
					
					command_label.position = (random_warp_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(target_warp_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::TargetWarp) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::TargetWarp) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::TargetWarp)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (target_warp_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(calcitic_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::CalciticInvocation) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::CalciticInvocation) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::CalciticInvocation)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (calcitic_invocation_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(spectral_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::SpectralInvocation) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::SpectralInvocation) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::SpectralInvocation)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (spectral_invocation_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(sanguine_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::SanguineInvocation) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::SanguineInvocation) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::SanguineInvocation)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (sanguine_invocation_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(galvanic_invocation_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::GalvanicInvocation) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::GalvanicInvocation) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::GalvanicInvocation)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (galvanic_invocation_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				} else if (mouse_s::is_inside(necromantic_ascendance_icon_position() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					command_label.text = runes_t{ to_string(command_e::NecromanticAscendance) };
					command_label.text
						.concatenate(runes_t{ " ["})
						.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform(discount_e::NecromanticAscendance) ? colors::Green : colors::Red })
						.concatenate(runes_t{ "/" })
						.concatenate(runes_t{ std::format("{}", player.get_cost(discount_e::NecromanticAscendance)) })
						.concatenate(runes_t{ "]" });
					
						command_label.position = (necromantic_ascendance_icon_position() + offset_t{ 1, 1 }) * globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + offset_t{ 2, 0 };
				}

				command_label.position.y = mouse_pos.y / globals::cell_size<grid_type_e::UI>.h;
			}

			show_depth = show_depth ? depth_expanded_label.is_hovered() : depth_hidden_label.is_hovered();

			if (show_depth) {
				depth_expanded_label.text = runes_t{
					std::format(
						"Minion Kills: {:4}\n\n"
						"Player Kills: {:4}\n\n\n"
						"Total Kills:  {:4}\n\n\n"
						"Score:  {} (x{:2.1f})\n\n\n"
						"    Depth: {:3}    ",
						
						game_stats.minion_kills,
						game_stats.player_kills,
						game_stats.total_kills(),
						scorekeeper.get_score(), scorekeeper.current_multiplier(),
						static_cast<isize>(game_stats.game_depth) * -1
					),
					colors::White,
				};
			} else {
				depth_hidden_label.text = runes_t{ std::format("Depth: {:3}", static_cast<isize>(game_stats.game_depth) * -1) };
			}

			show_favor = show_favor ? favor_expanded_label.is_hovered() : favor_hidden_label.is_hovered();

			if (show_favor) {
				const patron_e current_patron{ player.patron };
				const disposition_e current_disposition{ get_patron_disposition(current_patron) };

				favor_expanded_label.text = runes_t{ std::format("Patron:{}", std::string(padding_size(current_patron), ' ')), colors::White };

				favor_expanded_label.text.concatenate(necrowarp::to_colored_string(current_patron));

				favor_expanded_label.text.concatenate(runes_t{ std::format("\n\nDisposition:{}", std::string(padding_size(current_disposition), ' ')), colors::White });

				favor_expanded_label.text.concatenate(necrowarp::to_colored_string(current_disposition));

				favor_expanded_label.text.concatenate(runes_t{ "\n\n\n", colors::White });

				magic_enum::enum_for_each<discount_e>([&](auto val) -> void {
					constexpr discount_e cval{ val };
					const i8 value{ player.get_discount(cval) };
					const discount_type_e type{ player.get_discount_type(value) };

					favor_expanded_label.text.concatenate(to_colored_string(cval, value, type));

					if constexpr (cval == discount_e::TargetWarp || cval == discount_e::Annihilate || cval == discount_e::GalvanicInvocation || cval == discount_e::CalamitousRetaliation) {
						favor_expanded_label.text.concatenate(runes_t{ "\n\n\n" });
					} else {
						favor_expanded_label.text.concatenate(runes_t{ "\n\n" });
					}
				});

				const extent_t current_size{ text::calculate_size(favor_expanded_label.text) };

				favor_expanded_label.text.concatenate(runes_t{ std::format(" Favor{}", std::string(current_size.w - 6, ' ')) });
			} else {
				favor_hidden_label.text = runes_t{ " Favor " };
			}

			registry_access.lock();

			const bool has_entity{ entity_registry<MapType>.contains(grid_cursor<MapType>.current_position) };
			const bool has_object{ object_registry<MapType>.contains(grid_cursor<MapType>.current_position) };

			const fluid_e fluid{ fluid_map<MapType>[grid_cursor<MapType>.current_position] };

			show_tooltip = has_entity || has_object || fluid != fluid_e::None;

			if (show_tooltip) {
				tooltip_label<MapType>.text = runes_t{};

				if (has_entity) {
					tooltip_label<MapType>.text.concatenate(to_colored_string<MapType>(entity_registry<MapType>.at(grid_cursor<MapType>.current_position), grid_cursor<MapType>.current_position));
				}

				if (has_entity && has_object) {
					tooltip_label<MapType>.text.concatenate({ " | " });
				}

				if (has_object) {
					tooltip_label<MapType>.text.concatenate(to_colored_string<MapType>(object_registry<MapType>.at(grid_cursor<MapType>.current_position), grid_cursor<MapType>.current_position));
				}

				if ((has_entity || has_object) && fluid != fluid_e::None) {
					tooltip_label<MapType>.text.concatenate({ " | " });
				}
				
				if (fluid != fluid_e::None) {
					tooltip_label<MapType>.text.concatenate(to_colored_string(fluid));
				}
			}

			if (has_entity) {
				if (entity_registry<MapType>.dependent contains<player_t>(grid_cursor<MapType>.current_position)) {
					grid_cursor<MapType>.color = colors::Magenta;
				} else if (entity_registry<MapType>.dependent contains<ALL_EVIL_NPCS>(grid_cursor<MapType>.current_position)) {
					grid_cursor<MapType>.color = colors::Green;
				} else if (entity_registry<MapType>.dependent contains<ALL_GOOD_NPCS>(grid_cursor<MapType>.current_position)) {
					grid_cursor<MapType>.color = colors::Red;
				} else {
					grid_cursor<MapType>.color = colors::metals::Gold;
				}
			} else if (has_object) {
				grid_cursor<MapType>.color = colors::Blue;
			} else if (fluid != fluid_e::None) {
				grid_cursor<MapType>.color = fluid_color(fluid);
			} else {
				grid_cursor<MapType>.color = colors::metals::Gold;
			}

			registry_access.unlock();
		}

		template<map_type_e MapType> static inline void draw(ref<renderer_t> renderer) noexcept {
			player_statuses.draw(renderer);

			if (show_advancement) {
				advancement_label.draw(renderer);
			}

			icon_atlas.draw(glyph_t{ icons::RandomWarp.index, player.can_perform(discount_e::RandomWarp) ? colors::White : colors::dark::Grey }, random_warp_icon_position());
			icon_atlas.draw(glyph_t{ icons::TargetWarp.index, player.can_perform(discount_e::TargetWarp) ? colors::White : colors::dark::Grey }, target_warp_icon_position());

			icon_atlas.draw(glyph_t{ icons::CalciticInvocation.index, player.can_perform(discount_e::CalciticInvocation) ? colors::White : colors::dark::Grey }, calcitic_invocation_icon_position());
			icon_atlas.draw(glyph_t{ icons::SpectralInvocation.index, player.can_perform(discount_e::SpectralInvocation) ? colors::White : colors::dark::Grey }, spectral_invocation_icon_position());
			icon_atlas.draw(glyph_t{ icons::SanguineInvocation.index, player.can_perform(discount_e::SanguineInvocation) ? colors::White : colors::dark::Grey }, sanguine_invocation_icon_position());
			icon_atlas.draw(glyph_t{ icons::GalvanicInvocation.index, player.can_perform(discount_e::GalvanicInvocation) ? colors::White : colors::dark::Grey }, galvanic_invocation_icon_position());

			icon_atlas.draw(glyph_t{ icons::NecromanticAscendance.index, player.can_perform(discount_e::NecromanticAscendance) ? colors::White : colors::dark::Grey }, necromantic_ascendance_icon_position());

			if (show_command) {
				command_label.draw(renderer);
			}

			show_depth ?
				depth_expanded_label.draw(renderer) :
				depth_hidden_label.draw(renderer);

			show_favor ?
				favor_expanded_label.draw(renderer) :
				favor_hidden_label.draw(renderer);

			if (!draw_cursor && show_tooltip) {
				tooltip_label<MapType>.draw(renderer);
			}

			registry_access.lock();

			minimap<MapType>.draw(renderer);

			registry_access.unlock();
		};
	};
} // namespace necrowarp
