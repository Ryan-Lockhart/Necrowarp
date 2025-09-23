#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<banner_bearer_t> {
		static constexpr entity_e value = entity_e::BannerBearer;
	};

	template<> struct is_entity_type<banner_bearer_t, entity_e::BannerBearer> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::BannerBearer> {
		using type = banner_bearer_t;
	};

	template<> struct is_good<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<banner_bearer_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_armored<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_holy<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<banner_bearer_t>{ glyphs::BannerBearer };

	struct banner_bearer_t {
		static constexpr i8 EffectRadius{ 8 };
		static constexpr f32 EffectDamageMultiplier{ 1.5f };

		static constexpr i8 MaximumHealth{ 4 };
		static constexpr i8 MaximumDamage{ 2 };

		static constexpr i8 MaximumDamageReceived{ 1 };

		static constexpr std::array<entity_e, 18> EntityPriorities{
			entity_e::Player,
			entity_e::Skeleton,
			entity_e::Cultist,
			entity_e::Bloodhound,
			entity_e::AnimatedSuit,
			entity_e::Abomination,
			entity_e::Draugaz,
			entity_e::Hamaz,
			entity_e::Chromalese,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::Hemogheist,
			entity_e::DeathKnight,
			entity_e::FleshGolem,
			entity_e::Dreadwurm,
			entity_e::FurtiveHorror,
			entity_e::Isoscel,
			entity_e::Thetwo,
		};

		static constexpr f16 LeashRange{ 4 };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };
		
	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	  public:
		inline banner_bearer_t() noexcept : health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		constexpr i8 get_maximum_damage_received() const noexcept { return MaximumDamageReceived; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };
			
			if (actual_damage <= 0) {
				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		template<CombatantEntity Attacker> constexpr i8 get_maximum_damage_received() const noexcept {
			const i8 max_damage{ get_maximum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(max_damage * 2), 1);
			} else {
				return max_damage;
			}
		}

		template<CombatantEntity Attacker> inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received<Attacker>(), damage_amount); }

		template<CombatantEntity Attacker> inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage<Attacker>(damage_amount); }

		template<CombatantEntity Attacker> inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage<Attacker>(damage_amount) };
			
			if (actual_damage <= 0) {
				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::BannerBearer), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::BannerBearer) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::BannerBearer; }
	};

	static_assert(sizeof(banner_bearer_t) <= NPCSizeCap, "banner bearer entity size must not exceed npc size cap!");
} // namespace necrowarp
