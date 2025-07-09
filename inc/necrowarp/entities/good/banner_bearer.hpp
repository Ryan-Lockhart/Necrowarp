#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<banner_bearer_t> {
		static constexpr entity_e value = entity_e::BannerBearer;
	};

	template<> struct is_entity_type<banner_bearer_t, entity_e::BannerBearer> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::BannerBearer> {
		using type = banner_bearer_t;
	};

	template<> struct to_entity_group<entity_e::BannerBearer> {
		static constexpr entity_group_e value = entity_group_e::BannerBearer;
	};

	template<> struct is_good<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<banner_bearer_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_devourable<banner_bearer_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<banner_bearer_t>{ glyphs::BannerBearer };

	struct banner_bearer_t {
		static constexpr i8 EffectRadius{ 8 };
		static constexpr f32 EffectDamageMultiplier{ 1.5f };

		static constexpr i8 MaximumHealth{ 4 };
		static constexpr i8 MaximumDamage{ 2 };

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
		inline banner_bearer_t() noexcept : health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::BannerBearer), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::BannerBearer) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<banner_bearer_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::BannerBearer; }
	};

	static_assert(sizeof(banner_bearer_t) <= NPCSizeCap, "mist lady entity size must not exceed npc size cap!");
} // namespace necrowarp
