#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<skulker_t> {
		static constexpr entity_e value = entity_e::Skulker;
	};

	template<> struct is_entity_type<skulker_t, entity_e::Skulker> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Skulker> {
		using type = skulker_t;
	};

	template<> struct to_entity_group<entity_e::Skulker> {
		static constexpr entity_group_e value = entity_group_e::Skulker;
	};

	template<> struct is_good<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_sneaky<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_cleaver<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<skulker_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::BloodyIchor;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<skulker_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<skulker_t>{ glyphs::Skulker };

	enum struct concealment_e : u8 {
		Visible,
		Shrouded,
		Imperceptible
	};

	constexpr cstr to_string(concealment_e concealment) noexcept {
		switch (concealment) {
			case concealment_e::Visible: {
				return "visible";
			} case concealment_e::Shrouded: {
				return "shrouded";
			} case concealment_e::Imperceptible: {
				return "imperceptible";
			}
		}
	}

	constexpr color_t to_color(concealment_e concealment) noexcept {
		switch (concealment) {
			case concealment_e::Visible: {
				return colors::White;
			} case concealment_e::Shrouded: {
				return colors::light::Grey;
			} case concealment_e::Imperceptible: {
				return colors::dark::Grey;
			}
		}
	}

	constexpr runes_t to_colored_string(concealment_e concealment) noexcept { return runes_t{ to_string(concealment), to_color(concealment) }; }

	struct skulker_t {
	  public:
		mutable concealment_e concealment;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 3 };

		static constexpr i8 VisisbleRange{ 2 };
		static constexpr i8 ImperceptibleRange{ 8 };

		static constexpr std::array<entity_e, 18> EntityPriorities{
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
		};

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };

	  private:
		static constexpr concealment_e determine_concealment(f32 distance) noexcept {
			const isize rough_distance{ static_cast<isize>(std::floor(distance)) };

			if (rough_distance >= ImperceptibleRange) {
				return concealment_e::Imperceptible;
			} else if (rough_distance <= VisisbleRange) {
				return concealment_e::Visible;
			} else {
				return concealment_e::Shrouded;
			}
		}

		static inline std::uniform_int_distribution<i16> dodge_dis{ 0, 100 };

		template<concealment_e Concealment> static inline i8 dodge_threshold;

		template<> inline i8 dodge_threshold<concealment_e::Visible>{ 60 };
		template<> inline i8 dodge_threshold<concealment_e::Shrouded>{ 75 };
		template<> inline i8 dodge_threshold<concealment_e::Imperceptible>{ 90 };

		template<RandomEngine Generator> static inline i8 get_dodge_chance(ref<Generator> generator) noexcept { return static_cast<i8>(skulker_t::dodge_dis(generator)); }

	  public:

		inline skulker_t() noexcept : concealment{ concealment_e::Visible } {}

		inline skulker_t(concealment_e concealment) noexcept : concealment{ concealment } {}

		template<concealment_e Concealment> inline bool is() const noexcept { return concealment == Concealment; }

		inline bool is_imperceptible() const noexcept { return concealment == concealment_e::Imperceptible; }

		inline bool is_shrouded() const noexcept { return concealment == concealment_e::Shrouded; }

		inline bool is_visible() const noexcept { return concealment == concealment_e::Visible; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ false };

		template<RandomEngine Generator> inline bool dodge(ref<Generator> generator) noexcept {
			const i8 chance{ skulker_t::get_dodge_chance(generator) };

			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr concealment_e cval{ val };

				return chance < dodge_threshold<cval>;
			}, concealment);
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_e::Skulker), necrowarp::to_string(concealment)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Skulker) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(concealment))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			glyph_t glyph{ entity_glyphs<skulker_t> };

			if (!is_visible()) {
				glyph.color.fade(stealth_wave.current_value() + (is_imperceptible() ? 0.1 : 0.25));
			}

			return glyph;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Skulker; }
	};

	static_assert(sizeof(skulker_t) <= NPCSizeCap, "skulker entity size must not exceed npc size cap!");
} // namespace necrowarp
