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

	template<> struct is_evil<bloodhound_t> {
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
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_fast<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_vigilant<bloodhound_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<bloodhound_t, lunge_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<bloodhound_t>{ glyphs::Bloodhound };

	struct bloodhound_t {
		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
			entity_e::Thetwo,
		};

		inline bloodhound_t() noexcept {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept {
			switch (target) {
				case entity_e::Paladin: {
					return 0;
				} default: {
					return MaximumDamage;
				}
			}
		}

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<bloodhound_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<bloodhound_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<bloodhound_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Bloodhound; }
	};
} // namespace necrowarp
