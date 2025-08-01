#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/meditate.hpp>
#include <necrowarp/commands/binary/batter.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<battle_monk_t> {
		static constexpr bool value = true;
	};

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

	template<> struct is_good<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<battle_monk_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_serene<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<battle_monk_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<battle_monk_t, meditate_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<battle_monk_t, batter_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<battle_monk_t>{ glyphs::BattleMonk };

	enum struct tranquility_e : u8 {
		Distraught,
		Focused,
		Zen
	};

	constexpr cstr to_string(tranquility_e tranquility) noexcept {
		switch (tranquility) {
			case tranquility_e::Distraught: {
				return "distraught";
			} case tranquility_e::Focused: {
				return "focused";
			} case tranquility_e::Zen: {
				return "zen";
			}
		}
	}

	constexpr color_t to_color(tranquility_e tranquility) noexcept {
		switch (tranquility) {
			case tranquility_e::Distraught: {
				return colors::dark::Red;
			} case tranquility_e::Focused: {
				return colors::dark::Yellow;
			} case tranquility_e::Zen: {
				return colors::light::Cyan;
			}
		}
	}

	constexpr runes_t to_colored_string(tranquility_e tranquility) noexcept { return runes_t{ to_string(tranquility), to_color(tranquility) }; }

	struct battle_monk_t {
		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 QiPoint{ 3 };
		static constexpr i8 MaximumQi{ 8 };

		static constexpr i8 StartingTranquility{ QiPoint + 1 };

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

		static constexpr i8 DeathBoon{ 3 };

		static constexpr i8 ProteinValue{ 1 };

	  private:
		template<tranquility_e Tranquility> static inline i8 dodge_threshold;

		template<> inline i8 dodge_threshold<tranquility_e::Distraught>{ 10 };
		template<> inline i8 dodge_threshold<tranquility_e::Focused>{ 40 };
		template<> inline i8 dodge_threshold<tranquility_e::Zen>{ 70 };

		static inline std::uniform_int_distribution<i16> dodge_dis{ 0, 100 };

		template<RandomEngine Generator> static inline i8 get_dodge_chance(ref<Generator> generator) noexcept { return static_cast<i8>(battle_monk_t::dodge_dis(generator)); }
		
		i8 qi;

		inline void set_qi(i8 value) noexcept { qi = clamp<i8>(value, 0, max_qi()); }

	  public:
		inline battle_monk_t() noexcept : qi{ StartingTranquility } {}
		
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

		constexpr i8 max_qi() const noexcept { return MaximumQi; }

		inline bool can_survive(i8 damage_amount) const noexcept { return !is_distraught() || damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ false };

		template<RandomEngine Generator> inline bool dodge(ref<Generator> generator) noexcept {
			const i8 chance{ battle_monk_t::get_dodge_chance(generator) };

			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr tranquility_e cval{ val };

				return chance < dodge_threshold<cval>;
			}, get_tranquility());
		}

		constexpr i8 get_damage() const noexcept { return MaximumDamage; }

		constexpr i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			destabilize();

			return true;
		}

		inline void harmonize() noexcept { set_qi(qi + 1); }

		inline void harmonize(i8 amount) noexcept { set_qi(qi + amount); }

		inline void destabilize() noexcept { set_qi(qi - 1); }

		inline void destabilize(i8 amount) noexcept { set_qi(qi - amount); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{} ({})]", necrowarp::to_string(entity_e::BattleMonk), get_qi(), max_qi(), necrowarp::to_string(get_tranquility()));
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::BattleMonk) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{} (", get_qi(), max_qi()) })
				.concatenate(necrowarp::to_colored_string(get_tranquility()))
				.concatenate(runes_t{ ")]" });

			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<battle_monk_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<battle_monk_t>, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(entity_glyphs<battle_monk_t>, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::BattleMonk; }
	};

	static_assert(sizeof(battle_monk_t) <= NPCSizeCap, "battle_monk entity size must not exceed npc size cap!");
} // namespace necrowarp
