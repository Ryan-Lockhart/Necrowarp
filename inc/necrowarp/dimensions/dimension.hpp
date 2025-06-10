#pragma once

#include <necrowarp/stats.hpp>

namespace necrowarp {
	enum struct dimension_e : u8 {
		Abyss = 0, // null dimension
		Underworld, // caverns map
		Voitykras, // boon map
		Sudokras, // audience map
		Tribulation, // tribulation map
		Yyndikras, // tutorial map
		Overworld, // siege map
		AncientVault, // ruins map
	};

	constexpr cstr to_string(dimension_e dimension) {
		switch (dimension) {
			case dimension_e::Abyss: {
				return "The Abyss";
			} case dimension_e::Underworld: {
				return "The Underworld";
			} case dimension_e::Voitykras: {
				return "Voitykras";
			} case dimension_e::Sudokras: {
				return "Sudokras";
			} case dimension_e::Tribulation: {
				return "Tribulation";
			} case dimension_e::Yyndikras: {
				return "Yyndikras";
			} case dimension_e::Overworld: {
				return "The Overworld";
			} case dimension_e::AncientVault: {
				return "An Ancient Vault";
			}
		}
	}

	constexpr cstr to_description(dimension_e dimension) {
		switch (dimension) {
			case dimension_e::Abyss: {
				return "The Abyss, also known as the Great Chaos, is an immense region of desolated space around the galactic core destroyed thousands of millennia ago by the Andavanga";
			} case dimension_e::Underworld: {
				return "The Underworld is a massive expanse of caverns and natural tunnels that trickle throughout the subsurface of Mokrae";
			} case dimension_e::Voitykras: {
				return "Voitykras is the remnants of the First Plane and sovereign territory of the extramortal revenant; the Moslager";
			} case dimension_e::Sudokras: {
				return "Sudokras is the divine realm of Akurakhaithan and his banished subjects";
			} case dimension_e::Tribulation: {
				return "Tribulation! You are being tested by your patron; do not fail them now";
			} case dimension_e::Yyndikras: {
				return "Yyndikras is the divine realm of the eponymous Yyantranu, one of the Trydaiastokhai and ascendant vanguard of the Third Plane";
			} case dimension_e::Overworld: {
				return "The Overworld is the surface of Mokrae; the chiefest of planets in Nurrheim with three continents and a rich and excessively violent history";
			} case dimension_e::AncientVault: {
				return "An ancient vault storing the embalmed corpses of long dead necromancers and their necrotic fluids within ornately hewn chalices";
			}
		}
	}

	template<dimension_e Dimension> requires (Dimension != dimension_e::Abyss) static inline void plunge() noexcept;
}
