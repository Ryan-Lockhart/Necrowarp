#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/objects/object.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/decay.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<skeleton_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<skeleton_t> {
		static constexpr entity_e value = entity_e::Skeleton;
	};

	template<> struct is_entity_type<skeleton_t, entity_e::Skeleton> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Skeleton> {
		using type = skeleton_t;
	};

	template<> struct to_entity_group<entity_e::Skeleton> {
		static constexpr entity_group_e value = entity_group_e::Skeleton;
	};

	template<> struct is_evil<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_clumsy<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<skeleton_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Ichor;

		static constexpr bool conditional = true;
	};

	struct skeleton_t {
	  private:
		static inline std::bernoulli_distribution fumble_dis{ 0.25 };

	  public:
		const decay_e state;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

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
		
		constexpr i8 armor_boon() const noexcept {
			return static_cast<i8>(state) + 1;
		}

		inline skeleton_t() noexcept : state{ decay_e::Fresh } {}

		inline skeleton_t(decay_e state) noexcept : state{ state } {}

		inline bool is_fresh() const noexcept { return state == decay_e::Fresh; }

		inline bool is_animate() const noexcept { return state == decay_e::Animate; }

		inline bool is_rotted() const noexcept { return state == decay_e::Rotted; }

		inline bool can_bleed() const noexcept { return is_rotted(); }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> generator) noexcept { return fumble_dis(generator); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_e::Skeleton), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Skeleton) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case decay_e::Rotted: {
					return glyphs::RottedSkeleton;
				} case decay_e::Animate: {
					return glyphs::AnimateSkeleton;
				} case decay_e::Fresh: {
					return glyphs::FreshSkeleton;
				}
			}
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Skeleton; }
	};

	static_assert(sizeof(skeleton_t) <= NPCSizeCap, "skeleton entity size must not exceed npc size cap!");
} // namespace necrowarp
