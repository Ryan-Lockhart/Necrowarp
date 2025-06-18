#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

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

	template<> struct is_good_entity<skulker_t> {
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

	template<> struct is_bleeder<skulker_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<skulker_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> inline constexpr glyph_t entity_glyphs<skulker_t>{ glyphs::Skulker };

	enum struct concealment_e : u8 {
		Visible,
		Shrouded,
		Imperceptible
	};

	struct skulker_t {
	  private:
		static inline std::uniform_int_distribution<i16> dodge_dis{ 0, 100 };

		template<concealment_e Concealment> static inline i8 dodge_threshold;

		template<> inline i8 dodge_threshold<concealment_e::Visible>{ 50 };
		template<> inline i8 dodge_threshold<concealment_e::Shrouded>{ 75 };
		template<> inline i8 dodge_threshold<concealment_e::Imperceptible>{ 100 };

		template<RandomEngine Engine> static inline i8 get_dodge_chance(ref<Engine> engine) noexcept { return static_cast<i8>(skulker_t::dodge_dis(engine)); }

	  public:
		offset_t position;
		concealment_e concealment;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 MinimumRange{ 2 };
		static constexpr i8 MaximumRange{ 8 };

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

		inline skulker_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ false };

		template<RandomEngine Engine> inline bool dodge(ref<Engine> engine) noexcept {
			const i8 chance{ skulker_t::get_dodge_chance(engine) };

			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr concealment_e cval{ val };

				return chance < dodge_threshold<cval>;
			}, concealment);
		}

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline void update() noexcept;

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<skulker_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<skulker_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<skulker_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<skulker_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Skulker; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skulker_t> skulker) noexcept { return offset_t::std_hasher::operator()(skulker.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<skulker_t> lhs, cref<skulker_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skulker_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<skulker_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
