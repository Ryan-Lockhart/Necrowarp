#pragma once

#include <bleak.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	enum struct patron_e : u8 {
		None,
		Rathghul,
		Akurakhaithan,
		Merirfin,
		Saeiligarkeuss
	};

	constexpr usize padding_size(patron_e patron) noexcept {
		switch (patron) {
			case patron_e::None: {
				return 15;
			} case patron_e::Rathghul: {
				return 11;
			} case patron_e::Akurakhaithan: {
				return 6;
			} case patron_e::Merirfin: {
				return 11;
			} case patron_e::Saeiligarkeuss: {
				return 5;
			}
		}
	}

	constexpr cstr to_string(patron_e patron) noexcept {
		switch (patron) {
			case patron_e::None: {
				return "None";
			} case patron_e::Rathghul: {
				return "Rathghul";
			} case patron_e::Akurakhaithan: {
				return "Akurakhaithan";
			} case patron_e::Merirfin: {
				return "Merirfin";
			} case patron_e::Saeiligarkeuss: {
				return "Saeiligarkeuss";
			}
		}
	}

	constexpr runes_t to_colored_string(patron_e patron) noexcept {
		const cstr string{ to_string(patron) };
		
		switch (patron) {
			case patron_e::None: {
				return runes_t{ string, colors::light::Grey };
			} case patron_e::Rathghul: {
				return runes_t{ string, colors::light::Green };
			} case patron_e::Akurakhaithan: {
				return runes_t{ string, colors::light::Magenta };
			} case patron_e::Merirfin: {
				return runes_t{ string, colors::light::Red };
			} case patron_e::Saeiligarkeuss: {
				return runes_t{ string, colors::dark::Orange };
			}
		}
	}

	enum struct disposition_e : u8 {
		Sadistic,
		Apathetic,
		Cooperative,
	};

	constexpr usize padding_size(disposition_e disposition) noexcept {
		switch (disposition) {
			case disposition_e::Sadistic: {
				return 6;
			} case disposition_e::Apathetic: {
				return 5;
			} case disposition_e::Cooperative: {
				return 3;
			}
		}
	}

	constexpr cstr to_string(disposition_e disposition) noexcept {
		switch (disposition) {
			case disposition_e::Sadistic: {
				return "Sadistic";
			} case disposition_e::Apathetic: {
				return "Apathetic";
			} case disposition_e::Cooperative: {
				return "Cooperative";
			}
		}
	}

	constexpr runes_t to_colored_string(disposition_e disposition) noexcept {
		const cstr string{ to_string(disposition) };

		switch (disposition) {
			case disposition_e::Sadistic: {
				return runes_t{ string, colors::Red} ;
			} case disposition_e::Apathetic: {
				return runes_t{ string, colors::Yellow };
			} case disposition_e::Cooperative: {
				return runes_t{ string, colors::Green };
			}
		}
	}

	enum struct discount_e : u8 {
		RandomWarp,				// warp to a random, preferrably safe, position within the interior of the map; if not free and out of energy can still be used dangerously
		TargetWarp,				// warp to a specific position within the interior of the map

		Calcify,				// single target utility command; convert an open tile with bones into a temporary wall
	 	Repulse,				// circular area of effect combat command; bequeathed to devotees of the Moslager
	 	Annihilate,				// linear area of effect combat command; bequeathed to devotees of Merirfin

		CalciticInvocation,		// domain of bones; patrons are Kalypdrot, Rathghul, and Ionna; minions are skeletons and bonespurs
		SpectralInvocation,		// domain of ichor; patrons are Akurakhaithan, Rathghul, and Saeiligarkeuss; minions are cultists and wraithes
		SanguineInvocation,		// domain of blood; patrons are Viedskavn, Merirfin, and Neolithia; minions are bloodhounds and hemogheists
		GalvanicInvocation,		// domain of metal; patrons are Thuljanor, Praethornyn, and Exar; minions are animated suits of armor and death knights
		RavenousInvocation,		// domain of flesh; patrons are Tselgwedixxikog, Sketzuum, and Praethornyn; minions are abominations and flesh golems
	 	WretchedInvocation,		// domain of filth; patrons are Ionna, Kalypdrot, and Rathghul; minions are draugr and dreadwurms
	 // AbyssalInvocation,		// domain of cerebra; patrons are Saeiligarkeuss, the Moslager, and Akurakhaithan; minions are hamr, furtive horrors, chromalesia, and isosceline

		MaliceIncarnate,		// discard body temporarily and enter wraith-world becoming untargetable and undetectable; bequeathed to devotees of Rathghul
		NecromanticAscendance,	// expend massive amount of energy for invulnerability and empowered invocations; bequeathed to devotees of Saeiligarkeuss
		CalamitousRetaliation, 	// kill all entities on the map, regardless of allegiance; bequeathed to devotees of Neolithia
	};

	constexpr usize padding_size(discount_e discount) noexcept {
		switch (discount) {
			case discount_e::RandomWarp: {
				return 12;
			} case discount_e::TargetWarp: {
				return 12;
			} case discount_e::Calcify: {
				return 16;
			} case discount_e::Repulse: {
				return 16;
			} case discount_e::Annihilate: {
				return 13;
			} case discount_e::CalciticInvocation: {
				return 4;
			} case discount_e::SpectralInvocation: {
				return 4;
			} case discount_e::SanguineInvocation: {
				return 4;
			} case discount_e::GalvanicInvocation: {
				return 4;
			} case discount_e::RavenousInvocation: {
				return 4;
			} case discount_e::WretchedInvocation: {
				return 4;
			} case discount_e::MaliceIncarnate: {
				return 7;
			} case discount_e::NecromanticAscendance: {
				return 1;
			} case discount_e::CalamitousRetaliation: {
				return 1;
			}
		}
	}

	constexpr cstr to_string(discount_e discount) noexcept {
		switch (discount) {
			case discount_e::RandomWarp: {
				return "Random Warp";
			} case discount_e::TargetWarp: {
				return "Target Warp";
			} case discount_e::Calcify: {
				return "Calcify";
			} case discount_e::Repulse: {
				return "Repulse";
			} case discount_e::Annihilate: {
				return "Annihilate";
			} case discount_e::CalciticInvocation: {
				return "Calcitic Invocation";
			} case discount_e::SpectralInvocation: {
				return "Spectral Invocation";
			} case discount_e::SanguineInvocation: {
				return "Sanguine Invocation";
			} case discount_e::GalvanicInvocation: {
				return "Galvanic Invocation";
			} case discount_e::RavenousInvocation: {
				return "Ravenous Invocation";
			} case discount_e::WretchedInvocation: {
				return "Wretched Invocation";
			} case discount_e::MaliceIncarnate: {
				return "Malice Incarnate";
			} case discount_e::NecromanticAscendance: {
				return "Necromantic Ascendance";
			} case discount_e::CalamitousRetaliation: {
				return "Calamitous Retaliation";
			}
		}
	}

	enum struct discount_type_e : u8 {
		Malus,
		Placebo,
		Boon
	};

	constexpr cstr to_string(discount_type_e type) noexcept {
		switch (type) {
			case discount_type_e::Malus: {
				return "malus";
			} case discount_type_e::Placebo: {
				return "placebo";
			} case discount_type_e::Boon: {
				return "boon";
			}
		}
	}

	constexpr runes_t to_colored_string(i8 value, discount_type_e type) noexcept {
		runes_t colored_string{};

		switch (type) {
			case discount_type_e::Malus: {
				return colored_string.concatenate(runes_t{ std::format("+{} ", abs(value)), colors::Red });
			} case discount_type_e::Placebo: {
				return colored_string.concatenate(runes_t{ " 0 ", colors::Yellow });
			} case discount_type_e::Boon: {
				return colored_string.concatenate(runes_t{ std::format("-{} ", abs(value)), colors::Green });
			}
		}
	}

	constexpr runes_t to_colored_string(discount_e discount, i8 value, discount_type_e type) noexcept {
		runes_t colored_string{ std::format("{}:{}", to_string(discount), std::string(padding_size(discount), ' ')) };

		switch (type) {
			case discount_type_e::Malus: {
				return colored_string.concatenate(runes_t{ std::format("+{} ", abs(value)), colors::Red });
			} case discount_type_e::Placebo: {
				return colored_string.concatenate(runes_t{ " 0 ", colors::Yellow });
			} case discount_type_e::Boon: {
				return colored_string.concatenate(runes_t{ std::format("-{} ", abs(value)), colors::Green });
			}
		}
	}

	template<patron_e Patron> constexpr runes_t to_colored_string() noexcept;

	struct discount_t {
		const i8 negative{ 0 };
		const i8 neutral{ 0 };
		const i8 positive{ 0 };
		
		constexpr i8 current(disposition_e disposition) const noexcept {
			switch (disposition) {
				case disposition_e::Sadistic: {
					return negative;
				} case disposition_e::Apathetic: {
					return neutral;
				} case disposition_e::Cooperative: {
					return positive;
				}
			}
		}
	};

	struct patron_t {
		disposition_e disposition{ disposition_e::Apathetic };

		const discount_t random_warp{};
		const discount_t target_warp{};

		const discount_t calcify{};
		const discount_t repulse{};
		const discount_t annihilate{};

		const discount_t calcitic_invocation{};
		const discount_t spectral_invocation{};
		const discount_t sanguine_invocation{};
		const discount_t galvanic_invocation{};
		const discount_t ravenous_invocation{};
		const discount_t wretched_invocation{};

		const discount_t malice_incarnate{};
		const discount_t necromantic_ascendance{};
		const discount_t calamitous_retaliation{};

		template<discount_e Discount> constexpr discount_t get_discount() const noexcept {
			switch (Discount) {
				case discount_e::RandomWarp: {
					return random_warp;
				} case discount_e::TargetWarp: {
					return target_warp;
				} case discount_e::Calcify: {
					return calcify;
				} case discount_e::Repulse: {
					return repulse;
				} case discount_e::Annihilate: {
					return annihilate;
				} case discount_e::CalciticInvocation: {
					return calcitic_invocation;
				} case discount_e::SpectralInvocation: {
					return spectral_invocation;
				} case discount_e::SanguineInvocation: {
					return sanguine_invocation;
				} case discount_e::GalvanicInvocation: {
					return galvanic_invocation;
				} case discount_e::RavenousInvocation: {
					return ravenous_invocation;
				} case discount_e::WretchedInvocation: {
					return wretched_invocation;
				} case discount_e::MaliceIncarnate: {
					return malice_incarnate;
				} case discount_e::NecromanticAscendance: {
					return necromantic_ascendance;
				} case discount_e::CalamitousRetaliation: {
					return calamitous_retaliation;
				}
			}
		}
	};

	template<patron_e Patron> static inline patron_t patrons;

	template<> inline patron_t patrons<patron_e::None>{
		.disposition = disposition_e::Apathetic,
		.random_warp = discount_t{ 0, 1, 2 },
		.target_warp = discount_t{ 0, 2, 4 },
		.calcify = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.annihilate = discount_t{ 0, 0, 0 },
		.calcitic_invocation = discount_t{ 0, 0, 0 },
		.spectral_invocation = discount_t{ 0, 0, 0 },
		.sanguine_invocation = discount_t{ 0, 0, 0 },
		.galvanic_invocation = discount_t{ 0, 0, 0 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.malice_incarnate = discount_t{ 0, 0, 0 },
		.necromantic_ascendance = discount_t{ 0, 0, 0 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	template<> inline patron_t patrons<patron_e::Rathghul>{
		.disposition = disposition_e::Apathetic,
		.random_warp = discount_t{ 0, 0, 1 },
		.target_warp = discount_t{ 0, 0, 2 },
		.calcify = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.annihilate = discount_t{ 0, 0, 0 },
		.calcitic_invocation = discount_t{ 0, 4, 6 },
		.spectral_invocation = discount_t{ -6, -2, 2 },
		.sanguine_invocation = discount_t{ -4, 0, 4 },
		.galvanic_invocation = discount_t{ -4, 0, 4 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.malice_incarnate = discount_t{ 0, 0, 0 },
		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	template<> inline patron_t patrons<patron_e::Akurakhaithan>{
		.disposition = disposition_e::Apathetic,
		.random_warp = discount_t{ 0, 0, 1 },
		.target_warp = discount_t{ 0, 0, 2 },
		.calcify = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.annihilate = discount_t{ 0, 0, 0 },
		.calcitic_invocation = discount_t{ -4, 0, 4 },
		.spectral_invocation = discount_t{ 0, 4, 6 },
		.sanguine_invocation = discount_t{ -6, -2, 2 },
		.galvanic_invocation = discount_t{ -4, 0, 4 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.malice_incarnate = discount_t{ 0, 0, 0 },
		.necromantic_ascendance = discount_t{ -4, 0, 8 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	template<> inline patron_t patrons<patron_e::Merirfin>{
		.disposition = disposition_e::Apathetic,
		.random_warp = discount_t{ 0, 0, 1 },
		.target_warp = discount_t{ 0, 0, 2 },
		.calcify = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.annihilate = discount_t{ 0, 0, 0 },
		.calcitic_invocation = discount_t{ -4, 0, 4 },
		.spectral_invocation = discount_t{ -6, -2, 2 },
		.sanguine_invocation = discount_t{ 0, 4, 6 },
		.galvanic_invocation = discount_t{ -4, 0, 4 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.malice_incarnate = discount_t{ 0, 0, 0 },
		.necromantic_ascendance = discount_t{ -6, 0, 6 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	template<> inline patron_t patrons<patron_e::Saeiligarkeuss>{
		.disposition = disposition_e::Apathetic,
		.random_warp = discount_t{ -1, 1, 2 },
		.target_warp = discount_t{ -2, 2, 4 },
		.calcify = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.annihilate = discount_t{ 0, 0, 0 },
		.calcitic_invocation = discount_t{ -8, 0, 4 },
		.spectral_invocation = discount_t{ -8, 0, 4 },
		.sanguine_invocation = discount_t{ -8, 0, 4 },
		.galvanic_invocation = discount_t{ -6, 0, 6 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.malice_incarnate = discount_t{ 0, 0, 0 },
		.necromantic_ascendance = discount_t{ 0, 8, 16 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	constexpr discount_type_e get_discount_type(i8 value) {
		return value == 0 ? discount_type_e::Placebo : value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
	}

	template<patron_e Patron> constexpr runes_t to_colored_string(discount_e discount) noexcept {
		runes_t colored_string{};

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr discount_e discount_cval{ val };
			
			cauto discount_info{ patrons<Patron>.dependent get_discount<discount_cval>() };

			colored_string.concatenate(to_colored_string(discount_info.negative, get_discount_type(discount_info.negative)));

			colored_string.concatenate(runes_t{ " | " });

			colored_string.concatenate(to_colored_string(discount_info.neutral, get_discount_type(discount_info.neutral)));

			colored_string.concatenate(runes_t{ " | " });

			colored_string.concatenate(to_colored_string(discount_info.positive, get_discount_type(discount_info.positive)));
		}, discount);

		return colored_string;
	}

	static inline disposition_e get_patron_disposition(patron_e patron) noexcept {
		return magic_enum::enum_switch([&](auto val) -> disposition_e {
			constexpr patron_e cval{ val };

			return patrons<cval>.disposition;
		}, patron);
	}

	static inline void reset_patrons() noexcept {
		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			patrons<cval>.disposition = disposition_e::Apathetic;
		});
	}

	static inline void randomize_patrons() noexcept;
} //necrowarp
