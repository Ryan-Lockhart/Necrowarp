#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/objects/object.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/binary/exorcise.hpp>
#include <necrowarp/commands/binary/resurrect.hpp>
#include <necrowarp/commands/binary/anoint.hpp>
#include <necrowarp/commands/unary/suicide.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<priest_t> {
		static constexpr entity_e value = entity_e::Priest;
	};

	template<> struct is_entity_type<priest_t, entity_e::Priest> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Priest> {
		using type = priest_t;
	};

	template<> struct to_entity_group<entity_e::Priest> {
		static constexpr entity_group_e value = entity_group_e::Priest;
	};

	template<> struct is_good_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<priest_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_entity_command_valid<priest_t, exorcise_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<priest_t, resurrect_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<priest_t, anoint_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<priest_t, suicide_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<priest_t>{ glyphs::Priest };

	struct priest_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 0 };

		static constexpr i8 MaximumPiety{ 6 };
		static constexpr i8 StartingPiety{ 3 };

		static constexpr std::array<entity_e, 3> EntityPriorities{
			entity_e::Adventurer,
			entity_e::Mercenary
		};

		static constexpr std::array<object_e, 3> ObjectPriorities{
			object_e::Skull,
		};

		static constexpr i8 DeathBoon{ 4 };
		static constexpr i8 ExorcismBoon{ 3 };
		
		static constexpr i8 ResurrectCost{ 1 };
		static constexpr i8 AnointCost{ 2 };
		
	private:
		i8 piety;

		inline void set_piety(i8 value) noexcept { piety = clamp<i8>(value, 0, max_piety()); }

	public:
		inline priest_t(offset_t position) noexcept : position{ position }, piety{ StartingPiety } {}

		inline i8 get_piety() const noexcept { return piety; }

		inline bool has_piety() const noexcept { return piety > 0; }

		inline i8 max_piety() const noexcept { return MaximumPiety; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool can_resurrect() const noexcept { return piety >= ResurrectCost; }

		inline bool can_anoint() const noexcept { return piety >= AnointCost; }

		inline void pay_resurrect_cost() noexcept { set_piety(piety - ResurrectCost); }

		inline void pay_ordain_cost() noexcept { set_piety(piety - AnointCost); }

		inline void receive_exorcism_boon() noexcept { set_piety(piety + ExorcismBoon); }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<priest_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<priest_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<priest_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<priest_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Priest; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<priest_t> priest) noexcept { return offset_t::std_hasher::operator()(priest.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<priest_t> lhs, cref<priest_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<priest_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<priest_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
