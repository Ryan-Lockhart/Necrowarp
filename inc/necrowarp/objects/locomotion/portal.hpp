#pragma once

#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<portal_t> {
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
		Calm, // calm portals  take you to a pocket dimension with lesser ability improvements without tribulation
		Vocal, // vocal portals take you to an audience chamber with your patron
		Turbulent, // turbulent portals take you to a pocket dimension with unique ability unlocks after tribulation
		Insightful, // insightful portals take you to the tutorial dimension
		Collapsing, // collapsing portals return you to the underworld
		Yawning, // yawning portals take you to the overworld
		Echoing, // echoing portals take you to the ruins
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

	constexpr cstr to_string(stability_e type) noexcept {
		switch (type) {
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

	constexpr runes_t to_colored_string(stability_e type) noexcept {
		const cstr string{ to_string(type) };
		switch (type) {
			case stability_e::Calm: {
				return runes_t{ string, colors::light::Blue };
			} case stability_e::Vocal: {
				return runes_t{ string, colors::light::Orange };
			} case stability_e::Turbulent: {
				return runes_t{ string, colors::light::Red };
			} case stability_e::Insightful: {
				return runes_t{ string, colors::light::Green };
			} case stability_e::Collapsing: {
				return runes_t{ string, colors::light::Magenta };
			} case stability_e::Yawning: {
				return runes_t{ string, colors::light::Yellow };
			} case stability_e::Echoing: {
				return runes_t{ string, colors::light::Cyan };
			}
		}
	}

	static inline std::uniform_int_distribution<u16> stability_dis{ static_cast<u16>(stability_e::Calm), static_cast<u16>(stability_e::Yawning) };

	template<RandomEngine Generator> static inline stability_e random_stability(ref<Generator> generator) noexcept { return static_cast<stability_e>(stability_dis(generator)); }

	struct portal_t {
		offset_t position;
		
		const stability_e stability;

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
		keyframe_t idle_animation;

		inline portal_t(offset_t position, bool random = false) noexcept :
			position{ position },
			stability{ random ? random_stability(random_engine) : stability_e::Calm },
			idle_animation{ get_index(stability), random_engine, true }
		{}

		inline portal_t(offset_t position, stability_e stability) noexcept :
			position{ position },
			stability{ stability },
			idle_animation{ get_index(stability), random_engine, true }
		{}

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(object_e::Portal), necrowarp::to_string(stability)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Portal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(runes_t{ necrowarp::to_string(stability) })
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline void draw() const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Portal; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<portal_t> portal) noexcept { return offset_t::std_hasher::operator()(portal.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<portal_t> lhs, cref<portal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<portal_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<portal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
