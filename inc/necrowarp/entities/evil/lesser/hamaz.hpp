#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<hamaz_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<hamaz_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<hamaz_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<hamaz_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<hamaz_t> {
		static constexpr entity_e value = entity_e::Hamaz;
	};

	template<> struct is_entity_type<hamaz_t, entity_e::Hamaz> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Hamaz> {
		using type = hamaz_t;
	};

	template<> struct is_evil<hamaz_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<hamaz_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<hamaz_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ectoplasm;

		static constexpr bool conditional = false;
	};

	struct hamaz_t {
		keyframe_t idle_animation;

		const entity_e entity;
		const bool enhanced;

		template<entity_e Entity> static constexpr i8 MaximumHealth{ 0 };

		template<> inline constexpr i8 MaximumHealth<entity_e::Adventurer>{ 1 };
		template<> inline constexpr i8 MaximumHealth<entity_e::Mercenary>{ 2 };
		template<> inline constexpr i8 MaximumHealth<entity_e::Ranger>{ 1 };
		template<> inline constexpr i8 MaximumHealth<entity_e::Skulker>{ 2 };
		template<> inline constexpr i8 MaximumHealth<entity_e::MistLady>{ 1 };
		template<> inline constexpr i8 MaximumHealth<entity_e::BannerBearer>{ 2 };
		template<> inline constexpr i8 MaximumHealth<entity_e::BattleMonk>{ 3 };
		template<> inline constexpr i8 MaximumHealth<entity_e::Berserker>{ 3 };
		template<> inline constexpr i8 MaximumHealth<entity_e::Paladin>{ 5 };

		template<entity_e Entity> static constexpr i8 MaximumDamage{ 0 };

		template<> inline constexpr i8 MaximumDamage<entity_e::Adventurer>{ 1 };
		template<> inline constexpr i8 MaximumDamage<entity_e::Mercenary>{ 2 };
		template<> inline constexpr i8 MaximumDamage<entity_e::Ranger>{ 1 };
		template<> inline constexpr i8 MaximumDamage<entity_e::Skulker>{ 2 };
		template<> inline constexpr i8 MaximumDamage<entity_e::MistLady>{ 1 };
		template<> inline constexpr i8 MaximumDamage<entity_e::BannerBearer>{ 2 };
		template<> inline constexpr i8 MaximumDamage<entity_e::BattleMonk>{ 2 };
		template<> inline constexpr i8 MaximumDamage<entity_e::Berserker>{ 2 };
		template<> inline constexpr i8 MaximumDamage<entity_e::Paladin>{ 3 };

		static constexpr f16 EnhancedHealthMultiplier{ 2.0f };
		static constexpr f16 EnhancedDamageMultiplier{ 1.5f };

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

	private:
		static inline std::uniform_int_distribution<usize> entity_dis{ static_cast<usize>(entity_e::Adventurer), static_cast<usize>(entity_e::Paladin) };

		template<RandomEngine Generator> static inline entity_e determine_entity(ref<Generator> engine) noexcept { return static_cast<entity_e>(entity_dis(engine)); }

		static inline std::bernoulli_distribution reincarnation_dis{ 0.01 };

		const i8 investiture;
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		static constexpr i8 determine_health(entity_e entity, bool enhanced) noexcept {
			return magic_enum::enum_switch([&](auto val) -> i8 {
				constexpr entity_e cval{ val };

				if (enhanced) {
					return static_cast<i8>(MaximumHealth<cval> * EnhancedHealthMultiplier);
				}

				return MaximumHealth<cval>;
			}, entity);
		}

		static constexpr i8 determine_damage(entity_e entity, bool enhanced) noexcept {
			return magic_enum::enum_switch([&](auto val) -> i8 {
				constexpr entity_e cval{ val };

				if (enhanced) {
					return static_cast<i8>(MaximumDamage<cval> * EnhancedDamageMultiplier);
				}

				return MaximumDamage<cval>;
			}, entity);
		}

		static constexpr u8 determine_index(entity_e entity) noexcept {
			switch (entity) {
				case entity_e::Adventurer: {
					return indices::AdventurerHamaz;
				} case entity_e::Mercenary: {
					return indices::MercenaryHamaz;
				} case entity_e::Ranger: {
					return indices::RangerHamaz;
				} case entity_e::Skulker: {
					return indices::SkulkerHamaz;
				} case entity_e::MistLady: {
					return indices::MistLadyHamaz;
				} case entity_e::BannerBearer: {
					return indices::BannerBearerHamaz;
				} case entity_e::BattleMonk: {
					return indices::BattleMonkHamaz;
				} case entity_e::Berserker: {
					return indices::BerserkerHamaz;
				} case entity_e::Paladin: {
					return indices::PaladinHamaz;
				} default: {
					return 0;
				}
			}
		};

	public:
		inline hamaz_t(entity_e entity, bool enhanced = false) noexcept :
			idle_animation{ determine_index(entity), random_engine, true },
			entity{ entity },
			enhanced{ enhanced },
			investiture{ determine_health(entity, enhanced) }, 
			health{ investiture }
		{}

		template<RandomEngine Generator> inline hamaz_t(ref<Generator> engine) noexcept :
			idle_animation{ random_engine, true },
			entity{ determine_entity(engine) },
			enhanced{ true },
			investiture{ determine_health(entity, enhanced) }, 
			health{ investiture }
		{ idle_animation.index = determine_index(entity); }
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return investiture; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			set_health(health - damage_amount);

			return true;
		}

		inline i8 get_damage() const noexcept { return determine_damage(entity, false); }

		inline i8 get_damage(entity_e target) const noexcept { return determine_damage(target, target == entity); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		template<map_type_e MapType> inline bool reincarnate(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({}{}) [{}/{}]", necrowarp::to_string(entity_e::Hamaz), necrowarp::to_string(entity), enhanced ? " | enhanced" : "", get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Hamaz) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(entity))
				.concatenate(runes_t{ std::format("{}) [{}/{}]", enhanced ? " | enhanced" : "", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Hamaz; }
	};
} // namespace necrowarp
