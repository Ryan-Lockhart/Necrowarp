#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<mercenary_t> {
		static constexpr entity_e value = entity_e::Mercenary;
	};

	template<> struct is_entity_type<mercenary_t, entity_e::Mercenary> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Mercenary> {
		using type = mercenary_t;
	};

	template<> struct to_entity_group<entity_e::Mercenary> {
		static constexpr entity_group_e value = entity_group_e::Mercenary;
	};

	template<> struct is_good_entity<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<mercenary_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_armored<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<mercenary_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<mercenary_t>{ glyphs::Mercenary };

	struct mercenary_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 2 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 MaximumDamageReceived{ 1 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::AnimatedSuit,
			entity_e::Bloodhound,
			entity_e::Cultist,
			entity_e::Skeleton,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::FleshGolem,
			entity_e::DeathKnight,
		};

		static constexpr i8 DeathBoon{ 2 };
		
	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline mercenary_t(offset_t position) noexcept : position{ position }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		constexpr i8 get_minimum_damage_received() const noexcept { return MaximumDamageReceived; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		template<CombatantEntity Attacker> constexpr i8 get_minimum_damage_received() const noexcept {
			const i8 min_damage{ get_minimum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(min_damage * 2), 1);
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

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Mercenary), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Mercenary) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<mercenary_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<mercenary_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<mercenary_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<mercenary_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Mercenary; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<mercenary_t> mercenary) noexcept { return offset_t::std_hasher::operator()(mercenary.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<mercenary_t> lhs, cref<mercenary_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<mercenary_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<mercenary_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
