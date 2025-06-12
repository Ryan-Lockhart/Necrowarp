#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<berserker_t> {
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

	template<> struct is_good_entity<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<berserker_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<berserker_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_berker<berserker_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<berserker_t>{ glyphs::Berserker };

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

	constexpr runes_t to_colored_string(endurance_e endurance) noexcept {
		const cstr string{ to_string(endurance) };

		switch (endurance) {
			case endurance_e::Fresh: {
				return runes_t{ string, colors::dark::Green };
			} case endurance_e::Fatigued: {
				return runes_t{ string, colors::dark::Yellow };
			} case endurance_e::Exhausted: {
				return runes_t{ string, colors::dark::Red };
			}
		}
	}

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

	constexpr runes_t to_colored_string(temperament_e temperament) noexcept {
		const cstr string{ to_string(temperament) };

		switch (temperament) {
			case temperament_e::Calm: {
				return runes_t{ string, colors::light::Blue };
			} case temperament_e::Enraged: {
				return runes_t{ string, colors::Orange };
			}
		}
	}

	struct berserker_t {
		offset_t position;

		static constexpr i8 StartingFatigue{ 0 };

		static constexpr i8 FatiguePoint{ 3 };

		static constexpr i8 MaximumFatigue{ 8 };
		static constexpr i8 MaximumDamage{ 2 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::AnimatedSuit,
			entity_e::Bloodhound,
			entity_e::Cultist,
			entity_e::Skeleton,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::FleshGolem,
			entity_e::DeathKnight,
		};

		static constexpr i8 DeathBoon{ 2 };
		
	private:
		i8 fatigue;
		temperament_e temperament;
		fluid_e spatter;

		inline void set_fatigue(i8 value) noexcept { fatigue = clamp<i8>(value, 0, max_fatigue()); }

	public:
		inline berserker_t(offset_t position) noexcept : position{ position }, fatigue{ StartingFatigue }, temperament{ temperament_e::Calm } {}
		
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

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept {
			if (damage_amount <= 0) {
				return;
			}

			spatter += fluid_type<berserker_t>::type;
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

		inline void enrage() noexcept { temperament = temperament_e::Enraged; }

		inline void enspatter(fluid_e with) noexcept {
			spatter += with;
		}

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

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

		inline glyph_t current_glyph() const noexcept {
			switch (spatter) {
				case fluid_e::None: {
					return glyphs::Berserker;
				} case fluid_e::Blood: {
					return glyphs::BloodyBerserker;
				} case fluid_e::Ichor: {
					return glyphs::IchorousBerserker;
				} case fluid_e::BloodyIchor: {
					return glyphs::BloodyIchorousBerserker;
				}
			}
		}

		inline void draw() const noexcept { entity_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Berserker; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<berserker_t> berserker) noexcept { return offset_t::std_hasher::operator()(berserker.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<berserker_t> lhs, cref<berserker_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<berserker_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<berserker_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
