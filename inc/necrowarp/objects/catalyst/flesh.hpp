#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_object<flesh_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<flesh_t> {
		static constexpr object_e value = object_e::Flesh;
	};

	template<> struct is_object_type<flesh_t, object_e::Flesh> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Flesh> {
		using type = flesh_t;
	};

	template<> struct to_object_group<object_e::Flesh> {
		static constexpr object_group_e value = object_group_e::Flesh;
	};

	template<> inline constexpr glyph_t object_glyphs<flesh_t>{ glyphs::Flesh };

	struct flesh_t {
		offset_t position;

		inline flesh_t(offset_t position) noexcept : position{ position } {}

		inline void draw() const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Flesh; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<flesh_t> flesh) noexcept { return offset_t::std_hasher::operator()(flesh.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<flesh_t> lhs, cref<flesh_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<flesh_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<flesh_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
