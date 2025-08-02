#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/binary/eradicate.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<isoscel_t> {
		static constexpr entity_e value = entity_e::Isoscel;
	};

	template<> struct is_entity_type<isoscel_t, entity_e::Isoscel> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Isoscel> {
		using type = isoscel_t;
	};

	template<> struct to_entity_group<entity_e::Isoscel> {
		static constexpr entity_group_e value = entity_group_e::Isoscel;
	};

	template<> struct is_evil<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct is_primeval<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct is_inevadable<isoscel_t> {
		static constexpr bool value = true;
	};

	template<> struct is_concussable<isoscel_t> {
		static constexpr bool value = false;
	};

	template<> struct is_entity_command_valid<isoscel_t, eradicate_t> {
		static constexpr bool value = true;
	};

	struct isoscel_t {
		keyframe_t idle_animation;

		static constexpr i8 MaximumDamage{ 99 };

		static constexpr std::array<entity_e, 10> EntityPriorities{
			entity_e::Paladin,
			entity_e::Berserker,
			entity_e::BattleMonk,
			entity_e::BannerBearer,
			entity_e::MistLady,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::Mercenary,
			entity_e::Adventurer,
			entity_e::Thetwo,
		};

	private:
		const i8 investiture;
		i8 stability;

		inline void set_stability(i8 value) noexcept { stability = clamp<i8>(value, 0, max_stability()); }
	
	public:		
		inline isoscel_t(i8 stability) noexcept : idle_animation{ indices::Isoscel, random_engine, true }, investiture{ stability }, stability{ investiture } {}

		inline i8 get_stability() const noexcept { return stability; }

		inline bool has_stability() const noexcept { return stability > 0; }

		inline i8 max_stability() const noexcept { return investiture; }

		constexpr bool can_survive(i8 damage_amount) const noexcept { return true; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		constexpr bool receive_damage(i8 damage_amount) noexcept { return false; }

		inline bool destabilize() noexcept {
			if (stability <= 0) {
				return true;
			}

			set_stability(stability - 1);

			return stability <= 0;
		}

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Isoscel), get_stability(), max_stability()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Isoscel) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_stability(), max_stability()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Isoscel; }
	};

	static_assert(sizeof(isoscel_t) <= NPCSizeCap, "furtive horror entity size must not exceed npc size cap!");
} // namespace necrowarp
