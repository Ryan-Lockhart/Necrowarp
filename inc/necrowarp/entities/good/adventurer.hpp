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

	template<> struct is_good_entity<adventurer_t> {
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
	};

	template<> struct fluid_type<adventurer_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_devourable<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<adventurer_t>{ glyphs::Adventurer };

	template<> inline constexpr bool has_death_sound<adventurer_t>{ true };

	template<> inline constexpr usize num_death_sounds<adventurer_t>{ 6 };

	template<> inline clip_pool_t<num_death_sounds<adventurer_t>> death_sounds<adventurer_t>{
		sound_t{ "res/sfx/clips/death/adv/adv_death_0.flac" },
		sound_t{ "res/sfx/clips/death/adv/adv_death_1.flac" },
		sound_t{ "res/sfx/clips/death/adv/adv_death_2.flac" },
		sound_t{ "res/sfx/clips/death/adv/adv_death_3.flac" },
		sound_t{ "res/sfx/clips/death/adv/adv_death_4.flac" },
		sound_t{ "res/sfx/clips/death/adv/adv_death_5.flac" }
	};

	struct adventurer_t {
	  private:
		static inline std::bernoulli_distribution fumble_dis{ 0.25 };

	  public:
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::Skeleton,
			entity_e::Cultist,
			entity_e::Bloodhound,
			entity_e::AnimatedSuit,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::FleshGolem,
			entity_e::DeathKnight,
		};

		static constexpr i8 DeathBoon{ 1 };

		inline adventurer_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> generator) noexcept { return fumble_dis(generator); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Adventurer; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<adventurer_t> adventurer) noexcept { return offset_t::std_hasher::operator()(adventurer.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<adventurer_t> lhs, cref<adventurer_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<adventurer_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<adventurer_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
