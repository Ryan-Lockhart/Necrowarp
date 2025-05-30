#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<metal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<metal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<metal_t> {
		static constexpr object_e value = object_e::Metal;
	};

	template<> struct is_object_type<metal_t, object_e::Metal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Metal> {
		using type = metal_t;
	};

	template<> struct to_object_group<object_e::Metal> {
		static constexpr object_group_e value = object_group_e::Metal;
	};

	template<> inline constexpr glyph_t object_glyphs<metal_t>{ glyphs::TwistedMetal };

	struct metal_t {
		offset_t position;
		const galvanisation_e state;

		inline metal_t(offset_t position) noexcept : position{ position }, state{ galvanisation_e::Twisted } {}

		inline metal_t(offset_t position, galvanisation_e state) noexcept : position{ position }, state{ state } {}

		inline bool is_twisted() const noexcept { return state == galvanisation_e::Twisted; }

		inline bool is_shimmering() const noexcept { return state == galvanisation_e::Shimmering; }

		inline bool is_wriggling() const noexcept { return state == galvanisation_e::Wriggling; }

		inline bool is_writhing() const noexcept { return state == galvanisation_e::Writhing; }

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(object_e::Metal), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Metal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return glyphs::TwistedMetal;
				} case galvanisation_e::Shimmering: {
					return glyphs::ShimmeringMetal;
				} case galvanisation_e::Wriggling: {
					return glyphs::WrigglingMetal;
				} case galvanisation_e::Writhing: {
					return glyphs::WrithingMetal;
				}
			}
		}

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Metal; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<metal_t> metal) noexcept { return offset_t::std_hasher::operator()(metal.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<metal_t> lhs, cref<metal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<metal_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<metal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
