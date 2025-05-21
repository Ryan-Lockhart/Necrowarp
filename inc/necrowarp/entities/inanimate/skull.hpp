#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<skull_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<skull_t> {
		static constexpr entity_e value = entity_e::Skull;
	};

	template<> struct is_entity_type<skull_t, entity_e::Skull> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Skull> {
		using type = skull_t;
	};

	template<> struct to_entity_group<entity_e::Skull> {
		static constexpr entity_group_e value = entity_group_e::Skull;
	};

	template<> struct is_inanimate<skull_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<skull_t>{ glyphs::FreshSkull };

	struct skull_t {
		offset_t position;
		const decay_e state;

		inline skull_t(offset_t position) noexcept : position{ position }, state{ decay_e::Fresh } {}

		inline skull_t(offset_t position, decay_e state) noexcept : position{ position }, state{ state } {}

		inline bool is_fresh() const noexcept { return state == decay_e::Fresh; }

		inline bool is_animate() const noexcept { return state == decay_e::Animate; }

		inline bool is_rotted() const noexcept { return state == decay_e::Rotted; }

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_e::Skull), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Skull) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case decay_e::Rotted: {
					return glyphs::RottedSkull;
				} case decay_e::Animate: {
					return glyphs::AnimateSkull;
				} case decay_e::Fresh: {
					return glyphs::FreshSkull;
				}
			}
		}

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Skull; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skull_t> skull) noexcept { return offset_t::hasher::operator()(skull.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<skull_t> lhs, cref<skull_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skull_t> lhs, offset_t rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<skull_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
