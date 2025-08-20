#pragma once

#include <bleak.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/grimoire.tpp>

namespace necrowarp {
	enum struct patron_e : u8 {
		None,

		Kalypdrot,
		Akurakhaithan,
		Merirfin,
		Praethornyn,

		Tsendikor,
		Ionna,
		Rathghul,
		Moslager,

		Sudoclor
	};

	constexpr usize padding_size(patron_e patron) noexcept {
		switch (patron) {
			case patron_e::None: {
				return 15;
			} case patron_e::Kalypdrot: {
				return 10;
			} case patron_e::Akurakhaithan: {
				return 6;
			} case patron_e::Merirfin: {
				return 11;
			} case patron_e::Praethornyn: {
				return 8;
			} case patron_e::Tsendikor: {
				return 10;
			} case patron_e::Ionna: {
				return 14;
			} case patron_e::Rathghul: {
				return 11;
			} case patron_e::Moslager: {
				return 7;
			} case patron_e::Sudoclor: {
				return 11;
			}
		}
	}

	constexpr cstr to_string(patron_e patron) noexcept {
		switch (patron) {
			case patron_e::None: {
				return "None";
			} case patron_e::Kalypdrot: {
				return "Kalypdrot";
			} case patron_e::Akurakhaithan: {
				return "Akurakhaithan";
			} case patron_e::Merirfin: {
				return "Merirfin";
			} case patron_e::Praethornyn: {
				return "Praethornyn";
			} case patron_e::Tsendikor: {
				return "Tsendikor";
			} case patron_e::Ionna: {
				return "Ionna";
			} case patron_e::Rathghul: {
				return "Rathghul";
			} case patron_e::Moslager: {
				return "The Moslager";
			} case patron_e::Sudoclor: {
				return "Sudoclor";
			}
		}
	}

	constexpr color_t to_color(patron_e patron) noexcept {
		switch (patron) {
			case patron_e::None: {
				return colors::light::Grey;
			} case patron_e::Kalypdrot: {
				return colors::Marble;
			} case patron_e::Akurakhaithan: {
				return colors::light::Magenta;
			} case patron_e::Merirfin: {
				return colors::light::Red;
			} case patron_e::Praethornyn: {
				return mix(colors::Red, colors::Grey);
			} case patron_e::Tsendikor: {
				return mix(colors::Orange, colors::Grey);
			} case patron_e::Ionna: {
				return mix(colors::Green, colors::Grey);
			} case patron_e::Rathghul: {
				return colors::light::Green;
			} case patron_e::Moslager: {
				return mix(colors::metals::Steel, colors::Blue);
			} case patron_e::Sudoclor: {
				return colors::dark::Orange;
			}
		}
	}

	constexpr runes_t to_colored_string(patron_e patron) noexcept { return runes_t{ to_string(patron), to_color(patron) }; }

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

	constexpr usize padding_size(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::ChaoticWarp: {
				return 11;
			} case grimoire_e::PreciseWarp: {
				return 11;
			} case grimoire_e::Annihilate: {
				return 13;
			} case grimoire_e::Repulse: {
				return 16;
			} case grimoire_e::Calcify: {
				return 16;
			} case grimoire_e::Incorporealize: {
				return 9;
			} case grimoire_e::CalciticInvocation: {
				return 4;
			} case grimoire_e::SpectralInvocation: {
				return 4;
			} case grimoire_e::SanguineInvocation: {
				return 4;
			} case grimoire_e::GalvanicInvocation: {
				return 4;
			} case grimoire_e::RavenousInvocation: {
				return 4;
			} case grimoire_e::WretchedInvocation: {
				return 4;
			} case grimoire_e::CerebralInvocation: {
				return 4;
			} case grimoire_e::InfernalInvocation: {
				return 4;
			} case grimoire_e::NecromanticAscendance: {
				return 1;
			} case grimoire_e::CalamitousRetaliation: {
				return 1;
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
				return colored_string.concatenate(runes_t{ std::format("+{:<2}", abs(value)), colors::Red });
			} case discount_type_e::Placebo: {
				return colored_string.concatenate(runes_t{ " 0 ", colors::Yellow });
			} case discount_type_e::Boon: {
				return colored_string.concatenate(runes_t{ std::format("-{:<2}", abs(value)), colors::Green });
			}
		}
	}

	constexpr runes_t to_colored_string(grimoire_e grimoire, i8 value, discount_type_e type) noexcept {
		runes_t colored_string{ std::format("{}:{}", to_string(grimoire), std::string(padding_size(grimoire), ' ')) };

		switch (type) {
			case discount_type_e::Malus: {
				return colored_string.concatenate(runes_t{ std::format("+{:<2}", abs(value)), colors::Red });
			} case discount_type_e::Placebo: {
				return colored_string.concatenate(runes_t{ " 0  ", colors::Yellow });
			} case discount_type_e::Boon: {
				return colored_string.concatenate(runes_t{ std::format("-{:<2}", abs(value)), colors::Green });
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

	constexpr discount_t strong_invocation{ 0, 4, 6 };
	constexpr discount_t mild_invocation{ -4, 0, 4 };
	constexpr discount_t weak_invocation{ -6, -2, 2 };

	struct patron_t {
		disposition_e disposition{ disposition_e::Apathetic };

		const discount_t chaotic_warp{};
		const discount_t precise_warp{};

		const discount_t annihilate{};
		const discount_t repulse{};
		const discount_t calcify{};
		const discount_t incorporealize{};

		const discount_t calcitic_invocation{};
		const discount_t spectral_invocation{};
		const discount_t sanguine_invocation{};
		const discount_t galvanic_invocation{};
		const discount_t ravenous_invocation{};
		const discount_t wretched_invocation{};
		const discount_t cerebral_invocation{};
		const discount_t infernal_invocation{};
		
		const discount_t necromantic_ascendance{};
		const discount_t calamitous_retaliation{};

		template<grimoire_e Discount> constexpr discount_t get_discount() const noexcept {
			switch (Discount) {
				case grimoire_e::ChaoticWarp: {
					return chaotic_warp;
				} case grimoire_e::PreciseWarp: {
					return precise_warp;
				} case grimoire_e::Annihilate: {
					return annihilate;
				} case grimoire_e::Repulse: {
					return repulse;
				} case grimoire_e::Calcify: {
					return calcify;
				} case grimoire_e::Incorporealize: {
					return incorporealize;
				} case grimoire_e::CalciticInvocation: {
					return calcitic_invocation;
				} case grimoire_e::SpectralInvocation: {
					return spectral_invocation;
				} case grimoire_e::SanguineInvocation: {
					return sanguine_invocation;
				} case grimoire_e::GalvanicInvocation: {
					return galvanic_invocation;
				} case grimoire_e::RavenousInvocation: {
					return ravenous_invocation;
				} case grimoire_e::WretchedInvocation: {
					return wretched_invocation;
				} case grimoire_e::CerebralInvocation: {
					return cerebral_invocation;
				} case grimoire_e::InfernalInvocation: {
					return infernal_invocation;
				} case grimoire_e::NecromanticAscendance: {
					return necromantic_ascendance;
				} case grimoire_e::CalamitousRetaliation: {
					return calamitous_retaliation;
				}
			}
		}
	};

	template<patron_e Patron> static inline patron_t patrons;

	template<> inline patron_t patrons<patron_e::None>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 1, 2 },
		.precise_warp = discount_t{ 0, 2, 4 },

		.annihilate = discount_t{ 0, 0, 0 },
		.repulse = discount_t{ 0, 0, 0 },
		.calcify = discount_t{ 0, 0, 0 },
		.incorporealize = discount_t{ 0, 0, 0 },

		.calcitic_invocation = discount_t{ 0, 0, 0 },
		.spectral_invocation = discount_t{ 0, 0, 0 },
		.sanguine_invocation = discount_t{ 0, 0, 0 },
		.galvanic_invocation = discount_t{ 0, 0, 0 },
		.ravenous_invocation = discount_t{ 0, 0, 0 },
		.wretched_invocation = discount_t{ 0, 0, 0 },
		.cerebral_invocation = discount_t{ 0, 0, 0 },
		.infernal_invocation = discount_t{ 0, 0, 0 },

		.necromantic_ascendance = discount_t{ 0, 0, 0 },
		.calamitous_retaliation = discount_t{ 0, 0, 0 },
	};

	template<> inline patron_t patrons<patron_e::Kalypdrot>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = strong_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = mild_invocation,
		.galvanic_invocation = mild_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = strong_invocation,
		.cerebral_invocation = weak_invocation,
		.infernal_invocation = weak_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Akurakhaithan>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ 0, 4, 8 },
		.repulse = discount_t{ -4, -2, 0 },
		.calcify = discount_t{ -4, -2, 0 },
		.incorporealize = discount_t{ 0, 2, 4 },

		.calcitic_invocation = mild_invocation,
		.spectral_invocation = strong_invocation,
		.sanguine_invocation = weak_invocation,
		.galvanic_invocation = mild_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = mild_invocation,
		.infernal_invocation = mild_invocation,

		.necromantic_ascendance = discount_t{ -4, 0, 8 },
		.calamitous_retaliation = mild_invocation,
	};

	template<> inline patron_t patrons<patron_e::Merirfin>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ 0, 2, 4 },
		.repulse = discount_t{ 0, 2, 4 },
		.calcify = discount_t{ -4, -2, 0 },
		.incorporealize = discount_t{ -8, -4, 0 },

		.calcitic_invocation = weak_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = strong_invocation,
		.galvanic_invocation = mild_invocation,
		.ravenous_invocation = mild_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = weak_invocation,
		.infernal_invocation = mild_invocation,

		.necromantic_ascendance = discount_t{ -6, 0, 6 },
		.calamitous_retaliation = discount_t{ 0, 8, 16 },
	};

	template<> inline patron_t patrons<patron_e::Rathghul>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = weak_invocation,
		.spectral_invocation = strong_invocation,
		.sanguine_invocation = weak_invocation,
		.galvanic_invocation = weak_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = strong_invocation,
		.infernal_invocation = mild_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Praethornyn>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = weak_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = strong_invocation,
		.galvanic_invocation = strong_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = weak_invocation,
		.infernal_invocation = weak_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Tsendikor>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = weak_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = mild_invocation,
		.galvanic_invocation = weak_invocation,
		.ravenous_invocation = strong_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = weak_invocation,
		.infernal_invocation = strong_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Ionna>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = mild_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = weak_invocation,
		.galvanic_invocation = weak_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = strong_invocation,
		.cerebral_invocation = weak_invocation,
		.infernal_invocation = weak_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Moslager>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ 0, 0, 1 },
		.precise_warp = discount_t{ 0, 0, 2 },

		.annihilate = discount_t{ -4, -2, 0 },
		.repulse = discount_t{ 0, 4, 8 },
		.calcify = discount_t{ 0, 2, 4 },
		.incorporealize = discount_t{ -4, -2, 0 },

		.calcitic_invocation = weak_invocation,
		.spectral_invocation = weak_invocation,
		.sanguine_invocation = weak_invocation,
		.galvanic_invocation = weak_invocation,
		.ravenous_invocation = weak_invocation,
		.wretched_invocation = weak_invocation,
		.cerebral_invocation = strong_invocation,
		.infernal_invocation = strong_invocation,

		.necromantic_ascendance = discount_t{ -8, 0, 4 },
		.calamitous_retaliation = discount_t{ 4, 8, 12 },
	};

	template<> inline patron_t patrons<patron_e::Sudoclor>{
		.disposition = disposition_e::Apathetic,

		.chaotic_warp = discount_t{ -1, 1, 2 },
		.precise_warp = discount_t{ -2, 2, 4 },

		.annihilate = discount_t{ -8, 0, 4 },
		.repulse = discount_t{ -8, 0, 4 },
		.calcify = discount_t{ -4, 0, 2 },
		.incorporealize = discount_t{ -4, 0, 2 },

		.calcitic_invocation = discount_t{ -8, 0, 4 },
		.spectral_invocation = discount_t{ -8, 0, 4 },
		.sanguine_invocation = discount_t{ -8, 0, 4 },
		.galvanic_invocation = discount_t{ -12, 0, 6 },
		.ravenous_invocation = discount_t{ -12, 0, 6 },
		.wretched_invocation = discount_t{ -12, 0, 6 },
		.cerebral_invocation = discount_t{ -12, 4, 6 },
		.infernal_invocation = discount_t{ -12, 4, 6 },

		.necromantic_ascendance = discount_t{ -4, 4, 16 },
		.calamitous_retaliation = discount_t{ -8, 0, 12 },
	};

	constexpr discount_type_e get_discount_type(i8 value) {
		return value == 0 ? discount_type_e::Placebo : value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
	}

	template<patron_e Patron> constexpr runes_t to_colored_string(grimoire_e grimoire) noexcept {
		runes_t colored_string{};

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr grimoire_e discount_cval{ val };
			
			cauto discount_info{ patrons<Patron>.dependent get_discount<discount_cval>() };

			colored_string.concatenate(to_colored_string(discount_info.negative, get_discount_type(discount_info.negative)));

			colored_string.concatenate(runes_t{ " | " });

			colored_string.concatenate(to_colored_string(discount_info.neutral, get_discount_type(discount_info.neutral)));

			colored_string.concatenate(runes_t{ " | " });

			colored_string.concatenate(to_colored_string(discount_info.positive, get_discount_type(discount_info.positive)));
		}, grimoire);

		return colored_string;
	}

	static inline disposition_e get_patron_disposition(patron_e patron) noexcept {
		return magic_enum::enum_switch([&](auto val) -> disposition_e {
			constexpr patron_e cval{ val };

			return patrons<cval>.disposition;
		}, patron);
	}

	static inline void impress(patron_e patron) noexcept {
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr patron_e cval{ val };
			
			switch (patrons<cval>.disposition) {
				case disposition_e::Sadistic: {
					patrons<cval>.disposition = disposition_e::Apathetic;
					break;
				} case disposition_e::Apathetic: {
					patrons<cval>.disposition = disposition_e::Cooperative;
					break;
				} default: {
					break;
				}
			}
		}, patron);
	}

	static inline void annoy(patron_e patron) noexcept {
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr patron_e cval{ val };
			
			switch (patrons<cval>.disposition) {
				case disposition_e::Apathetic: {
					patrons<cval>.disposition = disposition_e::Sadistic;
					break;
				} case disposition_e::Cooperative: {
					patrons<cval>.disposition = disposition_e::Apathetic;
					break;
				} default: {
					break;
				}
			}
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
