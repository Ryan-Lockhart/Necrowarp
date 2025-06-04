#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<paladin_t> {
		static constexpr entity_e value = entity_e::Paladin;
	};

	template<> struct is_entity_type<paladin_t, entity_e::Paladin> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Paladin> {
		using type = paladin_t;
	};

	template<> struct to_entity_group<entity_e::Paladin> {
		static constexpr entity_group_e value = entity_group_e::Paladin;
	};

	template<> struct is_good_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<paladin_t> {
		static constexpr fluid_type_e type = fluid_type_e::Blood;
	};

	template<> inline constexpr glyph_t entity_glyphs<paladin_t>{ glyphs::Paladin };

	struct paladin_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 4 };
		static constexpr i8 MaximumDamage{ 3 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::DeathKnight,
			entity_e::Wraith,
			entity_e::FleshGolem,
			entity_e::AnimatedSuit,
			entity_e::Cultist,
			entity_e::Skeleton,
			entity_e::Bloodhound,
			entity_e::Bonespur,
		};

		static constexpr i8 DeathBoon{ 3 };
		
	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline paladin_t(offset_t position) noexcept : position{ position }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Paladin), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Paladin) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<paladin_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<paladin_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Paladin; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<paladin_t> paladin) noexcept { return offset_t::std_hasher::operator()(paladin.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<paladin_t> lhs, cref<paladin_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<paladin_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<paladin_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
