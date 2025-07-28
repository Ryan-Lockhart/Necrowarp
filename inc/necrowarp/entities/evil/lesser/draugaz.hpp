#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<draugaz_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<draugaz_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<draugaz_t> {
		static constexpr entity_e value = entity_e::Draugaz;
	};

	template<> struct is_entity_type<draugaz_t, entity_e::Draugaz> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Draugaz> {
		using type = draugaz_t;
	};

	template<> struct to_entity_group<entity_e::Draugaz> {
		static constexpr entity_group_e value = entity_group_e::Draugaz;
	};

	template<> struct is_evil<draugaz_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unholy<draugaz_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<draugaz_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<draugaz_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Filth;

		static constexpr bool conditional = false;
	};

	template<> inline constexpr glyph_t entity_glyphs<draugaz_t>{ glyphs::Draugaz };

	struct draugaz_t {
		static constexpr i8 MaximumHealth{ 3 };
		static constexpr i8 MaximumDamage{ 2 };

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
		
	private:
		i8 health;
		bool seared;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline draugaz_t() noexcept : health{ MaximumHealth }, seared{ false } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool is_seared() const noexcept { return seared; }

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

		inline void regenerate() noexcept {
			if (health >= max_health() || seared) {
				return;
			}

			set_health(health + 1);
		}

		inline void regenerate(i8 amount) noexcept {
			if (health >= max_health() || seared) {
				return;
			}

			set_health(health + amount);
		}

		inline void sear() noexcept { seared = true; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Draugaz), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Draugaz) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<draugaz_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<draugaz_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<draugaz_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Draugaz; }
	};
} // namespace necrowarp
