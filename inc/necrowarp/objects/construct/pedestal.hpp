#pragma once

#include <necrowarp/objects/object.hpp>

#include <optional>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_blocking<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<pedestal_t> {
		static constexpr object_e value = object_e::Pedestal;
	};

	template<> struct is_object_type<pedestal_t, object_e::Pedestal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Pedestal> {
		using type = pedestal_t;
	};

	template<> struct to_object_group<object_e::Pedestal> {
		static constexpr object_group_e value = object_group_e::Pedestal;
	};

	enum struct grimoire_e : u8 {
		ChaoticWarp,
		PreciseWarp,
		
		Annihilate,
		Repulse,
		Calcify,
		Incorporealize,

		CalciticInvocation,
		SpectralInvocation,
		SanguineInvocation,
		GalvanicInvocation,
		RavenousInvocation,
		WretchedInvocation,
		CerebralInvocation,
		InfernalInvocation,

		NecromanticAscendance,
		CalamitousRetaliation
	};

	constexpr cstr to_string(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::ChaoticWarp: {
				return "chaotic warp";
			} case grimoire_e::PreciseWarp: {
				return "precise warp";
			} case grimoire_e::Annihilate: {
				return "annihilate";
			} case grimoire_e::Repulse: {
				return "repulse";
			} case grimoire_e::Calcify: {
				return "calcify";
			} case grimoire_e::Incorporealize: {
				return "incorporealize";
			} case grimoire_e::CalciticInvocation: {
				return "calcitic invocation";
			} case grimoire_e::SpectralInvocation: {
				return "spectral invocation";
			} case grimoire_e::SanguineInvocation: {
				return "sanguine invocation";
			} case grimoire_e::GalvanicInvocation: {
				return "galvanic invocation";
			} case grimoire_e::RavenousInvocation: {
				return "ravenous invocation";
			} case grimoire_e::WretchedInvocation: {
				return "wretched invocation";
			} case grimoire_e::CerebralInvocation: {
				return "cerebral invocation";
			} case grimoire_e::InfernalInvocation: {
				return "infernal invocation";
			} case grimoire_e::NecromanticAscendance: {
				return "necromantic ascendance";
			} case grimoire_e::CalamitousRetaliation: {
				return "calamitous retaliation";
			}
		}
	}

	constexpr color_t to_color(grimoire_e grimoire) noexcept {
		switch (grimoire) {
			case grimoire_e::ChaoticWarp: {
				return colors::dark::Blue;
			} case grimoire_e::PreciseWarp: {
				return colors::Yellow;
			} case grimoire_e::Annihilate: {
				return colors::dark::Green;
			} case grimoire_e::Repulse: {
				return colors::light::Blue;
			} case grimoire_e::Calcify: {
				return colors::Marble;
			} case grimoire_e::Incorporealize: {
				return colors::light::Grey;
			} case grimoire_e::CalciticInvocation: {
				return colors::dark::Marble;
			} case grimoire_e::SpectralInvocation: {
				return colors::materials::Fluids<fluid_e::Ichor>;
			} case grimoire_e::SanguineInvocation: {
				return colors::materials::Fluids<fluid_e::Blood>;
			} case grimoire_e::GalvanicInvocation: {
				return colors::metals::Steel;
			} case grimoire_e::RavenousInvocation: {
				return mix(colors::Red, colors::Orange);
			} case grimoire_e::WretchedInvocation: {
				return colors::materials::Fluids<fluid_e::Filth>;
			} case grimoire_e::CerebralInvocation: {
				return mix(colors::Red, colors::light::Magenta);
			} case grimoire_e::InfernalInvocation: {
				return colors::materials::Fluids<fluid_e::Ectoplasm>;
			} case grimoire_e::NecromanticAscendance: {
				return colors::metals::Gold;
			} case grimoire_e::CalamitousRetaliation: {
				return colors::light::Red;
			}
		}
	}

	static constexpr runes_t to_colored_string(grimoire_e grimoire) noexcept { return runes_t{ to_string(grimoire), to_color(grimoire) }; }

	struct pedestal_t {
		const grimoire_e grimoire;

	  private:
		static inline std::uniform_int_distribution<u16> grimoire_dis{ static_cast<u16>(grimoire_e::ChaoticWarp), static_cast<u16>(grimoire_e::CalamitousRetaliation) };

		template<RandomEngine Generator> static inline grimoire_e random_grimoire(ref<Generator> engine) noexcept { return grimoire_dis(engine); }

		bool looted;

	  public:
		inline pedestal_t(grimoire_e grimoire) noexcept : grimoire{ grimoire }, looted{ false } {}

		template<RandomEngine Generator> inline pedestal_t(ref<Generator> engine) noexcept : grimoire{ random_grimoire(engine) }, looted{ false } {}

		inline bool is_looted() const noexcept { return looted; }

		inline std::optional<grimoire_e> loot() noexcept {
			if (looted) {
				return std::nullopt;
			}

			looted = true;

			return grimoire;
		}

		inline std::string to_string() const noexcept {
			return std::format("{} ({})", necrowarp::to_string(object_e::Pedestal), looted ? "empty" : necrowarp::to_string(grimoire));
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Pedestal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(looted ? runes_t{ "empty", colors::dark::Grey } : necrowarp::to_colored_string(grimoire))
				.concatenate(runes_t{ ")" });

			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Pedestal, static_cast<u8>(grimoire) }; }

		inline void draw(offset_t position) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position);
			}
		}

		inline void draw(offset_t position, offset_t offset) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position + offset);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position + offset);
			}
		}

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position + offset, nudge);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge);
			}
		}

		constexpr operator object_e() const noexcept { return object_e::Pedestal; }
	};
} // namespace necrowarp
