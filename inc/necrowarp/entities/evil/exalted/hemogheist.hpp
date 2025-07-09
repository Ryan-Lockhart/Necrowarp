#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<hemogheist_t> {
		static constexpr entity_e value = entity_e::Hemogheist;
	};

	template<> struct is_entity_type<hemogheist_t, entity_e::Hemogheist> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Hemogheist> {
		using type = hemogheist_t;
	};

	template<> struct to_entity_group<entity_e::Hemogheist> {
		static constexpr entity_group_e value = entity_group_e::Hemogheist;
	};

	template<> struct is_evil<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<hemogheist_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_concussable<hemogheist_t> {
		static constexpr bool value = false;
	};

	template<> struct is_cleaver<hemogheist_t> {
		static constexpr bool value = true;
	};

	struct hemogheist_t {
		keyframe_t idle_animation;

		static constexpr i8 MaximumHealth{ 9 };
		static constexpr i8 MaximumDamage{ 5 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::Thetwo,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
		};

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:		
		inline hemogheist_t(i8 health) noexcept : idle_animation{ indices::Hemogheist, random_engine, true }, health{ health } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Hemogheist), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Hemogheist) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Hemogheist; }
	};

	static_assert(sizeof(hemogheist_t) <= NPCSizeCap, "hemogheist entity size must not exceed npc size cap!");
} // namespace necrowarp
