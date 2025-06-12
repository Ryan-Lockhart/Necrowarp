#pragma once

#include <bleak.hpp>

#include <random>

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
				return runes_t{ string, colors::Red};
			} case disposition_e::Apathetic: {
				return runes_t{ string, colors::Yellow};
			} case disposition_e::Cooperative: {
				return runes_t{ string, colors::Green};
			}
		}
	}

	enum struct discount_e : u8 {
		RandomWarp,
		TargetWarp,
	
	 // Repulse,    (planned; circular area of effect combat command)
	 // Annihilate, (planned; linear area of effect combat command)

		CalciticInvocation, // domain of bones; patrons are Kalypdrot, Rathghul, Ionna; minions are skeletons and bonespurs
		SpectralInvocation, // domain of ichor; patrons are Akurakhaithan, Rathghul, Saeiligarkeuss; minions are cultists and wraithes
		SanguineInvocation, // domain of blood; patrons are Viedskavn, Merirfin, Neolithia; minions are bloodhounds and hemogheists
		GalvanicInvocation, // domain of metal; patrons are Thuljanor, Praethornyn, Exar; minions are animated suits of armor and death knights
	 // RavenousInvocation, // domain of flesh; patrons are Tselgwedixxikog, Sketzuum; minions are rakes and flesh golems
	 // WretchedInvocation, // domain of filth; patrons are Ionna, Kalypdrot; minions are draugr and dreadwyrms

	 // MaliceIncarnate, (planned; discard body temporarily and enter wraith-world)
		NecromanticAscendance
	};

	constexpr usize padding_size(discount_e discount) noexcept {
		switch (discount) {
			case discount_e::RandomWarp: {
				return 12;
			} case discount_e::TargetWarp: {
				return 12;
			} case discount_e::CalciticInvocation: {
				return 4;
			} case discount_e::SpectralInvocation: {
				return 4;
			} case discount_e::SanguineInvocation: {
				return 4;
			} case discount_e::GalvanicInvocation: {
				return 4;
			} case discount_e::NecromanticAscendance: {
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
			} case discount_e::CalciticInvocation: {
				return "Calcitic Invocation";
			} case discount_e::SpectralInvocation: {
				return "Spectral Invocation";
			} case discount_e::SanguineInvocation: {
				return "Sanguine Invocation";
			} case discount_e::GalvanicInvocation: {
				return "Galvanic Invocation";
			} case discount_e::NecromanticAscendance: {
				return "Necromantic Ascendance";
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

		const discount_t calcitic_invocation{};
		const discount_t spectral_invocation{};
		const discount_t sanguine_invocation{};
		const discount_t galvanic_invocation{};

		const discount_t necromantic_ascendance{};

		template<discount_e Discount> constexpr discount_t get_discount() const noexcept {
			switch (Discount) {
				case discount_e::RandomWarp: {
					return random_warp;
				} case discount_e::TargetWarp: {
					return target_warp;
				} case discount_e::CalciticInvocation: {
					return calcitic_invocation;
				} case discount_e::SpectralInvocation: {
					return spectral_invocation;
				} case discount_e::SanguineInvocation: {
					return sanguine_invocation;
				} case discount_e::GalvanicInvocation: {
					return galvanic_invocation;
				} case discount_e::NecromanticAscendance: {
					return necromantic_ascendance;
				}
			}
		}
	};

	template<patron_e Patron> static inline patron_t patrons;

	template<> inline patron_t patrons<patron_e::None>{
		disposition_e::Apathetic,
		{ 0, 1, 2 },
		{ 0, 2, 4 }
	};

	template<> inline patron_t patrons<patron_e::Rathghul>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ 0, 4, 6 },
		{ -6, -2, 2 },
		{ -4, 0, 4 },
		{ -4, 0, 4 },
		{ -8, 0, 4 },
	};

	template<> inline patron_t patrons<patron_e::Akurakhaithan>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ -4, 0, 4 },
		{ 0, 4, 6 },
		{ -6, -2, 2 },
		{ -4, 0, 4 },
		{ -4, 0, 8 },
	};

	template<> inline patron_t patrons<patron_e::Merirfin>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ -4, 0, 4 },
		{ -6, -2, 2 },
		{ 0, 4, 6 },
		{ -4, 0, 4 },
		{ -6, 0, 6 },
	};

	template<> inline patron_t patrons<patron_e::Saeiligarkeuss>{
		disposition_e::Apathetic,
		{ -1, 1, 2 },
		{ -2, 2, 4 },
		{ -8, 0, 4 },
		{ -8, 0, 4 },
		{ -8, 0, 4 },
		{ -6, 0, 6 },
		{ 0, 8, 16 },
	};

	constexpr discount_type_e get_discount_type(i8 value) {
		return value == 0 ? discount_type_e::Placebo : value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
	}

	template<patron_e Patron> constexpr runes_t to_colored_string(discount_e discount) noexcept {
		runes_t colored_string{};

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr discount_e discount_cval{ val };
			
			cauto discount_info{ patrons<Patron>.template get_discount<discount_cval>() };

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

	static inline void randomize_patrons() noexcept {
		static std::mt19937 generator{ std::random_device{}() };
		static std::uniform_int_distribution<i16> distribution{
			-1,
			1,
		};

		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			const i16 random_number{ distribution(generator) };

			disposition_e disposition{ disposition_e::Apathetic };

			if (random_number < 0) {
				disposition = disposition_e::Sadistic;
			} else if (random_number > 0) {
				disposition = disposition_e::Cooperative;
			}

			patrons<cval>.disposition = disposition;
		});
	}
} //necrowarp
