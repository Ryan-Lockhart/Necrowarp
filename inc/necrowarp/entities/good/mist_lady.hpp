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
		static constexpr fluid_e type = fluid_e::BloodyEctoplasm;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<mist_lady_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<mist_lady_t>{ glyphs::MistLady };

	struct mist_lady_t {
		static constexpr i8 EffectRadius{ 8 };

		static constexpr i8 MaximumHealth{ 1 };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };

		inline mist_lady_t() noexcept {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<mist_lady_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::MistLady; }
	};

	static_assert(sizeof(mist_lady_t) <= NPCSizeCap, "mist lady entity size must not exceed npc size cap!");
} // namespace necrowarp
