#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/fluid.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<berserker_t> {
		static constexpr entity_e value = entity_e::Berserker;
	};

	template<> struct is_entity_type<berserker_t, entity_e::Berserker> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Berserker> {
		using type = berserker_t;
	};

	template<> struct to_entity_group<entity_e::Berserker> {
		static constexpr entity_group_e value = entity_group_e::Berserker;
	};

	template<> struct is_good<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<berserker_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_berker<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_spatterable<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_cleaver<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<berserker_t> {
		static constexpr bool value = true;
	};

	enum struct endurance_e : u8 {
		Fresh,
		Fatigued,
		Exhausted
	};

	constexpr cstr to_string(endurance_e endurance) noexcept {
		switch (endurance) {
			case endurance_e::Fresh: {
				return "fresh";
			} case endurance_e::Fatigued: {
				return "fatigued";
			} case endurance_e::Exhausted: {
				return "exhausted";
			}
		}
	}

	constexpr color_t to_color(endurance_e endurance) noexcept {
		switch (endurance) {
			case endurance_e::Fresh: {
				return colors::dark::Green;
			} case endurance_e::Fatigued: {
				return colors::dark::Yellow;
			} case endurance_e::Exhausted: {
				return colors::dark::Red;
			}
		}
	}

	constexpr runes_t to_colored_string(endurance_e endurance) noexcept { return runes_t{ to_string(endurance), to_color(endurance) }; }

	enum struct temperament_e : u8 {
		Calm,
		Enraged
	};

	constexpr cstr to_string(temperament_e temperament) noexcept {
		switch (temperament) {
			case temperament_e::Calm: {
				return "calm";
			} case temperament_e::Enraged: {
				return "enraged";
			} 
		}
	}

	constexpr color_t to_color(temperament_e temperament) noexcept {
		switch (temperament) {
			case temperament_e::Calm: {
				return colors::light::Blue;
			} case temperament_e::Enraged: {
				return colors::Orange;
			}
		}
	}

	constexpr runes_t to_colored_string(temperament_e temperament) noexcept { return runes_t{ to_string(temperament), to_color(temperament) }; }

	struct berserker_t {
		static constexpr i8 MinimumFatigue{ 0 };
		static constexpr i8 MaximumFatigue{ 8 };

		static constexpr i8 StartingFatigue{ MinimumFatigue };

		static constexpr i8 FatiguePoint{ 3 };

		static constexpr i8 MaximumDamage{ 2 };

		static constexpr std::array<entity_e, 18> EntityPriorities{
			entity_e::Player,
			entity_e::DeathKnight,
			entity_e::AnimatedSuit,
			entity_e::Isoscel,
			entity_e::FurtiveHorror,
			entity_e::Dreadwurm,
			entity_e::Chromalese,
			entity_e::Hamaz,
			entity_e::Draugaz,
			entity_e::FleshGolem,
			entity_e::Abomination,
			entity_e::Wraith,
			entity_e::Cultist,
			entity_e::Hemogheist,
			entity_e::Bloodhound,
			entity_e::Bonespur,
			entity_e::Skeleton,
			entity_e::Thetwo,
		};

		static constexpr i8 DeathBoon{ 3 };

		static constexpr i8 ProteinValue{ 1 };
		
	private:
		i8 fatigue;
		temperament_e temperament;
		fluid_e spatter;

		inline void set_fatigue(i8 value) noexcept { fatigue = clamp<i8>(value, 0, max_fatigue()); }

	public:
		inline berserker_t() noexcept : fatigue{ StartingFatigue }, temperament{ temperament_e::Calm }, spatter{ fluid_e::None } {}
		
		inline i8 get_fatigue() const noexcept { return fatigue; }

		inline bool has_fatigue() const noexcept { return fatigue > 0; }

		inline bool is_fresh() const noexcept { return fatigue <= FatiguePoint; }

		inline bool is_fatigued() const noexcept { return fatigue > FatiguePoint && fatigue < MaximumFatigue; }

		inline bool is_exhausted() const noexcept { return fatigue >= MaximumFatigue; }

		inline endurance_e get_endurance() const noexcept {
			if (is_fresh()) {
				return endurance_e::Fresh;
			} else if (is_exhausted()) {
				return endurance_e::Exhausted;
			} else {
				return endurance_e::Fatigued;
			}
		}

		inline temperament_e get_temperament() const noexcept { return temperament; }

		inline bool is_calm() const noexcept { return temperament == temperament_e::Calm; }
		
		inline bool is_enraged() const noexcept { return temperament == temperament_e::Enraged; }

		inline fluid_e get_spatter() const noexcept { return spatter; }

		inline bool is_vulnerable() const noexcept { return is_enraged() && is_exhausted(); }

		constexpr i8 max_fatigue() const noexcept { return MaximumFatigue; }

		inline bool can_survive(i8 damage_amount) const noexcept { return !is_vulnerable() || damage_amount <= 0; }

		constexpr i8 get_damage() const noexcept { return MaximumDamage; }

		constexpr i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return false;
			}

			return true;
		}

		inline void exhaust() noexcept {
			if (is_calm()) {
				return;
			}

			set_fatigue(fatigue + 1);
		}

		inline void exhaust(i8 amount) noexcept {
			if (is_calm()) {
				return;
			}

			set_fatigue(fatigue + amount);
		}

		inline void recuperate() noexcept {
			if (is_calm()) {
				return;
			}

			set_fatigue(fatigue - 1);
		}

		inline void recuperate(i8 amount) noexcept {
			if (is_calm()) {
				return;
			}

			set_fatigue(fatigue - amount);
		}

		inline void soothe() noexcept { temperament = temperament_e::Calm; }

		inline void enrage() noexcept { temperament = temperament_e::Enraged; }

		inline void enspatter(fluid_e with) noexcept {
			spatter += with;
		}

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{} ({})] ({} | {})",
				necrowarp::to_string(entity_e::Berserker),
				get_fatigue(),
				max_fatigue(),
				necrowarp::to_string(get_endurance()),
				necrowarp::to_string(temperament),
				necrowarp::to_string(spatter)
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Berserker) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{} (", get_fatigue(), max_fatigue()) })
				.concatenate(necrowarp::to_colored_string(get_endurance()))
				.concatenate(runes_t{ ")] (" })
				.concatenate(necrowarp::to_colored_string(temperament))
				.concatenate(runes_t{ " | " })
				.concatenate(spatter != fluid_e::None ? necrowarp::to_colored_string(spatter) : runes_t{ "unstained", colors::light::Grey })
				.concatenate(runes_t{ ")" });

			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Berserker, static_cast<u8>(spatter) }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Berserker; }
	};

	static_assert(sizeof(berserker_t) <= NPCSizeCap, "berserker entity size must not exceed npc size cap!");
} // namespace necrowarp
