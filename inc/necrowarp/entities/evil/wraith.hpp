#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<wraith_t> {
		static constexpr entity_e value = entity_e::Wraith;
	};

	template<> struct is_entity_type<wraith_t, entity_e::Wraith> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Wraith> {
		using type = wraith_t;
	};

	template<> struct to_entity_group<entity_e::Wraith> {
		static constexpr entity_group_e value = entity_group_e::Wraith;
	};

	template<> struct is_evil_entity<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<wraith_t> {
		static constexpr fluid_type_e type = fluid_type_e::Ichor;
	};

	template<> inline constexpr glyph_t entity_glyphs<wraith_t>{ glyphs::Wraith };

	struct wraith_t {
		offset_t position;

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:
		static constexpr i8 MaximumHealth{ 9 };
		static constexpr i8 MaximumDamage{ 2 };

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Priest,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Paladin
		};
		
		inline wraith_t(offset_t position, i8 health) noexcept : position{ position }, health{ health } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<wraith_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Wraith; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<wraith_t> wraith) noexcept { return offset_t::std_hasher::operator()(wraith.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<wraith_t> lhs, cref<wraith_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<wraith_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<wraith_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
