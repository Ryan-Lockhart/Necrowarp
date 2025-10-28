#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>
#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/galvanisation.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct has_animation<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<death_knight_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<death_knight_t> {
		static constexpr entity_e value = entity_e::DeathKnight;
	};

	template<> struct is_entity_type<death_knight_t, entity_e::DeathKnight> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::DeathKnight> {
		using type = death_knight_t;
	};

	template<> struct is_evil<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<death_knight_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ichor;

		static constexpr bool conditional = false;
	};

	template<> struct is_concussable<death_knight_t> {
		static constexpr bool value = false;
	};

	template<> struct is_armored<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_cleaver<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_vigilant<death_knight_t> {
		static constexpr bool value = true;
	};

	struct death_knight_t {
		keyframe_t idle_animation;

		const galvanisation_e state;

		static constexpr f32 HealthMultiplier{ 1.5f };

		static constexpr i8 MaximumDamage{ 5 };

		static constexpr std::array<entity_e, 14> EntityPriorities{
			entity_e::Paladin,
			entity_e::Hexeater,
			entity_e::Berserker,
			entity_e::BattleMonk,
			entity_e::BannerBearer,
			entity_e::Mercenary,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::Mansling,
			entity_e::Medicus,
			entity_e::MistLady,
			entity_e::Adventurer,
			entity_e::Thetwo,
			entity_e::Fauna,
		};

	private:
		const i8 investiture;
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		static constexpr u8 get_index(galvanisation_e state) noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return indices::TwistedKnight;
				} case galvanisation_e::Shimmering: {
					return indices::ShimmeringKnight;
				} case galvanisation_e::Wriggling: {
					return indices::WrigglingKnight;
				} case galvanisation_e::Writhing: {
					return indices::WrithingKnight;
				}
			}
		}
	
	public:
		inline death_knight_t(i8 health) noexcept :
			idle_animation{ get_index(galvanisation_e::Twisted), random_engine, true }, state{ galvanisation_e::Twisted }, investiture{ static_cast<i8>(health * HealthMultiplier) }, health{ investiture }
		{}

		inline death_knight_t(i8 health, galvanisation_e state) noexcept :
			idle_animation{ get_index(state), random_engine, true }, state{ state }, investiture{ static_cast<i8>(health * HealthMultiplier) }, health{ investiture }
		{}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		inline i8 max_health() const noexcept { return investiture; }

		inline i8 get_maximum_damage_received() const noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return 3;
				} case galvanisation_e::Shimmering: {
					return 2;
				} case galvanisation_e::Wriggling: {
					return 1;
				} case galvanisation_e::Writhing: {
					return 0;
				}
			}
		}

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

		template<CombatantEntity Attacker> inline i8 get_maximum_damage_received() const noexcept {
			const i8 max_damage{ get_maximum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(max_damage * 1.5), 1);
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

		inline std::string to_string() const noexcept { return std::format("{} ({}) [{}/{}]", necrowarp::to_string(entity_e::DeathKnight), necrowarp::to_string(state), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::DeathKnight) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" })
				.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::DeathKnight; }
	};
} // namespace necrowarp
