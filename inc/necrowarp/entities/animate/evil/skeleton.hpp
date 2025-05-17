#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;
	
	template<> struct is_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<skeleton_t, entity_type_t::Skeleton> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Skeleton> {
		using type = skeleton_t;
	};

	template<> struct is_evil_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<skeleton_t> {
		static constexpr fluid_type_e type = fluid_type_e::Ichor;
	};

	template<> inline constexpr glyph_t entity_glyphs<skeleton_t>{ glyphs::FreshSkeleton };

	struct skeleton_t {
		offset_t position;
		const decay_e state;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };
		
		constexpr i8 armor_boon() const noexcept {
			return static_cast<i8>(state) + 1;
		}

		inline skeleton_t(offset_t position) noexcept : position{ position }, state{ decay_e::Fresh } {}

		inline skeleton_t(offset_t position, decay_e state) noexcept : position{ position }, state{ state } {}

		inline bool is_fresh() const noexcept { return state == decay_e::Fresh; }

		inline bool is_animate() const noexcept { return state == decay_e::Animate; }

		inline bool is_rotted() const noexcept { return state == decay_e::Rotted; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline entity_command_t think() const noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_type_t::Skeleton), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_type_t::Skeleton) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case decay_e::Rotted: {
					return glyphs::RottedSkeleton;
				} case decay_e::Animate: {
					return glyphs::AnimateSkeleton;
				} case decay_e::Fresh: {
					return glyphs::FreshSkeleton;
				}
			}
		}

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Skeleton; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skeleton_t> skeleton) noexcept { return offset_t::hasher::operator()(skeleton.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<skeleton_t> lhs, cref<skeleton_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skeleton_t> lhs, offset_t rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<skeleton_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
