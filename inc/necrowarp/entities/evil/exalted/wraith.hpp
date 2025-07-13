#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/eviscerate.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<wraith_t> {
		static constexpr entity_e value = entity_e::Wraith;
	};

	template<> struct is_entity_type<wraith_t, entity_e::Wraith> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Wraith> {
		using type = wraith_t;
	};

	template<> struct to_entity_group<entity_e::Wraith> {
		static constexpr entity_group_e value = entity_group_e::Wraith;
	};

	template<> struct is_evil<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<wraith_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ichor;

		static constexpr bool conditional = false;
	};

	template<> struct is_concussable<wraith_t> {
		static constexpr bool value = false;
	};

	template<> struct is_cleaver<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_vigilant<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<wraith_t, eviscerate_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<wraith_t>{ glyphs::Wraith };

	struct wraith_t {
		static constexpr i8 MaximumHealth{ 9 };
		static constexpr i8 MaximumDamage{ 5 };

		static constexpr std::array<entity_e, 8> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Thetwo,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
		};

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:		
		inline wraith_t(i8 health) noexcept : health{ health } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void killed(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Wraith), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Wraith) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Wraith; }
	};

	static_assert(sizeof(wraith_t) <= NPCSizeCap, "wraith entity size must not exceed npc size cap!");
} // namespace necrowarp
