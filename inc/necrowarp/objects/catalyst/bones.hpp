#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/decay.tpp>

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
		const decay_e state;

		static constexpr decay_e DefaultDecay{ decay_e::Fresh };

		inline bones_t() noexcept : state{ DefaultDecay } {}

		inline bones_t(decay_e state) noexcept : state{ state } {}

		template<decay_e Decay> inline bool is() const noexcept { return state == Decay; }

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

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Bones; }
	};
} // namespace necrowarp
