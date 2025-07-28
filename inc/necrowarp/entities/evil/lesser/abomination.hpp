#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/mitose.hpp>

#include <necrowarp/commands/binary/devour.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<abomination_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<abomination_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<abomination_t> {
		static constexpr entity_e value = entity_e::Abomination;
	};

	template<> struct is_entity_type<abomination_t, entity_e::Abomination> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Abomination> {
		using type = abomination_t;
	};

	template<> struct to_entity_group<entity_e::Abomination> {
		static constexpr entity_group_e value = entity_group_e::Abomination;
	};

	template<> struct is_evil<abomination_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<abomination_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<abomination_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<abomination_t, devour_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<abomination_t, mitose_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<abomination_t>{ glyphs::Abomination };

	struct abomination_t {
		static constexpr i8 MaximumHealth{ 3 };
		static constexpr i8 MaximumProtein{ 3 };

		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
			entity_e::Thetwo,
		};

		static constexpr std::array<object_e, 1> ObjectPriorities{ object_e::Flesh };

		static constexpr i8 ProteinValue{ 1 };
		static constexpr f16 ProteinRatio{ 0.66f };
		
	private:
		i8 health;
		i8 protein;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		inline void set_protein(i8 value) noexcept { protein = clamp<i8>(value, 0, max_protein()); }

	public:
		inline abomination_t() noexcept : health{ MaximumHealth }, protein{ 0 } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }
		
		inline i8 get_protein() const noexcept { return protein; }

		inline bool has_protein() const noexcept { return protein > 0; }

		constexpr i8 max_protein() const noexcept { return MaximumProtein; }

		constexpr bool can_devour(entity_e entity) const noexcept { return false; }

		inline bool can_devour(object_e object) const noexcept { return protein < max_protein() && object == object_e::Flesh; }

		constexpr bool can_devour(bulk_e bulk) const noexcept { return bulk == bulk_e::Neonatal || bulk == bulk_e::Young; }

		template<map_type_e MapType> inline bool can_devour(offset_t position) const noexcept;

		inline void fatten() noexcept { set_protein(protein + 1); }

		inline void fatten(i8 amount) noexcept { set_protein(protein + amount); }

		inline bool can_mitose() const noexcept { return protein >= max_protein(); }

		template<map_type_e MapType> inline bool can_mitose(offset_t position) const noexcept;

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			set_health(health - damage_amount);

			return true;
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void mitose(offset_t position) noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format(" [{}/{}] ({}/{})", get_health(), max_health(), get_protein(), max_protein()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Abomination) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}] ({}/{})", get_health(), max_health(), get_protein(), max_protein()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<abomination_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<abomination_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<abomination_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Abomination; }
	};
} // namespace necrowarp
