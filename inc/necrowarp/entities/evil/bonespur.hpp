#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<bonespur_t> {
		static constexpr entity_e value = entity_e::Bonespur;
	};

	template<> struct is_entity_type<bonespur_t, entity_e::Bonespur> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Bonespur> {
		using type = bonespur_t;
	};

	template<> struct to_entity_group<entity_e::Bonespur> {
		static constexpr entity_group_e value = entity_group_e::Bonespur;
	};

	template<> struct is_evil_entity<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<bonespur_t>{ glyphs::Bonespur };

	struct bonespur_t {
		offset_t position;

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:
		static constexpr i8 MaximumHealth{ 9 };
		static constexpr i8 MaximumDamage{ 0 };

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Paladin,
			entity_e::Mercenary,
			entity_e::Adventurer,
			entity_e::Priest,
		};

		static constexpr i8 MinimumDamageReceived{ 1 };
		
		inline bonespur_t(offset_t position, i8 health) noexcept : position{ position }, health{ health } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(MinimumDamageReceived, damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Bonespur), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Bonespur) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<bonespur_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<bonespur_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<bonespur_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<bonespur_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Bonespur; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<bonespur_t> flesh_golem) noexcept { return offset_t::std_hasher::operator()(flesh_golem.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<bonespur_t> lhs, cref<bonespur_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<bonespur_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<bonespur_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
