#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_object<crevice_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<crevice_t> {
		static constexpr object_e value = object_e::Crevice;
	};

	template<> struct is_object_type<crevice_t, object_e::Crevice> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Crevice> {
		using type = crevice_t;
	};

	template<> struct to_object_group<object_e::Crevice> {
		static constexpr object_group_e value = object_group_e::Crevice;
	};

	template<> inline glyph_t object_glyphs<crevice_t>{ glyphs::Crevice };

	struct crevice_t {
		inline crevice_t() noexcept {}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(object_glyphs<crevice_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(object_glyphs<crevice_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(object_glyphs<crevice_t>, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Crevice; }
	};
} // namespace necrowarp
