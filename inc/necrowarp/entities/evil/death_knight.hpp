#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>
#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<death_knight_t> {
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

	template<> struct to_entity_group<entity_e::DeathKnight> {
		static constexpr entity_group_e value = entity_group_e::DeathKnight;
	};

	template<> struct is_evil_entity<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<death_knight_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<death_knight_t> {
		static constexpr fluid_e type = fluid_e::Ichor;
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
		offset_t position;
		const galvanisation_e state;

	private:
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
		keyframe_t idle_animation;

		static constexpr f32 HealthMultiplier{ 1.5f };

		static constexpr i8 BaseMaximumHealth{ 9 };

		static constexpr i8 MaximumHealth{ static_cast<i8>(BaseMaximumHealth * HealthMultiplier) };

		static constexpr i8 MaximumDamage{ 3 };

		static constexpr std::array<entity_e, 8> EntityPriorities{
			entity_e::Paladin,
			entity_e::Berserker,
			entity_e::BattleMonk,
			entity_e::Mercenary,
			entity_e::Thetwo,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::Adventurer,
		};

		inline death_knight_t(offset_t position, i8 health) noexcept :
			position{ position },
			state{ galvanisation_e::Twisted },
			health{ static_cast<i8>(health * HealthMultiplier) },
			idle_animation{ get_index(state), random_engine, true }
		{}

		inline death_knight_t(offset_t position, i8 health, galvanisation_e state) noexcept :
			position{ position },
			state{ state },
			health{ static_cast<i8>(health * HealthMultiplier) },
			idle_animation{ get_index(state), random_engine, true }
		{}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline i8 get_minimum_damage_received() const noexcept {
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

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

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

		inline void draw() const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::DeathKnight; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<death_knight_t> death_knight) noexcept { return offset_t::std_hasher::operator()(death_knight.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<death_knight_t> lhs, cref<death_knight_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<death_knight_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<death_knight_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
