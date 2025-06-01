#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/objects/object.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<skeleton_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<skeleton_t> {
		static constexpr entity_e value = entity_e::Skeleton;
	};

	template<> struct is_entity_type<skeleton_t, entity_e::Skeleton> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Skeleton> {
		using type = skeleton_t;
	};

	template<> struct to_entity_group<entity_e::Skeleton> {
		static constexpr entity_group_e value = entity_group_e::Skeleton;
	};

	template<> struct is_evil_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_clumsy<skeleton_t> {
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

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Priest,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Paladin,
		};
		
		constexpr i8 armor_boon() const noexcept {
			return static_cast<i8>(state) + 1;
		}

		inline skeleton_t(offset_t position) noexcept : position{ position }, state{ decay_e::Fresh } {}

		inline skeleton_t(offset_t position, decay_e state) noexcept : position{ position }, state{ state } {}

		inline bool is_fresh() const noexcept { return state == decay_e::Fresh; }

		inline bool is_animate() const noexcept { return state == decay_e::Animate; }

		inline bool is_rotted() const noexcept { return state == decay_e::Rotted; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_e::Skeleton), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Skeleton) };

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

		inline void draw() const noexcept { entity_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Skeleton; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skeleton_t> skeleton) noexcept { return offset_t::std_hasher::operator()(skeleton.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<skeleton_t> lhs, cref<skeleton_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skeleton_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<skeleton_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
