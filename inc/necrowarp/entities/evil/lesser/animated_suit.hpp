#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>
#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/galvanisation.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<animated_suit_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<animated_suit_t> {
		static constexpr entity_e value = entity_e::AnimatedSuit;
	};

	template<> struct is_entity_type<animated_suit_t, entity_e::AnimatedSuit> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::AnimatedSuit> {
		using type = animated_suit_t;
	};

	template<> struct to_entity_group<entity_e::AnimatedSuit> {
		static constexpr entity_group_e value = entity_group_e::AnimatedSuit;
	};

	template<> struct is_evil<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<animated_suit_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ichor;

		static constexpr bool conditional = false;
	};

	template<> struct is_concussable<animated_suit_t> {
		static constexpr bool value = false;
	};

	template<> struct is_armored<animated_suit_t> {
		static constexpr bool value = true;
	};

	struct animated_suit_t {
		keyframe_t idle_animation;

		const galvanisation_e state;

		static constexpr i8 MaximumHealth{ 3 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Paladin,
			entity_e::Berserker,
			entity_e::BattleMonk,
			entity_e::BannerBearer,
			entity_e::Thetwo,
			entity_e::Mercenary,
			entity_e::MistLady,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::Adventurer,
		};

		static constexpr galvanisation_e default_galvanisation{ galvanisation_e::Twisted };

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		static constexpr u8 get_index(galvanisation_e state) noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return indices::TwistedSuit;
				} case galvanisation_e::Shimmering: {
					return indices::ShimmeringSuit;
				} case galvanisation_e::Wriggling: {
					return indices::WrigglingSuit;
				} case galvanisation_e::Writhing: {
					return indices::WrithingSuit;
				}
			}
		}
	
	public:
		inline animated_suit_t() noexcept :
			idle_animation{ get_index(default_galvanisation), random_engine, true }, state{ default_galvanisation }, health{ MaximumHealth }
		{}

		inline animated_suit_t(galvanisation_e state) noexcept :
			idle_animation{ get_index(state), random_engine, true }, state{ state }, health{ MaximumHealth }
		{}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline i8 get_minimum_damage_received() const noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return 4;
				} case galvanisation_e::Shimmering: {
					return 3;
				} case galvanisation_e::Wriggling: {
					return 2;
				} case galvanisation_e::Writhing: {
					return 1;
				}
			}
		}

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		template<CombatantEntity Attacker> inline i8 get_minimum_damage_received() const noexcept {
			const i8 min_damage{ get_minimum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(min_damage * 1.5), 1);
			} else {
				return min_damage;
			}
		}

		template<CombatantEntity Attacker> inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received<Attacker>(), damage_amount); }

		template<CombatantEntity Attacker> inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage<Attacker>(damage_amount); }

		template<CombatantEntity Attacker> inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage<Attacker>(damage_amount)); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void killed(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({}) [{}/{}]", necrowarp::to_string(entity_e::AnimatedSuit), necrowarp::to_string(state), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::AnimatedSuit) };

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

		constexpr operator entity_e() const noexcept { return entity_e::AnimatedSuit; }
	};
} // namespace necrowarp
