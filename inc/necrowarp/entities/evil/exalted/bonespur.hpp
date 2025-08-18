#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/fluid.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<bonespur_t> {
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

	template<> struct is_evil<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<bonespur_t> {
		static constexpr bool value = true;
	};

	template<> struct is_concussable<bonespur_t> {
		static constexpr bool value = false;
	};

	template<> struct is_spatterable<bonespur_t> {
		static constexpr bool value = true;
	};

	struct bonespur_t {
		static constexpr i8 MaximumDamage{ 0 };

		static constexpr i8 MinimumDamageReceived{ 1 };

		static constexpr std::array<entity_e, 13> EntityPriorities{
			entity_e::Paladin,
			entity_e::Hexeater,
			entity_e::Berserker,
			entity_e::BattleMonk,
			entity_e::BannerBearer,
			entity_e::Mercenary,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::Mansling,
			entity_e::Medicus,
			entity_e::MistLady,
			entity_e::Adventurer,
			entity_e::Thetwo,
		};

	private:
		const i8 investiture;
		i8 health;

		fluid_e spatter;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:
		inline bonespur_t(i8 health) noexcept : investiture{ health }, health{ investiture }, spatter{ fluid_e::None } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		inline i8 max_health() const noexcept { return investiture; }

		inline i8 armor_boon() const noexcept { return health; }

		inline fluid_e get_spatter() const noexcept { return spatter; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(MinimumDamageReceived, damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };
			
			if (actual_damage <= 0) {
				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		inline void enspatter(fluid_e with) noexcept {
			spatter += with;
		}

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}] ({})", necrowarp::to_string(entity_e::Bonespur), get_health(), max_health(), necrowarp::to_string(spatter)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Bonespur) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{}] (", get_health(), max_health()) })
				.concatenate(spatter != fluid_e::None ? necrowarp::to_colored_string(spatter) : runes_t{ "unstained", colors::light::Grey })
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Bonespur, static_cast<u8>(spatter) }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Bonespur; }
	};
} // namespace necrowarp
