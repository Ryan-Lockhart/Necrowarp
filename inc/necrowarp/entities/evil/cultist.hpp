#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<cultist_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<cultist_t> {
		static constexpr entity_e value = entity_e::Cultist;
	};

	template<> struct is_entity_type<cultist_t, entity_e::Cultist> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Cultist> {
		using type = cultist_t;
	};

	template<> struct to_entity_group<entity_e::Cultist> {
		static constexpr entity_group_e value = entity_group_e::Cultist;
	};

	template<> struct is_evil<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<cultist_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<cultist_t> {
		static constexpr fluid_e type = fluid_e::BloodyIchor;
	};

	template<> struct is_vigilant<cultist_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<cultist_t>{ glyphs::Cultist };

	struct cultist_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 2 };
		static constexpr i8 MaximumDamage{ 1 };

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
		inline cultist_t(offset_t position) noexcept : position{ position }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Cultist), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Cultist) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<cultist_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<cultist_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<cultist_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<cultist_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Cultist; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<cultist_t> cultist) noexcept { return offset_t::std_hasher::operator()(cultist.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<cultist_t> lhs, cref<cultist_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<cultist_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<cultist_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
