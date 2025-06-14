#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

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

	template<> struct is_good_entity<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_fodder<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_elusive<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<ranger_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<ranger_t> {
		static constexpr fluid_e type = fluid_e::Blood;
	};

	template<> inline constexpr glyph_t entity_glyphs<ranger_t>{ glyphs::Ranger };

	struct ranger_t {
	  private:
		static inline std::bernoulli_distribution dodge_dis{ 0.4 };

	  public:
		offset_t position;

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

		static constexpr i8 DeathBoon{ 1 };

		inline ranger_t(offset_t position) noexcept : position{ position } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		static constexpr bool HasStaticDodge{ true };

		template<RandomEngine Engine> static inline bool dodge(ref<Engine> engine) noexcept { return fumble_dis(engine); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		template<map_type_e MapType> inline command_pack_t think() const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void draw() const noexcept { entity_atlas.draw(entity_glyphs<ranger_t>, position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<ranger_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(entity_glyphs<ranger_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(entity_glyphs<ranger_t>, position + camera.get_offset(), offset); }

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
} // namespace necrowarp
