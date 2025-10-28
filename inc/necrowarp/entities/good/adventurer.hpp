#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_variants<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<adventurer_t> {
		static constexpr entity_e value = entity_e::Adventurer;
	};

	template<> struct is_entity_type<adventurer_t, entity_e::Adventurer> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Adventurer> {
		using type = adventurer_t;
	};

	template<> struct is_good<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_clumsy<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<adventurer_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_encouragable<adventurer_t> {
		static constexpr bool value = true;
	};

	struct adventurer_t {
		const u8 variant;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr affliction_e AfflictionEquilibrium{ affliction_e::Stable };

		static constexpr f32 BaseDemoralizeChance{ 0.25f };
		static constexpr f32 BaseEmboldenChance{ 0.125f };

		static constexpr std::array<entity_e, 19> EntityPriorities{
			entity_e::Player,
			entity_e::Skeleton,
			entity_e::Cultist,
			entity_e::Bloodhound,
			entity_e::AnimatedSuit,
			entity_e::Abomination,
			entity_e::Draugaz,
			entity_e::Hamaz,
			entity_e::Chromalese,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::Hemogheist,
			entity_e::DeathKnight,
			entity_e::FleshGolem,
			entity_e::Dreadwurm,
			entity_e::FurtiveHorror,
			entity_e::Isoscel,
			entity_e::Thetwo,
			entity_e::Fauna,
		};

		static constexpr f16 MinLeashRange{ 4 };
		static constexpr f16 MaxLeashRange{ 8 };

		static constexpr i8 DeathBoon{ 1 };

		static constexpr i8 ProteinValue{ 1 };

	  private:
		static constexpr u8 VariantCount{ 9 };

		static inline std::uniform_int_distribution<u16> variant_dis{ 0, VariantCount - 1 };

		static inline std::bernoulli_distribution fumble_dis{ 0.25 };

		template<RandomEngine Generator> static inline u8 random_variant(ref<Generator> engine) noexcept { return static_cast<u8>(variant_dis(engine)); }

	  public:
		inline adventurer_t() noexcept : variant{ random_variant(random_engine) } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> engine) noexcept { return fumble_dis(engine); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Adventurer, variant }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Adventurer; }
	};

	static_assert(sizeof(adventurer_t) <= NPCSizeCap, "adventurer entity size must not exceed npc size cap!");
} // namespace necrowarp
