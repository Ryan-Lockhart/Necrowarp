#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;
	
	template<> struct is_entity<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<cultist_t> {
		static constexpr entity_e value = entity_e::Cultist;
	};

	template<> struct is_entity_type<cultist_t, entity_e::Cultist> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Cultist> {
		using type = cultist_t;
	};

	template<> struct to_entity_group<entity_e::Cultist> {
		static constexpr entity_group_e value = entity_group_e::Cultist;
	};

	template<> struct is_evil_entity<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<cultist_t> {
		static constexpr fluid_e type = fluid_e::BloodyIchor;
	};

	template<> inline constexpr glyph_t entity_glyphs<cultist_t>{ glyphs::Cultist };

	struct cultist_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Priest,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Paladin,
		};

		inline cultist_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<cultist_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<cultist_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<cultist_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<cultist_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Cultist; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<cultist_t> cultist) noexcept { return offset_t::std_hasher::operator()(cultist.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<cultist_t> lhs, cref<cultist_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<cultist_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<cultist_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
