#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<flesh_golem_t> {
		static constexpr entity_e value = entity_e::FleshGolem;
	};

	template<> struct is_entity_type<flesh_golem_t, entity_e::FleshGolem> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::FleshGolem> {
		using type = flesh_golem_t;
	};

	template<> struct to_entity_group<entity_e::FleshGolem> {
		static constexpr entity_group_e value = entity_group_e::FleshGolem;
	};

	template<> struct is_evil<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<flesh_golem_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_devourable<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<flesh_golem_t>{ glyphs::FleshGolem };

	struct flesh_golem_t {
		static constexpr f32 HealthMultiplier{ 2.0f };

		static constexpr i8 BaseMaximumHealth{ 9 };

		static constexpr i8 MaximumHealth{ static_cast<i8>(BaseMaximumHealth * HealthMultiplier) };
		static constexpr i8 MaximumDamage{ 1 };

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
		inline flesh_golem_t(i8 health) noexcept : health{ static_cast<i8>(health * HealthMultiplier) } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::FleshGolem), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::FleshGolem) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::FleshGolem; }
	};
} // namespace necrowarp
