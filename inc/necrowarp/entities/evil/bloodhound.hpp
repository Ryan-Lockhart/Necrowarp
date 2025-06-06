#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/ternary/lunge.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;
	
	template<> struct is_entity<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<bloodhound_t> {
		static constexpr entity_e value = entity_e::Bloodhound;
	};

	template<> struct is_entity_type<bloodhound_t, entity_e::Bloodhound> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Bloodhound> {
		using type = bloodhound_t;
	};

	template<> struct to_entity_group<entity_e::Bloodhound> {
		static constexpr entity_group_e value = entity_group_e::Bloodhound;
	};

	template<> struct is_evil_entity<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<bloodhound_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_fast<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<bloodhound_t, lunge_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<bloodhound_t>{ glyphs::Bloodhound };

	struct bloodhound_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Paladin,
			entity_e::Mercenary,
			entity_e::Adventurer,
			entity_e::Priest,
		};

		inline bloodhound_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return target != entity_e::Paladin ? MaximumDamage : 0; }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<bloodhound_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<bloodhound_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<bloodhound_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<bloodhound_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Bloodhound; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<bloodhound_t> bloodhound) noexcept { return offset_t::std_hasher::operator()(bloodhound.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<bloodhound_t> lhs, cref<bloodhound_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<bloodhound_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<bloodhound_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
