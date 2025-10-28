#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/binary/nuzzle.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/demeanor.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct has_animation<plonzo_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<plonzo_t> {
		static constexpr entity_e value = entity_e::Plonzo;
	};

	template<> struct is_entity_type<plonzo_t, entity_e::Plonzo> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Plonzo> {
		using type = plonzo_t;
	};

	template<> struct is_fodder<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_neutral<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_docile<plonzo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<plonzo_t, nuzzle_t> {
		static constexpr bool value = true;
	};

	struct plonzo_t {
		keyframe_t idle_animation;

		static constexpr f32 DemeanorRandomizeChance{ 0.1f };

		static constexpr std::array<entity_e, 1> EntityPriorities{ entity_e::Player };

	private:
		static inline std::bernoulli_distribution randomize_chance{ DemeanorRandomizeChance };

		static inline std::uniform_int_distribution<u16> demeanor_dis{ static_cast<u16>(demeanor_e::Aloof), static_cast<u16>(demeanor_e::Jubilant) };

		template<RandomEngine Generator> static inline demeanor_e random_demeanor(ref<Generator> engine) noexcept { return static_cast<demeanor_e>(demeanor_dis(engine)); }

		demeanor_e demeanor;

		inline void set_demeanor(demeanor_e demeanor) noexcept { this->demeanor = demeanor; }

	public:
		inline plonzo_t() noexcept : idle_animation{ indices::Plonzo, random_engine, true }, demeanor{ demeanor_e::Aloof } {}

		inline plonzo_t(demeanor_e demeanor) noexcept : idle_animation{ indices::Plonzo, random_engine, true }, demeanor{ demeanor } {}

		template<RandomEngine Generator> inline plonzo_t(ref<Generator> engine) noexcept : idle_animation{ indices::Plonzo, random_engine, true }, demeanor{ random_demeanor(engine) } {}
		
		inline demeanor_e get_demeanor() const noexcept { return demeanor; }

		template<RandomEngine Generator> inline void randomize(ref<Generator> engine) noexcept {
			if (!randomize_chance(engine)) {
				return;
			}

			demeanor = random_demeanor(engine);
		}

		inline bool can_survive(i8 damage_amount) const noexcept { return true; }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(entity_e::Plonzo), necrowarp::to_string(demeanor)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Plonzo) };

			colored_string.concatenate(runes_t{ " (" });
			colored_string.concatenate(necrowarp::to_colored_string(demeanor));
			colored_string.concatenate(runes_t{ ")" });

			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Plonzo; }
	};

	static_assert(sizeof(plonzo_t) <= NPCSizeCap, "plonzo entity size must not exceed npc size cap!");
} // namespace necrowarp
