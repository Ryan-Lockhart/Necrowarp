#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/binary/resuscitate.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<medicus_t> {
		static constexpr entity_e value = entity_e::Medicus;
	};

	template<> struct is_entity_type<medicus_t, entity_e::Medicus> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Medicus> {
		using type = medicus_t;
	};

	template<> struct is_good<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<medicus_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_stockable<medicus_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<medicus_t, resuscitate_t> {
		static constexpr bool value = true;
	};

	struct medicus_t {
		static constexpr i8 MaximumHealth{ 1 };

		static constexpr i8 MaximumSatchels{ 4 };

		static constexpr i8 MinimumRestockTurns{ 3 };
		static constexpr i8 DepthPerRestockTurn{ 10 };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };

	  private:
		i8 satchels;

		inline void set_satchels(i8 value) noexcept { satchels = clamp<i8>(value, 0, max_satchels()); }

	  public:
		inline medicus_t() noexcept : satchels{ MaximumSatchels } {}

		inline medicus_t(i8 amount) noexcept : satchels{ clamp<i8>(amount, 0, max_satchels()) } {}

		inline i8 get_satchels() const noexcept { return satchels; }

		inline bool has_satchels() const noexcept { return satchels > 0; }

		constexpr i8 max_satchels() const noexcept { return MaximumSatchels; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		template<map_type_e MapType> inline std::optional<entity_e> can_resuscitate(offset_t position) const noexcept;

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline bool resuscitate(offset_t position) noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}]", necrowarp::to_string(entity_e::Medicus), get_satchels(), max_satchels()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Medicus) };

			colored_string.concatenate(runes_t{ std::format(" [{}/{}]", get_satchels(), max_satchels()) });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept { return has_satchels() ? glyphs::StockedMedicus : glyphs::DepletedMedicus; }

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Medicus; }
	};

	static_assert(sizeof(medicus_t) <= NPCSizeCap, "medicus entity size must not exceed npc size cap!");
} // namespace necrowarp
