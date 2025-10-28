#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/patronage.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_animation<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<mansling_t> {
		static constexpr entity_e value = entity_e::Mansling;
	};

	template<> struct is_entity_type<mansling_t, entity_e::Mansling> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Mansling> {
		using type = mansling_t;
	};

	template<> struct is_good<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<mansling_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<mansling_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::BloodyEctoplasm;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<mansling_t> {
		static constexpr bool value = true;
	};

	struct mansling_t {
		keyframe_t idle_animation;

		template<disposition_e Disposition> static constexpr i8 EffectRadius{};

		template<> inline constexpr i8 EffectRadius<disposition_e::Sadistic>{ 24 };
		template<> inline constexpr i8 EffectRadius<disposition_e::Apathetic>{ 12 };
		template<> inline constexpr i8 EffectRadius<disposition_e::Cooperative>{ 6 };

		static constexpr i8 get_effect_radius() noexcept;

		static constexpr i8 MaximumHealth{ 1 };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };

		inline mansling_t() noexcept : idle_animation{ indices::Mansling, random_engine, true } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<map_type_e MapType> static constexpr bool in_range() noexcept;

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Mansling; }
	};

	static_assert(sizeof(mansling_t) <= NPCSizeCap, "mansling entity size must not exceed npc size cap!");
} // namespace necrowarp
