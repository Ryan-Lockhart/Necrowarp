#pragma once

#include "bleak/constants/glyphs.hpp"
#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<portal_t> {
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

	template<> inline constexpr glyph_t object_glyphs<portal_t>{ glyphs::CalmPortal };

	enum struct stability_e : u8 {
		Calm,
		Vocal,
		Turbulent
	};

	constexpr cstr to_string(stability_e type) noexcept {
		switch (type) {
			case stability_e::Calm: {
				return "calm";
			} case stability_e::Vocal: {
				return "vocal";
			} case stability_e::Turbulent: {
				return "turbulent";
			} default: {
				return "unknown";
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
			} default: {
				return runes_t{ string, colors::metals::Iron };
			}
		}
	}

	static inline std::uniform_int_distribution<u16> stability_dis{ static_cast<u16>(stability_e::Calm), static_cast<u16>(stability_e::Turbulent) };

	template<RandomEngine Generator> static inline stability_e random_stability(ref<Generator> generator) noexcept { return static_cast<stability_e>(stability_dis(generator)); }

	struct portal_t {
		offset_t position;
		
		const stability_e stability;

		inline portal_t(offset_t position, bool random = false) noexcept :
			position{ position },
			stability{ random ? random_stability(random_engine) : stability_e::Calm }
		{}

		inline portal_t(offset_t position, stability_e stability) noexcept :
			position{ position },
			stability{ stability }
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

		inline glyph_t current_glyph() const noexcept {
			switch (stability) {
				case stability_e::Calm: {
					return glyphs::CalmPortal;
				} case stability_e::Vocal: {
					return glyphs::VocalPortal;
				} case stability_e::Turbulent: {
					return glyphs::TurbulentPortal;
				}
			}
		}

		inline void draw() const noexcept { entity_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Portal; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<portal_t> ladder) noexcept { return offset_t::std_hasher::operator()(ladder.position); }

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
