#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<chromalese_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<chromalese_t> {
		static constexpr entity_e value = entity_e::Chromalese;
	};

	template<> struct is_entity_type<chromalese_t, entity_e::Chromalese> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Chromalese> {
		using type = chromalese_t;
	};

	template<> struct is_evil<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct is_inevadable<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<chromalese_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ectoplasm;

		static constexpr bool conditional = false;
	};

	template<> struct is_incorporeal<chromalese_t>{
		static constexpr bool value = true;
		static constexpr bool conditional = false;
	};

	template<> struct is_vigilant<chromalese_t> {
		static constexpr bool value = true;
	};

	template<> struct is_concussable<chromalese_t> {
		static constexpr bool value = false;
	};

	struct chromalese_t {
		keyframe_t idle_animation;

		static constexpr i8 MaximumHealth{ 3 };
		static constexpr i8 MaximumDamage{ 2 };

		static constexpr std::array<entity_e, 14> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::Mansling,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::Medicus,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Hexeater,
			entity_e::Paladin,
			entity_e::Thetwo,
			entity_e::Fauna,
		};
		
	private:
		static inline std::bernoulli_distribution dodge_dis{ 0.80 };

		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline chromalese_t() noexcept : idle_animation{ indices::Chromalese, random_engine, true }, health{ MaximumHealth } {}

		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		static constexpr bool HasStaticDodge{ true };

		template<RandomEngine Generator> static inline bool dodge(ref<Generator> generator) noexcept { return dodge_dis(generator); }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			set_health(health - damage_amount);

			return true;
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Chromalese), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Chromalese) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Chromalese; }
	};
} // namespace necrowarp
