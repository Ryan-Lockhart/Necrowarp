#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<bones_t> {
		static constexpr object_e value = object_e::Bones;
	};

	template<> struct is_object_type<bones_t, object_e::Bones> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Bones> {
		using type = bones_t;
	};

	template<> struct to_object_group<object_e::Bones> {
		static constexpr object_group_e value = object_group_e::Bones;
	};

	struct bones_t {
		offset_t position;
		const decay_e state;

		inline bones_t(offset_t position) noexcept : position{ position }, state{ decay_e::Fresh } {}

		inline bones_t(offset_t position, decay_e state) noexcept : position{ position }, state{ state } {}

		inline bool is_fresh() const noexcept { return state == decay_e::Fresh; }

		inline bool is_animate() const noexcept { return state == decay_e::Animate; }

		inline bool is_rotted() const noexcept { return state == decay_e::Rotted; }

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string<plurality_e::Multiple>(object_e::Bones), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string<plurality_e::Multiple>(object_e::Bones) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case decay_e::Rotted: {
					return glyphs::RottedBones;
				} case decay_e::Animate: {
					return glyphs::AnimateBones;
				} case decay_e::Fresh: {
					return glyphs::FreshBones;
				}
			}
		}

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Bones; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<bones_t> bones) noexcept { return offset_t::std_hasher::operator()(bones.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<bones_t> lhs, cref<bones_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<bones_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<bones_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
