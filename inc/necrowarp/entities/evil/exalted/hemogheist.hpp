#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/eviscerate.hpp>

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

	template<> struct is_volumetric<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<hemogheist_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_thirsty<hemogheist_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_concussable<hemogheist_t> {
		static constexpr bool value = false;
	};

	template<> struct is_cleaver<hemogheist_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<hemogheist_t, eviscerate_t> {
		static constexpr bool value = true;
	};

	struct hemogheist_t {
		keyframe_t idle_animation;

		static constexpr f16 ThirstRetention{ 1.00f };

		static constexpr i8 MaximumDamage{ 5 };

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
		const f16 investiture;
		f16 volume;

		inline void set_volume(f32 value) noexcept { volume = clamp<f32>(value, 0, max_volume()); }

	public:		
		inline hemogheist_t(i8 catalyst) noexcept : idle_animation{ indices::Hemogheist, random_engine, true }, investiture{ fluid_pool_volume(catalyst) }, volume{ investiture } {}

		inline f32 get_volume() const noexcept { return volume; }

		inline bool has_volume() const noexcept { return volume > 0; }

		inline f32 max_volume() const noexcept { return investiture; }

		inline bool can_imbibe() const noexcept { return volume < max_volume(); }

		inline void imbibe() noexcept {
			if (!can_imbibe()) {
				return;
			}

			set_volume(volume + fluid_pool_volume() * ThirstRetention);
		}

		inline void imbibe(i8 amount) noexcept {
			if (!can_imbibe()) {
				return;
			}

			set_volume(volume + fluid_pool_volume(amount) * ThirstRetention);
		}

		inline bool can_survive(f16 damage_amount) const noexcept { return volume > damage_amount; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(f16 damage_amount) noexcept { set_volume(volume - damage_amount); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void killed(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{:.1f}L/{:.1f}L]", necrowarp::to_string(entity_e::Hemogheist), get_volume(), max_volume()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Hemogheist) };

			colored_string.concatenate(runes_t{ std::format(" [{:.1f}L/{:.1f}L]", get_volume(), max_volume()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Hemogheist; }
	};

	static_assert(sizeof(hemogheist_t) <= NPCSizeCap, "hemogheist entity size must not exceed npc size cap!");
} // namespace necrowarp
