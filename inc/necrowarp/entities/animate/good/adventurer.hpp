#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<adventurer_t, entity_type_t::Adventurer> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Adventurer> {
		using type = adventurer_t;
	};

	template<> struct is_good_entity<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<adventurer_t> {
		static constexpr fluid_type_e type = fluid_type_e::Blood;
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
		offset_t position;

		inline adventurer_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<adventurer_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Adventurer; }

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 DeathBoon{ 1 };

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<adventurer_t> adventurer) noexcept { return offset_t::hasher::operator()(adventurer.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<adventurer_t> lhs, cref<adventurer_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<adventurer_t> lhs, offset_t rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<adventurer_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
