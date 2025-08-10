#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<furtive_horror_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<furtive_horror_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<furtive_horror_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<furtive_horror_t> {
		static constexpr entity_e value = entity_e::FurtiveHorror;
	};

	template<> struct is_entity_type<furtive_horror_t, entity_e::FurtiveHorror> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::FurtiveHorror> {
		using type = furtive_horror_t;
	};

	template<> struct is_evil<furtive_horror_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<furtive_horror_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<furtive_horror_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::IchorousEctoplasm;

		static constexpr bool conditional = false;
	};

	template<> struct is_concussable<furtive_horror_t> {
		static constexpr bool value = false;
	};

	template<> struct is_abominable<furtive_horror_t> {
		static constexpr bool value = true;
	};

	struct furtive_horror_t {
		keyframe_t idle_animation;

		static constexpr i8 MaximumDamage{ 4 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
			entity_e::Thetwo,
		};

		static constexpr f16 EffectRadius{ 4.0f };

	private:
		static inline std::bernoulli_distribution fumble_dis{ 0.333 };

		const i8 investiture;
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:		
		inline furtive_horror_t(i8 health) noexcept : idle_animation{ indices::FurtiveHorror, random_engine, true }, investiture{ health }, health{ investiture } {}

		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		inline i8 max_health() const noexcept { return investiture; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> generator) noexcept { return fumble_dis(generator); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			set_health(health - damage_amount);

			return true;
		}

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::FurtiveHorror), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::FurtiveHorror) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::FurtiveHorror; }
	};

	static_assert(sizeof(furtive_horror_t) <= NPCSizeCap, "furtive horror entity size must not exceed npc size cap!");
} // namespace necrowarp
