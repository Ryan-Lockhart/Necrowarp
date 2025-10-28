#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/binary/graze.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/bulk.tpp>
#include <necrowarp/constants/enums/species.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<fauna_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<fauna_t> {
		static constexpr entity_e value = entity_e::Fauna;
	};

	template<> struct is_entity_type<fauna_t, entity_e::Fauna> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Fauna> {
		using type = fauna_t;
	};

	template<> struct is_fodder<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_neutral<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_grazer<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<fauna_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_devourable<fauna_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<fauna_t, graze_t> {
		static constexpr bool value = true;
	};

	struct fauna_t {
		const species_e species;

		static constexpr i8 MaximumProtein{ 16 };
		static constexpr i8 MaximumBowels{ 8 };

		static constexpr std::array<object_e, 1> ObjectPriorities{ object_e::Flora };

		static constexpr i8 DigestionAmount{ 2 };

		static constexpr f32 DigestionChancePerProtein{ 0.025f };

		static constexpr f32 ProteinRatio{ 0.25f };
		static constexpr f32 BowelsRatio{ 0.50f };

		static constexpr f32 DefecationChancePerBowels{ 0.0125f };

		static constexpr f32 CrimsonShartChance{ 0.001f };

	private:
		static inline std::uniform_int_distribution<u16> species_dis{ static_cast<u16>(species_e::SanguineTetrapod), static_cast<u16>(species_e::InfernalHexapod) };

		template<RandomEngine Generator> static inline species_e random_species(ref<Generator> engine) noexcept { return static_cast<species_e>(species_dis(engine)); }

		i8 protein;
		i8 bowels;

		inline void set_protein(i8 value) noexcept { protein = clamp<i8>(value, 0, max_protein()); }

		inline void set_bowels(i8 value) noexcept { bowels = clamp<i8>(value, 0, max_bowels()); }

		static inline std::bernoulli_distribution crimson_shart_dis{ CrimsonShartChance };

	public:
		inline fauna_t(species_e species) noexcept : species{ species }, protein{ 0 }, bowels{ 0 } {}

		template<RandomEngine Generator> inline fauna_t(ref<Generator> engine) noexcept : species{ random_species(engine) }, protein{ 0 }, bowels{ 0 } {}
		
		inline i8 get_protein() const noexcept { return protein; }

		inline bool has_protein() const noexcept { return protein > 0; }

		constexpr i8 max_protein() const noexcept { return MaximumProtein; }

		inline i8 protein_value() const noexcept { return static_cast<i8>(protein * ProteinRatio); }
		
		inline i8 get_bowels() const noexcept { return bowels; }

		inline bool has_bowels() const noexcept { return bowels > 0; }

		constexpr i8 max_bowels() const noexcept { return MaximumBowels; }

		inline void fatten() noexcept { set_protein(protein + 1); }

		inline void fatten(i8 amount) noexcept { set_protein(protein + amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline bool can_digest() const noexcept { return protein >= DigestionAmount && bowels < max_bowels(); }

		inline bool can_defecate() const noexcept { return bowels > 0; }

		inline bool digest() noexcept {
			if (!can_digest()) {
				return false;
			}

			set_protein(protein - DigestionAmount);
			set_bowels(bowels + 1);

			return true;
		}

		template<RandomEngine Generator> inline bool digest(ref<Generator> engine) noexcept {
			if (!can_digest() || !std::bernoulli_distribution{ protein * DigestionChancePerProtein }(engine)) {
				return false;
			}

			set_protein(protein - DigestionAmount);
			set_bowels(bowels + 1);

			return true;
		}

		template<map_type_e MapType> inline bool defecate(offset_t position) noexcept;

		template<map_type_e MapType, RandomEngine Generator> inline bool defecate(offset_t position, ref<Generator> engine) noexcept;

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{}, {}/{}]",
				necrowarp::to_string(species),
				get_protein(), max_protein(),
				get_bowels(), max_bowels()
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(species) };

			colored_string.concatenate(runes_t{ std::format(", [{}/{}, {}/{}]", get_protein(), max_protein(), get_bowels(), max_bowels()) });

			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Fauna, static_cast<u8>(species) }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Fauna; }
	};

	static_assert(sizeof(fauna_t) <= NPCSizeCap, "fauna entity size must not exceed npc size cap!");
} // namespace necrowarp
