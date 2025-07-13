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
		inline flesh_t() noexcept {}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(object_glyphs<flesh_t>, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Flesh; }
	};
} // namespace necrowarp
