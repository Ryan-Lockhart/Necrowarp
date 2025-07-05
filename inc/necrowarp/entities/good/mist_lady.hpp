#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<mist_lady_t> {
		static constexpr entity_e value = entity_e::MistLady;
	};

	template<> struct is_entity_type<mist_lady_t, entity_e::MistLady> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::MistLady> {
		using type = mist_lady_t;
	};

	template<> struct to_entity_group<entity_e::MistLady> {
		static constexpr entity_group_e value = entity_group_e::MistLady;
	};

	template<> struct is_good<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<mist_lady_t> {
		static constexpr fluid_e type = fluid_e::BloodyEctoplasm;
	};

	template<> struct is_devourable<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<mist_lady_t>{ glyphs::MistLady };

	struct mist_lady_t {
		offset_t position;

		static constexpr i8 EffectRadius{ 8 };

		static constexpr i8 MaximumHealth{ 1 };

		static constexpr i8 DeathBoon{ 2 };

		inline mist_lady_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::MistLady; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<mist_lady_t> mist_lady) noexcept { return offset_t::std_hasher::operator()(mist_lady.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<mist_lady_t> lhs, cref<mist_lady_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<mist_lady_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<mist_lady_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};

	static_assert(sizeof(mist_lady_t) <= NPCSizeCap, "mist lady entity size must not exceed npc size cap!");
} // namespace necrowarp
