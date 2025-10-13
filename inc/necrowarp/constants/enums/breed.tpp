#pragma once

#include <necrowarp/constants/enums/breed.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr cstr to_string(breed_e breed) noexcept {
		switch (breed) {
			case breed_e::Scaly: {
				return "scaly";
			} case breed_e::Shaggy: {
				return "shaggy";
			} case breed_e::Lanky: {
				return "lanky";
			}
		}
	}

	constexpr color_t to_color(breed_e breed) noexcept {
		switch (breed) {
			case breed_e::Scaly: {
				return colors::dark::Green;
			} case breed_e::Shaggy: {
				return colors::dark::Orange;
			} case breed_e::Lanky: {
				return colors::dark::Grey;
			}
		}
	}

	constexpr cstr to_description(breed_e breed) noexcept {
		switch (breed) {
			case breed_e::Scaly: {
				return "Originating from the hot and humid south of Mok and Raetun, the scaly thetwo are well adapted for the briny marshes and sweltering jungles they call home. With a hardened suit of armor plating, these ferocious creatures have never been tamed like their northern cousins despite the attempts of many maimed and devoured Rictorn...";
			} case breed_e::Shaggy: {
				return "The thetwo endemic to the north of Raetun are adorned, much like their hardy cohabitants, in a thick fur cloak; shielding them from the harsh winters that rage downwards from the peaks of the Falkorring. These versatile creatures have been tamed for their strength and endurance, serving as able steeds and strong pack animals.";
			} case breed_e::Lanky: {
				return "Feared across any terrestrial environ are the leathery, lanky thetwo. Bred for war and brutality in the image of their Drakhullic masters, these formidable monsters have preternatural dexterity, a terrifying gallop, and a sadistic edge only matched by the Drakhul that ride them into battle. Woe be upon you to encounter a wild one...";
			}
		}
	}

	constexpr runes_t to_colored_string(breed_e breed) noexcept { return runes_t{ to_string(breed), to_color(breed) }; }
} // namespace necrowarp
