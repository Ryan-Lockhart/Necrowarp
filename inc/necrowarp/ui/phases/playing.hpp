#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	constexpr color_t ActiveEnergyColor{ colors::Magenta };
	constexpr color_t InactiveEnergyColor{ ActiveEnergyColor, u8{ 0x80 } };

	constexpr color_t ActiveArmorColor{ colors::Marble };
	constexpr color_t InactiveArmorColor{ ActiveArmorColor, u8{ 0x80 } };

	constexpr color_t ActiveDivinityColor{ colors::metals::Gold };
	constexpr color_t InactiveDivinityColor{ ActiveDivinityColor, u8{ 0x80 } };

	constexpr color_t ActivePhantasmColor{ mix(colors::Blue, colors::metals::Silver) };
	constexpr color_t InactivePhantasmColor{ ActivePhantasmColor, u8{ 0x80 } };

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
		"Chaotic Warp:             0000\n\n"
		"Precise Warp:             0000\n\n\n"
		"Calcitic Invocation:     0000\n\n"
		"Spectral Invocation:     0000\n\n"
		"Sanguine Invocation:     0000\n\n\n"
		"Necromantic Ascendance:  0000\n\n\n"
		" Favor                       "
	};

	template<> struct phase_state_t<phase_e::Playing> {
		static inline status_bar_t<4> player_statuses{
			anchor_t{ offset_t{ 1, 1 }, cardinal_e::Northwest},
			std::array<status_t, 4>{
				status_t{ runes_t{ "  energy: ", ActiveEnergyColor }, player_t::MaximumEnergy, ActiveEnergyColor, InactiveEnergyColor },
				status_t{ runes_t{ "   armor: ", ActiveArmorColor }, player_t::MaximumArmor, ActiveArmorColor, InactiveArmorColor },
				status_t{ runes_t{ "phantasm: ", ActivePhantasmColor }, player_t::MaximumPhantasm, ActivePhantasmColor, InactivePhantasmColor },
				status_t{ runes_t{ "divinity: ", ActiveDivinityColor }, player_t::MaximumDivinity, ActiveDivinityColor, InactiveDivinityColor }
			},
			embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		};

		static inline command_bar_t<7> player_commands{
			anchor_t{ offset_t{ 0, 0 }, cardinal_e::West },
			std::array<embedded_command_icon_t, 7>{
				embedded_command_icon_t{ icons::ChaoticWarp, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },
				embedded_command_icon_t{ icons::PreciseWarp, embedded_box_t{ colors::Black, border_t{ colors::White, 1 } }, extent_t{ 1, 1 } },

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
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>() }, cardinal_e::Southeast },
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

		static inline bool show_big_map{ false };

		static constexpr extent_t MinimapPixelSize{ 2, 2 };
		static constexpr extent_t BigMapPixelSize{ 4, 4 };

		template<map_type_e MapType> static inline minimap_t<MapType, MinimapPixelSize> minimap{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w * globals::cell_size<grid_type_e::UI>.w, 1 }, cardinal_e::Northeast },
			embedded_box_t{ colors::Black, border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		};

		template<map_type_e MapType> static inline minimap_t<MapType, BigMapPixelSize> big_map{
			anchor_t{ offset_t{ globals::grid_size<grid_type_e::UI>().w * globals::cell_size<grid_type_e::UI>.w, 1 }, cardinal_e::Northeast },
			embedded_box_t{ colors::Black, border_t{ colors::White, 2 } },
			extent_t{ 2, 2 }
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

		template<grimoire_e Command> static constexpr offset_t icon_position();

		template<> inline constexpr offset_t icon_position<grimoire_e::SpectralInvocation>() {
			return offset_t{
				globals::grid_size<grid_type_e::Icon>().w * 0.5 - 1,
				globals::grid_size<grid_type_e::Icon>().h - 1
			};
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::CalciticInvocation>() {
			return icon_position<grimoire_e::SpectralInvocation>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::Incorporealize>() {
			return icon_position<grimoire_e::CalciticInvocation>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::Calcify>() {
			return icon_position<grimoire_e::Incorporealize>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::Repulse>() {
			return icon_position<grimoire_e::Calcify>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::Annihilate>() {
			return icon_position<grimoire_e::Repulse>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::PreciseWarp>() {
			return icon_position<grimoire_e::Annihilate>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::ChaoticWarp>() {
			return icon_position<grimoire_e::PreciseWarp>() - offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::SanguineInvocation>() {
			return icon_position<grimoire_e::SpectralInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::GalvanicInvocation>() {
			return icon_position<grimoire_e::SanguineInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::RavenousInvocation>() {
			return icon_position<grimoire_e::GalvanicInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::WretchedInvocation>() {
			return icon_position<grimoire_e::RavenousInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::CerebralInvocation>() {
			return icon_position<grimoire_e::WretchedInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::InfernalInvocation>() {
			return icon_position<grimoire_e::CerebralInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::NecromanticAscendance>() {
			return icon_position<grimoire_e::InfernalInvocation>() + offset_t{ 1, 0 };
		}

		template<> inline constexpr offset_t icon_position<grimoire_e::CalamitousRetaliation>() {
			return icon_position<grimoire_e::NecromanticAscendance>() + offset_t{ 1, 0 };
		}

		static constexpr bool any_icon_hovered() noexcept {
			bool hovered{ false };

			magic_enum::enum_for_each<grimoire_e>([&](auto val) {
				constexpr grimoire_e cval{ val };

				if (grimoire_s<cval>::can_use() && mouse_s::is_inside(icon_position<cval>() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					hovered = true;
				}
			});

			return hovered;
		}

		static constexpr std::optional<grimoire_e> get_hovered_icon() noexcept {
			std::optional<grimoire_e> hovered{ std::nullopt };

			magic_enum::enum_for_each<grimoire_e>([&](auto val) {
				constexpr grimoire_e cval{ val };

				if (grimoire_s<cval>::can_use() && mouse_s::is_inside(icon_position<cval>() * globals::cell_size<grid_type_e::Icon>, globals::cell_size<grid_type_e::Icon>)) {
					hovered = cval;
				}
			});

			return hovered;
		}

		template<map_type_e MapType> static constexpr void update_command() noexcept {
			const std::optional<grimoire_e> command{ get_hovered_icon() };

			if (!command.has_value()) {
				return;
			}

			magic_enum::enum_switch([&](auto val) {
				constexpr grimoire_e cval{ val };
				
				const bool can_perform{ player.can_perform<MapType>(cval) };

				const bool blocked{ mansling_t::in_range<MapType>() };

				command_label.text = runes_t{ to_string(cval) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), can_perform ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.get_cost(cval)) });

				if (!can_perform && blocked) {
					command_label.text
						.concatenate(runes_t{ " (" })
						.concatenate(runes_t{ "blocked", colors::Red })
						.concatenate(runes_t{ ")"});
				}

				command_label.text.concatenate(runes_t{ std::format("] (uses: {})", grimoire_s<cval>::get_uses()) });
			}, command.value());
		}

		template<map_type_e MapType> static constexpr void draw_commands() noexcept {
			magic_enum::enum_for_each<grimoire_e>([&](auto val) {
				constexpr grimoire_e cval{ val };

				if (grimoire_s<cval>::can_use()) {
					icon_atlas.draw(glyph_t{ static_cast<u16>(cval), player.can_perform<MapType>(cval) ? colors::White : colors::dark::Grey }, icon_position<cval>());
				}
			});
		}

		template<map_type_e MapType> static inline bool any_hovered() noexcept {
			if (phase.current_phase != phase_e::Playing) {
				return false;
			}
			
			return player_statuses.is_hovered() || any_icon_hovered() || advancement_label.is_hovered() || (show_depth ? depth_expanded_label.is_hovered() : depth_hidden_label.is_hovered()) || (show_favor ? favor_expanded_label.is_hovered() : favor_hidden_label.is_hovered()) || (show_big_map ? big_map<MapType>.is_hovered() : minimap<MapType>.is_hovered());
		}

		template<map_type_e MapType> static inline void update(button_e button) noexcept {
			if (phase.current_phase != phase_e::Playing) {
				return;
			}

			player_statuses[0].current_value = player.get_energy();
			player_statuses[0].max_value = player.max_energy();

			player_statuses[1].current_value = player.get_armor();
			player_statuses[1].max_value = player.max_armor();

			player_statuses[2].current_value = player.get_phantasm();
			player_statuses[2].max_value = player.max_phantasm();

			player_statuses[3].current_value = player.get_divinity();
			player_statuses[3].max_value = player.max_divinity();

			const i8 kills_energy{ game_stats.kills_until_next_energy_slot() };
			const i8 kills_armor{ game_stats.kills_until_next_armor_slot() };
			const i8 kills_phantasm{ game_stats.kills_until_next_phantasm_turn() };
			const i8 kills_divinity{ game_stats.kills_until_next_divinity_turn() };

			show_advancement = player_statuses.is_hovered() || advancement_label.is_hovered();

			if (show_advancement) {
				advancement_label.position = player_statuses.position + extent_t{ player_statuses.external_size().w, 0 };
				advancement_label.alignment = player_statuses.alignment;

				advancement_label.text = runes_t{
					std::format(
						"[{}/{}]{}\n\n"
						"[{}/{}]{}\n\n"
						"[{}/{}]{}\n\n"
						"[{}/{}]{}",

						player.get_energy(), player.max_energy(), player.max_energy() >= player_t::MaximumEnergy ? "" : std::format(" (+1 energy slot in {} minion kill{})", kills_energy == 0 ? globals::KillsPerEnergySlot : kills_energy, kills_energy == 1 ? "" : "s"),
						player.get_armor(), player.max_armor(), player.max_armor() >= player_t::MaximumArmor ? "" : std::format(" (+1 armor slot in {} player kill{})", kills_armor == 0 ? globals::KillsPerArmorSlot : kills_armor, kills_armor == 1 ? "" : "s"),
						player.get_phantasm(), player.max_phantasm(), player.max_phantasm() >= player_t::MaximumPhantasm ? "" : std::format(" (+1 phantasm turn in {} total kill{})", kills_phantasm == 0 ? globals::KillsPerPhantasmTurn : kills_phantasm, kills_phantasm == 1 ? "" : "s"),
						player.get_divinity(), player.max_divinity(), player.max_divinity() >= player_t::MaximumDivinity ? "" : std::format(" (+1 divinity turn in {} total kill{})", kills_divinity == 0 ? globals::KillsPerDivinityTurn : kills_divinity, kills_divinity == 1 ? "" : "s")
					)
				};
			}

			show_command = any_icon_hovered();

			if (show_command) {
				update_command<MapType>();
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

				magic_enum::enum_for_each<grimoire_e>([&](auto val) -> void {
					constexpr grimoire_e cval{ val };
					const i8 value{ player.get_discount(cval) };
					const discount_type_e type{ player.get_discount_type(value) };

					favor_expanded_label.text.concatenate(to_colored_string(cval, value, type));

					if constexpr (cval == grimoire_e::PreciseWarp || cval == grimoire_e::Incorporealize || cval == grimoire_e::InfernalInvocation || cval == grimoire_e::CalamitousRetaliation) {
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

			if (registry_access.try_lock()) {
				const bool has_entity{ entity_registry<MapType>.contains(grid_cursor<MapType>.current_position) };
				const bool has_object{ object_registry<MapType>.contains(grid_cursor<MapType>.current_position) };

				const fluid_e fluid{ fluid_map<MapType>[grid_cursor<MapType>.current_position] };

				show_tooltip = has_entity || has_object || fluid != fluid_e::None;

				if (show_tooltip) {
					tooltip_label<MapType>.text = runes_t{};

					if (has_entity) {
						const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(grid_cursor<MapType>.current_position) };

						if (maybe_entity.has_value()) {
							tooltip_label<MapType>.text.concatenate(
								magic_enum::enum_switch(
									[&](auto val) -> runes_t {
										constexpr entity_e cval{ val };

										using entity_type = typename to_entity_type<cval>::type;

										if constexpr (globals::has_unique_descriptor<entity_type>::value) {
											cauto entity_ptr{ entity_registry<MapType>.dependent at<entity_type>(grid_cursor<MapType>.current_position) };

											if (entity_ptr != nullptr) {
												return entity_ptr->to_colored_string();
											}
										}

										return to_colored_string(cval);
									}, maybe_entity.value()
								)
							);
						}
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
					} else if (entity_registry<MapType>.dependent contains<ALL_GOOD_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Red;
					} else if (entity_registry<MapType>.dependent contains<ALL_NEUTRAL_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Orange;
					} else if (entity_registry<MapType>.dependent contains<ALL_EVIL_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Green;
					} else {
						grid_cursor<MapType>.color = colors::metals::Gold;
					}
				} else if (has_object) {
					grid_cursor<MapType>.color = colors::Blue;
				} else if (fluid != fluid_e::None) {
					grid_cursor<MapType>.color = to_color(fluid);
				} else {
					grid_cursor<MapType>.color = colors::metals::Gold;
				}

				registry_access.unlock();
			} else if (buffer_access.try_lock()) {
				const bool has_entity{ entity_buffer<MapType>.contains(grid_cursor<MapType>.current_position) };
				const bool has_object{ object_buffer<MapType>.contains(grid_cursor<MapType>.current_position) };

				const fluid_e fluid{ fluid_map<MapType>[grid_cursor<MapType>.current_position] };

				show_tooltip = has_entity || has_object || fluid != fluid_e::None;

				if (show_tooltip) {
					tooltip_label<MapType>.text = runes_t{};

					if (has_entity) {
						const std::optional<entity_e> maybe_entity{ entity_buffer<MapType>.at(grid_cursor<MapType>.current_position) };

						if (maybe_entity.has_value()) {
							tooltip_label<MapType>.text.concatenate(
								magic_enum::enum_switch(
									[&](auto val) -> runes_t {
										constexpr entity_e cval{ val };

										using entity_type = typename to_entity_type<cval>::type;

										if constexpr (globals::has_unique_descriptor<entity_type>::value) {
											cauto entity_ptr{ entity_buffer<MapType>.dependent at<entity_type>(grid_cursor<MapType>.current_position) };

											if (entity_ptr != nullptr) {
												return entity_ptr->to_colored_string();
											}
										}

										return to_colored_string(cval);
									}, maybe_entity.value()
								)
							);
						}
					}

					if (has_entity && has_object) {
						tooltip_label<MapType>.text.concatenate({ " | " });
					}

					if (has_object) {
						tooltip_label<MapType>.text.concatenate(to_colored_string<MapType>(object_buffer<MapType>.at(grid_cursor<MapType>.current_position), grid_cursor<MapType>.current_position));
					}

					if ((has_entity || has_object) && fluid != fluid_e::None) {
						tooltip_label<MapType>.text.concatenate({ " | " });
					}
					
					if (fluid != fluid_e::None) {
						tooltip_label<MapType>.text.concatenate(to_colored_string(fluid));
					}
				}

				if (has_entity) {
					if (entity_buffer<MapType>.dependent contains<player_t>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Magenta;
					} else if (entity_buffer<MapType>.dependent contains<ALL_GOOD_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Red;
					} else if (entity_buffer<MapType>.dependent contains<ALL_NEUTRAL_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Orange;
					} else if (entity_buffer<MapType>.dependent contains<ALL_EVIL_NPCS>(grid_cursor<MapType>.current_position)) {
						grid_cursor<MapType>.color = colors::Green;
					} else {
						grid_cursor<MapType>.color = colors::metals::Gold;
					}
				} else if (has_object) {
					grid_cursor<MapType>.color = colors::Blue;
				} else if (fluid != fluid_e::None) {
					grid_cursor<MapType>.color = to_color(fluid);
				} else {
					grid_cursor<MapType>.color = colors::metals::Gold;
				}

				buffer_access.unlock();
			}

			if (show_big_map) {
				big_map<MapType>.update(button_e::Left);
			} else {
				minimap<MapType>.update(button_e::Left);
			}
		}

		template<map_type_e MapType> static inline void draw(ref<renderer_t> renderer) noexcept {
			player_statuses.draw(renderer);

			if (show_advancement) {
				advancement_label.draw(renderer);
			}

			draw_commands<MapType>();

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

			if (registry_access.try_lock()) {
				if (show_big_map) {
					big_map<MapType>.draw(renderer, entity_registry<MapType>, object_registry<MapType>);
				} else {
					minimap<MapType>.draw(renderer, entity_registry<MapType>, object_registry<MapType>);
				}

				registry_access.unlock();
			} else if (buffer_access.try_lock()) {
				if (show_big_map) {
					big_map<MapType>.draw(renderer, entity_buffer<MapType>, object_buffer<MapType>);
				} else {
					minimap<MapType>.draw(renderer, entity_buffer<MapType>, object_buffer<MapType>);
				}

				buffer_access.unlock();
			}
		};
	};
} // namespace necrowarp
