#pragma once

#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct has_animation<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<portal_t> {
		static constexpr object_e value = object_e::Portal;
	};

	template<> struct is_object_type<portal_t, object_e::Portal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Portal> {
		using type = portal_t;
	};

	template<> struct to_object_group<object_e::Portal> {
		static constexpr object_group_e value = object_group_e::Portal;
	};

	enum struct stability_e : u8 {
		Calm,		// calm portals take you to a pocket dimension with lesser ability improvements without tribulation
		Vocal,		// vocal portals take you to an audience chamber with your patron
		Turbulent,	// turbulent portals take you to a pocket dimension with unique ability unlocks after tribulation
		Insightful, // insightful portals take you to the tutorial dimension
		Collapsing, // collapsing portals return you to the underworld
		Yawning,	// yawning portals take you to the overworld
		Echoing,	// echoing portals take you to the ruins
	};

	static constexpr bool operator==(stability_e lhs, dimension_e rhs) noexcept {
		switch (lhs) {
			case stability_e::Calm: {
				return rhs == dimension_e::Voitykras;
			} case stability_e::Vocal: {
				return rhs == dimension_e::Sudokras;
			} case stability_e::Turbulent: {
				return rhs == dimension_e::Tribulation;
			} case stability_e::Insightful: {
				return rhs == dimension_e::Yyndikras;
			} case stability_e::Collapsing: {
				return rhs == dimension_e::Underworld;
			} case stability_e::Yawning: {
				return rhs == dimension_e::Overworld;
			}  case stability_e::Echoing: {
				return rhs == dimension_e::AncientVault;
			}
		}
	}

	constexpr cstr to_string(stability_e stability) noexcept {
		switch (stability) {
			case stability_e::Calm: {
				return "calm";
			} case stability_e::Vocal: {
				return "vocal";
			} case stability_e::Turbulent: {
				return "turbulent";
			} case stability_e::Insightful: {
				return "insightful";
			} case stability_e::Collapsing: {
				return "collpasing";
			} case stability_e::Yawning: {
				return "yawning";
			} case stability_e::Echoing: {
				return "echoing";
			}
		}
	}

	constexpr color_t to_color(stability_e stability) noexcept {
		switch (stability) {
			case stability_e::Calm: {
				return colors::light::Blue;
			} case stability_e::Vocal: {
				return colors::light::Orange;
			} case stability_e::Turbulent: {
				return colors::light::Red;
			} case stability_e::Insightful: {
				return colors::light::Green;
			} case stability_e::Collapsing: {
				return colors::light::Magenta;
			} case stability_e::Yawning: {
				return colors::light::Yellow;
			} case stability_e::Echoing: {
				return colors::light::Cyan;
			}
		}
	}

	constexpr runes_t to_colored_string(stability_e stability) noexcept { return runes_t{ to_string(stability), to_color(stability) }; }

	static inline std::uniform_int_distribution<u16> stability_dis{ static_cast<u16>(stability_e::Calm), static_cast<u16>(stability_e::Yawning) };

	template<RandomEngine Generator> static inline stability_e random_stability(ref<Generator> engine) noexcept { return static_cast<stability_e>(stability_dis(engine)); }

	struct portal_t {
		const stability_e stability;
		keyframe_t idle_animation;

	  private:
		static constexpr u8 get_index(stability_e stability) noexcept {
			switch (stability) {
				case stability_e::Calm: {
					return indices::CalmPortal;
				} case stability_e::Vocal: {
					return indices::VocalPortal;
				} case stability_e::Turbulent: {
					return indices::TurbulentPortal;
				} case stability_e::Insightful: {
					return indices::InsightfulPortal;
				} case stability_e::Collapsing: {
					return indices::CollapsingPortal;
				} case stability_e::Yawning: {
					return indices::YawningPortal;
				} case stability_e::Echoing: {
					return indices::EchoingPortal;
				}
			}
		}

	  public:
		inline portal_t(stability_e stability) noexcept :
			stability{ stability },
			idle_animation{ get_index(stability), random_engine, true }
		{}

		template<RandomEngine Generator> inline portal_t(ref<Generator> engine) noexcept :
			stability{ random_stability(engine) },
			idle_animation{ get_index(stability), random_engine, true }
		{}

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(object_e::Portal), necrowarp::to_string(stability)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Portal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(runes_t{ necrowarp::to_colored_string(stability) })
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Portal; }
	};
} // namespace necrowarp
