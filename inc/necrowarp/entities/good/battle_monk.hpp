#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<battle_monk_t> {
		static constexpr entity_e value = entity_e::BattleMonk;
	};

	template<> struct is_entity_type<battle_monk_t, entity_e::BattleMonk> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::BattleMonk> {
		using type = battle_monk_t;
	};

	template<> struct to_entity_group<entity_e::BattleMonk> {
		static constexpr entity_group_e value = entity_group_e::BattleMonk;
	};

	template<> struct is_good_entity<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<battle_monk_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> inline constexpr glyph_t entity_glyphs<battle_monk_t>{ glyphs::BattleMonk };

	enum struct tranquility_e : u8 {
		Distraught,
		Focused,
		Zen
	};

	struct battle_monk_t {
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 StartingTranquility{ 4 };

		static constexpr i8 QiPoint{ 3 };

		static constexpr i8 MaximumQi{ 8 };

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

	  private:
		template<tranquility_e Tranquility> static inline i8 dodge_threshold;

		template<> inline i8 dodge_threshold<tranquility_e::Distraught>{ 30 };
		template<> inline i8 dodge_threshold<tranquility_e::Focused>{ 60 };
		template<> inline i8 dodge_threshold<tranquility_e::Zen>{ 90 };

		static inline std::uniform_int_distribution<i16> dodge_dis{ 0, 100 };
		
		i8 qi;

		inline void set_qi(i8 value) noexcept { qi = clamp<i8>(value, 0, max_qi()); }

	  public:
		inline battle_monk_t(offset_t position) noexcept : position{ position }, qi{ StartingTranquility } {}
		
		inline i8 get_qi() const noexcept { return qi; }

		inline bool has_qi() const noexcept { return qi > 0; }

		inline bool is_distraught() const noexcept { return qi <= QiPoint; }

		inline bool is_focused() const noexcept { return qi > QiPoint && qi < MaximumQi; }

		inline bool is_zen() const noexcept { return qi >= MaximumQi; }

		inline tranquility_e get_tranquility() const noexcept {
			if (is_distraught()) {
				return tranquility_e::Distraught;
			} else if (is_focused()) {
				return tranquility_e::Focused;
			} else {
				return tranquility_e::Zen;
			}
		}

		inline bool max_qi() const noexcept { return MaximumQi; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ false };

		template<RandomEngine Engine> inline bool dodge(ref<Engine> engine) noexcept {
			const i8 chance{ battle_monk_t::dodge_dis(engine) };

			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr tranquility_e cval{ val };

				return chance < dodge_threshold<cval>;
			}, get_tranquility());
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void harmonize() noexcept { set_qi(qi + 1); }

		inline void harmonize(i8 amount) noexcept { set_qi(qi + amount); }

		inline void destabilize() noexcept { set_qi(qi - 1); }

		inline void destabilize(i8 amount) noexcept { set_qi(qi - amount); }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<battle_monk_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<battle_monk_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<battle_monk_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<battle_monk_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::BattleMonk; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<battle_monk_t> battle_monk) noexcept { return offset_t::std_hasher::operator()(battle_monk.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<battle_monk_t> lhs, cref<battle_monk_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<battle_monk_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<battle_monk_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
