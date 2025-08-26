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

	template<dimension_e Dimension> struct is_abyssal{
		static constexpr bool value = false;
	};

	template<dimension_e Dimension> constexpr bool is_abyssal_v = is_abyssal<Dimension>::value;

	template<dimension_e Dimension> concept AbyssalDimension = is_abyssal<Dimension>::value;

	template<> struct is_abyssal<dimension_e::Abyss> {
		static constexpr bool value = true;
	};

	template<dimension_e Dimension> struct is_material{
		static constexpr bool value = true;
	};

	template<dimension_e Dimension> constexpr bool is_material_v = is_material<Dimension>::value;

	template<dimension_e Dimension> concept MaterialDimension = is_material<Dimension>::value;

	template<> struct is_material<dimension_e::Abyss> {
		static constexpr bool value = false;
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
				return "The Great Chaos is an immense region of desolated space around the galactic core destroyed thousands of millennia ago by the Andavanga";
			} case dimension_e::Underworld: {
				return "The Underworld is a massive expanse of caverns and natural tunnels that trickle throughout the subsurface of Mokrae";
			} case dimension_e::Voitykras: {
				return "Voitykras are the remnants of the First Plane and sovereign territory of the extramortal revenant, the Moslager";
			} case dimension_e::Sudokras: {
				return "Sudokras is the divine realm of Akurakhaithan and his banished subjects and their subrealms";
			} case dimension_e::Tribulation: {
				return "Tribulation! You are being tested by your patron; do not fail them now...";
			} case dimension_e::Yyndikras: {
				return "Yyndikras is the divine realm of the eponymous Yyantranu, one of the Trydaiastokhai and ascendant vanguard of the Third Plane";
			} case dimension_e::Overworld: {
				return "The Overworld is the surface of Mokrae, the chiefest of planets in Nurrheim with its three continents and rich, if not violent history";
			} case dimension_e::AncientVault: {
				return "The Ancient Vaults were constructed a millennium ago to store the embalmed corpses of archnecromancers and their fermenting, necrotic fluids";
			}
		}
	}

	template<dimension_e Dimension>
		requires is_material<Dimension>::value
	static constexpr map_type_e determine_map() noexcept {
		switch (Dimension) {
			case dimension_e::Underworld:
			case dimension_e::Overworld:
			case dimension_e::AncientVault: {
				return map_type_e::Standard;
			} default: {
				return map_type_e::Pocket;
			}
		}
	}
}
