#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

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

	template<> struct to_entity_group<entity_e::Adventurer> {
		static constexpr entity_group_e value = entity_group_e::Adventurer;
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

	template<> inline constexpr glyph_t entity_glyphs<adventurer_t>{ glyphs::Adventurer };

	struct adventurer_t {
	  private:
		static inline std::bernoulli_distribution fumble_dis{ 0.25 };

	  public:
		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 14> EntityPriorities{
			entity_e::Player,
			entity_e::Skeleton,
			entity_e::Cultist,
			entity_e::Bloodhound,
			entity_e::AnimatedSuit,
			entity_e::Abomination,
			entity_e::Draugaz,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::Hemogheist,
			entity_e::DeathKnight,
			entity_e::FleshGolem,
			entity_e::Dreadwurm,
			entity_e::Thetwo,
		};

		static constexpr i8 DeathBoon{ 1 };

		static constexpr i8 ProteinValue{ 1 };

		inline adventurer_t() noexcept {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> generator) noexcept { return fumble_dis(generator); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Adventurer; }
	};

	static_assert(sizeof(adventurer_t) <= NPCSizeCap, "adventurer entity size must not exceed npc size cap!");
} // namespace necrowarp
