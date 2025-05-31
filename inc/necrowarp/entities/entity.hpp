#pragma once

#include <bleak.hpp>

#include <cstddef>

#include <necrowarp/cell.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t;

	// lesser minions

	struct skeleton_t;
	struct cultist_t;
	struct bloodhound_t;
	struct animated_suit_t;

	// exalted minions

	struct bonespur_t;
	struct wraith_t;
	struct flesh_golem_t;
	struct death_knight_t;

	// do-gooders

	struct adventurer_t;
	struct mercenary_t;
	struct paladin_t;
	struct priest_t;

	#define ALL_EVIL_NPCS \
		skeleton_t, \
		cultist_t, \
		bloodhound_t, \
		animated_suit_t, \
		bonespur_t, \
		wraith_t, \
		flesh_golem_t, \
		death_knight_t
	
	#define ALL_EVIL \
		player_t, \
		ALL_EVIL_NPCS

	#define ALL_GOOD_NPCS \
		adventurer_t, \
		mercenary_t, \
		paladin_t, \
		priest_t
	
	#define ALL_GOOD \
		ALL_GOOD_NPCS

	#define ALL_NON_PLAYER \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS

	#define ALL_ENTITIES \
		player_t, \
		ALL_NON_PLAYER

	enum struct entity_e : u8 {
		None = 0,
		Player,
		Skeleton,
		Cultist,
		Bloodhound,
		AnimatedSuit,
		Bonespur,
		Wraith,
		FleshGolem,
		DeathKnight,
		Adventurer,
		Mercenary,
		Paladin,
		Priest
	};

	static constexpr cstr to_string(entity_e type) noexcept {
		switch (type) {
			case entity_e::None: {
				return "none";
			} case entity_e::Player: {
				return "player";
			} case entity_e::Skeleton: {
				return "skeleton";
			} case entity_e::Cultist: {
				return "cultist";
			} case entity_e::Bloodhound: {
				return "bloodhound";
			} case entity_e::AnimatedSuit: {
				return "animated suit of armor";
			} case entity_e::Bonespur: {
				return "bonespur";
			} case entity_e::Wraith: {
				return "wraith";
			} case entity_e::FleshGolem: {
				return "flesh golem";
			} case entity_e::DeathKnight: {
				return "death knight";
			} case entity_e::Adventurer: {
				return "adventurer";
			} case entity_e::Mercenary: {
				return "mercenary";
			} case entity_e::Paladin: {
				return "paladin";
			} case entity_e::Priest: {
				return "priest";
			} default: {
				return "unknown";
			}
		}
	}

	static constexpr runes_t to_colored_string(entity_e type) noexcept {
		const cstr string{ to_string(type) };

		switch (type) {
			case entity_e::None: {
				return runes_t{ string, colors::Grey };
			} case entity_e::Player: {
				return runes_t{ string, colors::Magenta };
			} case entity_e::Skeleton: {
				return runes_t{ string, colors::White };
			} case entity_e::Cultist: {
				return runes_t{ string, colors::dark::Magenta };
			} case entity_e::Bloodhound: {
				return runes_t{ string, colors::materials::LightBlood };
			} case entity_e::AnimatedSuit: {
				return runes_t{ string, colors::metals::Tin };
			} case entity_e::Bonespur: {
				return runes_t{ string, colors::Marble };
			} case entity_e::Wraith: {
				return runes_t{ string, colors::light::Green };
			} case entity_e::FleshGolem: {
				return runes_t{ string, colors::materials::DarkBlood };
			} case entity_e::DeathKnight: {
				return runes_t{ string, colors::metals::Iron };
			} case entity_e::Adventurer: {
				return runes_t{ string, colors::metals::Bronze };
			} case entity_e::Mercenary: {
				return runes_t{ string, colors::metals::Brass };
			} case entity_e::Paladin: {
				return runes_t{ string, colors::metals::Steel };
			} case entity_e::Priest: {
				return runes_t{ string, colors::metals::Gold };
			} default: {
				return runes_t{ string, colors::White };
			}
		}
	}

	constexpr usize EntityTypeCount{ static_cast<usize>(entity_e::Priest) + 1 };

	using entity_group_t = u16;

	enum struct entity_group_e : entity_group_t {
		None = 0,
		Player = 1 << 0,
		Skeleton = Player << 1,
		Cultist = Skeleton << 1,
		Bloodhound = Cultist << 1,
		AnimatedSuit = Bloodhound << 1,
		Bonespur = AnimatedSuit << 1,
		Wraith = Bonespur << 1,
		FleshGolem = Wraith << 1,
		DeathKnight = FleshGolem << 1,
		Adventurer = DeathKnight << 1,
		Mercenary = Adventurer << 1,
		Paladin = Mercenary << 1,
		Priest = Paladin << 1
	};

	constexpr entity_group_e operator+(entity_group_e lhs, entity_group_e rhs) noexcept { return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) | static_cast<entity_group_t>(rhs)); }

	constexpr ref<entity_group_e> operator+=(ref<entity_group_e> lhs, entity_group_e rhs) noexcept { return lhs = lhs + rhs, lhs; }

	constexpr entity_group_e operator-(entity_group_e lhs, entity_group_e rhs) noexcept { return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) & (~static_cast<entity_group_t>(rhs))); }

	constexpr ref<entity_group_e> operator-=(ref<entity_group_e> lhs, entity_group_e rhs) noexcept { return lhs = lhs - rhs, lhs; }

	template<entity_e EntityType> struct to_entity_group {
		static constexpr entity_group_e value{ entity_group_e::None };
	};

	constexpr bool operator==(entity_group_e lhs, entity_e rhs) noexcept;

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator==(entity_group_e lhs, EntityTypes... rhs) noexcept;

	constexpr bool operator!=(entity_group_e lhs, entity_e rhs) noexcept;

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator!=(entity_group_e lhs, EntityTypes... rhs) noexcept;

	static constexpr std::string to_string(entity_group_e group) noexcept;

	static constexpr runes_t to_colored_string(entity_group_e group) noexcept;

	template<typename T> struct is_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_entity_v = is_entity<T>::value;

	template<typename T> concept Entity = is_entity<T>::value;

	template<Entity T> struct to_entity_enum {
		static constexpr entity_e value = entity_e::None;
	};

	template<Entity T> constexpr entity_e to_entity_enum_v = to_entity_enum<T>::value;

	template<> struct is_entity<std::nullptr_t> {
		static constexpr bool value = true;
	};
	
	template<entity_e EntityType> struct to_entity_type;
	
	template<> struct to_entity_type<entity_e::None> {
		using type = std::nullptr_t;
	};

	template<entity_e EntityType> using to_entity_type_t = typename to_entity_type<EntityType>::type;

	template<typename T> struct is_null_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_null_entity_v = is_null_entity<T>::value;

	template<typename T> concept NullEntity = Entity<T> && is_null_entity<T>::value;

	template<typename T> concept NonNullEntity = Entity<T> && !is_null_entity<T>::value;

	template<> struct is_null_entity<std::nullptr_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_good_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_good_entity_v = is_good_entity<T>::value;

	template<typename T> concept GoodEntity = NonNullEntity<T> && is_good_entity<T>::value;

	template<typename T> struct is_evil_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_evil_entity_v = is_evil_entity<T>::value;

	template<typename T> concept EvilEntity = NonNullEntity<T> && is_evil_entity<T>::value;

	template<typename T> struct is_non_player_entity {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_non_player_v = is_non_player_entity<T>::value;

	template<typename T> concept NonPlayerEntity = NonNullEntity<T> && is_non_player_entity<T>::value;

	template<typename T> concept NPCEntity = NonNullEntity<T> && NonPlayerEntity<T>;

	template<typename T> struct is_npc_entity {
		static constexpr bool value = NPCEntity<T>;
	};

	template<typename T> constexpr bool is_npc_entity_v = is_npc_entity<T>::value;

	template<typename T> struct is_player {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_player_v = is_player<T>::value;

	template<typename T> concept PlayerEntity = NonNullEntity<T> && is_player<T>::value;

	template<typename T> struct is_combatant {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_combatant_v = is_combatant<T>::value;

	template<typename T> concept CombatantEntity = NonNullEntity<T> && is_combatant<T>::value;

	template<typename T> struct is_docile {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_docile_v = is_docile<T>::value;

	template<typename T> concept DocileEntity = NonNullEntity<T> && is_docile<T>::value;

	template<typename T> struct is_fodder {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_fodder_v = is_fodder<T>::value;

	template<typename T> concept FodderEntity = NonNullEntity<T> && is_fodder<T>::value;

	template<typename T> struct is_clumsy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_clumsy_v = is_clumsy<T>::value;

	template<typename T> concept ClumsyEntity = NonNullEntity<T> && is_clumsy<T>::value;

	template<typename T> struct is_fast {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_fast_v = is_fast<T>::value;

	template<typename T> concept FastEntity = NonNullEntity<T> && is_fast<T>::value;

	template<typename T> struct is_bleeder {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_bleeder_v = is_fodder<T>::value;

	template<typename T> concept BleederEntity = NonNullEntity<T> && is_bleeder<T>::value;

	template<typename T> struct fluid_type {
		static constexpr fluid_type_e type = fluid_type_e::None;
	};

	template<typename T> constexpr fluid_type_e fluid_type_v = fluid_type<T>::type;

	template<typename T, entity_e EntityType> struct is_entity_type {
		static constexpr bool value = false;
	};

	template<> struct is_entity_type<std::nullptr_t, entity_e::None> {
		static constexpr bool value = true;
	};

	template<typename T, entity_e EntityType> constexpr bool is_entity_type_v = is_entity_type<T, EntityType>::value;

	template<Entity EntityType> inline constexpr glyph_t entity_glyphs;

	template<NonNullEntity EntityType> static inline constexpr bool has_death_sound{ false };

	template<NonNullEntity EntityType> static inline constexpr usize num_death_sounds{ 0 };

	template<NonNullEntity EntityType> static inline clip_pool_t<num_death_sounds<EntityType>> death_sounds;

	template<> inline constexpr glyph_t entity_glyphs<std::nullptr_t>{ 0x40, colors::White };
} // namespace necrowarp
