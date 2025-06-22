#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/ternary/lunge.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;
	
	template<> struct is_entity<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<thetwo_t> {
		static constexpr entity_e value = entity_e::Thetwo;
	};

	template<> struct is_entity_type<thetwo_t, entity_e::Thetwo> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Thetwo> {
		using type = thetwo_t;
	};

	template<> struct to_entity_group<entity_e::Thetwo> {
		static constexpr entity_group_e value = entity_group_e::Thetwo;
	};

	template<> struct is_evil_entity<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<thetwo_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_fast<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<thetwo_t, lunge_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<thetwo_t>{ glyphs::Bloodhound };

	struct thetwo_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 16 };
		static constexpr i8 MaximumDamage{ 2 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::FleshGolem,
			entity_e::Bloodhound,
			entity_e::Cultist,
			entity_e::Skeleton,
			entity_e::Bonespur,
			entity_e::AnimatedSuit,
			entity_e::Wraith,
			entity_e::DeathKnight,
		};

		static constexpr i8 DeathBoon{ 2 };
		
	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline thetwo_t(offset_t position) noexcept : position{ position }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return target != entity_e::DeathKnight ? MaximumDamage : 0; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<thetwo_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<thetwo_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<thetwo_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<thetwo_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Thetwo; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<thetwo_t> thetwo) noexcept { return offset_t::std_hasher::operator()(thetwo.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<thetwo_t> lhs, cref<thetwo_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<thetwo_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<thetwo_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
