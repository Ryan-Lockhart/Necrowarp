#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/nock.hpp>
#include <necrowarp/commands/binary/retrieve.hpp>
#include <necrowarp/commands/binary/loose.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<ranger_t> {
		static constexpr entity_e value = entity_e::Ranger;
	};

	template<> struct is_entity_type<ranger_t, entity_e::Ranger> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Ranger> {
		using type = ranger_t;
	};

	template<> struct to_entity_group<entity_e::Ranger> {
		static constexpr entity_group_e value = entity_group_e::Ranger;
	};

	template<> struct is_good<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_clumsy<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<ranger_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> struct is_entity_command_valid<ranger_t, nock_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<ranger_t, retrieve_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<ranger_t, loose_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<ranger_t>{ glyphs::Ranger };

	struct ranger_t {
	  private:
		static inline std::bernoulli_distribution dodge_dis{ 0.333 };
		static inline std::bernoulli_distribution fumble_dis{ 0.333 };

		i8 ammunition;
		bool nocked;

		inline void set_ammunition(i8 value) noexcept { ammunition = clamp<i8>(value, 0, max_ammunition()); }

	  public:
		offset_t position;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 QuiverCapacity{ 8 };

		static constexpr i8 MinimumRange{ 2 };
		static constexpr i8 OptimalRange{ 4 };
		static constexpr i8 MaximumRange{ 6 };

		static inline bool in_range(offset_t origin, offset_t target) noexcept {
			return between<f32>(offset_t::distance<distance_function_e::Octile>(origin, target), ranger_t::MinimumRange, ranger_t::MaximumRange);
		}

		static inline bool in_range(f32 distance) noexcept {
			return between<f32>(distance, ranger_t::MinimumRange, ranger_t::MaximumRange);
		}

		inline bool in_range(offset_t target) const noexcept { return in_range(position, target); }

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

		static constexpr i8 DeathBoon{ 2 };

		inline ranger_t(offset_t position) noexcept : ammunition{ QuiverCapacity }, nocked{ false }, position{ position } {}
		
		inline i8 get_ammunition() const noexcept { return ammunition; }

		inline bool has_ammunition() const noexcept { return ammunition > 0; }

		inline bool ammunition_full() const noexcept { return ammunition >= QuiverCapacity; }

		constexpr i8 max_ammunition() const noexcept { return QuiverCapacity; }

		inline i8 get_capacity() const noexcept { return QuiverCapacity - ammunition; }

		inline bool can_nock() const noexcept { return !nocked && has_ammunition(); }
		
		inline void nock() noexcept {
			if (!can_nock()) {
				return;
			}

			set_ammunition(ammunition - 1);

			nocked = true;
		}

		inline bool can_retrieve() const noexcept { return !ammunition_full(); }
		
		inline void retrieve() noexcept {
			if (!can_retrieve()) {
				return;
			}

			set_ammunition(ammunition + 1);
		}

		inline bool can_retrieve(i8 amount) const noexcept { return amount <= get_capacity(); }

		inline void retrieve(i8 amount) noexcept {
			if (!can_retrieve(amount)) {
				return;
			}

			set_ammunition(ammunition + amount);
		}

		inline bool can_loose() const noexcept { return nocked; }
		
		template<map_type_e MapType> inline bool loose(offset_t position) noexcept;

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ true };

		template<RandomEngine Generator> static inline bool dodge(ref<Generator> generator) noexcept { return dodge_dis(generator); }

		template<RandomEngine Generator> static inline bool fumble(ref<Generator> generator) noexcept { return fumble_dis(generator); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{} ({})]",
				necrowarp::to_string(entity_e::Ranger),
				get_ammunition(),
				max_ammunition(),
				nocked ? "nocked" : "unnocked"
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Ranger) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{} (", get_ammunition(), max_ammunition()) })
				.concatenate(nocked ? runes_t{ "nocked", colors::dark::Green } : runes_t{ "unnocked", colors::dark::Yellow })
				.concatenate(runes_t{ ")]" });
			
			return colored_string;
		}

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<ranger_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<ranger_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<ranger_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<ranger_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Ranger; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<ranger_t> ranger) noexcept { return offset_t::std_hasher::operator()(ranger.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<ranger_t> lhs, cref<ranger_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<ranger_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<ranger_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};

	static_assert(sizeof(ranger_t) <= NPCSizeCap, "ranger entity size must not exceed npc size cap!");
} // namespace necrowarp
