#pragma once

#include <bleak.hpp>

#include <cstddef>

#include <necrowarp/cell.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t;

	struct skeleton_t;
	struct cultist_t;
	struct bloodhound_t;
	struct wraith_t;
	struct flesh_golem_t;

	struct adventurer_t;
	struct mercenary_t;
	struct paladin_t;
	struct priest_t;

	struct skull_t;
	struct ladder_t;

	#define ALL_EVIL_NPCS \
		skeleton_t, \
		cultist_t, \
		bloodhound_t, \
		wraith_t, \
		flesh_golem_t
	
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

	#define ALL_NPCS \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS

	#define ALL_ANIMATE \
		player_t, \
		ALL_NPCS

	#define ALL_INANIMATE \
		skull_t, \
		ladder_t

	#define ALL_NON_PLAYER \
		ALL_NPCS, \
		ALL_INANIMATE

	#define ALL_ENTITIES \
		ALL_ANIMATE, \
		ALL_INANIMATE

	enum struct entity_e : u8 {
		None = 0,
		Player,
		Skeleton,
		Cultist,
		Bloodhound,
		Wraith,
		FleshGolem,
		Adventurer,
		Mercenary,
		Paladin,
		Priest,
		Skull,
		Ladder
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
			} case entity_e::Wraith: {
				return "wraith";
			} case entity_e::FleshGolem: {
				return "flesh golem";
			} case entity_e::Adventurer: {
				return "adventurer";
			} case entity_e::Mercenary: {
				return "mercenary";
			} case entity_e::Paladin: {
				return "paladin";
			} case entity_e::Priest: {
				return "priest";
			} case entity_e::Skull: {
				return "skull";
			} case entity_e::Ladder: {
				return "ladder";
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
			} case entity_e::Wraith: {
				return runes_t{ string, colors::light::Green };
			} case entity_e::FleshGolem: {
				return runes_t{ string, colors::materials::DarkBlood };
			} case entity_e::Adventurer: {
				return runes_t{ string, colors::metals::Bronze };
			} case entity_e::Mercenary: {
				return runes_t{ string, colors::metals::Brass };
			} case entity_e::Paladin: {
				return runes_t{ string, colors::metals::Steel };
			} case entity_e::Priest: {
				return runes_t{ string, colors::metals::Gold };
			} case entity_e::Skull: {
				return runes_t{ string, colors::White };
			} case entity_e::Ladder: {
				return runes_t{ string, colors::materials::Oak };
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
		Wraith = Bloodhound << 1,
		FleshGolem = Wraith << 1,
		Adventurer = FleshGolem << 1,
		Mercenary = Adventurer << 1,
		Paladin = Mercenary << 1,
		Priest = Paladin << 1,
		Skull = Priest << 1,
		Ladder = Skull << 1
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

	enum class decay_e : i8 {
		Rotted = -1,
		Animate = 0,
		Fresh = 1,
	};

	constexpr decay_e decay(decay_e state) noexcept {
		switch (state) {
			case decay_e::Fresh: {
				return decay_e::Animate;
			} case decay_e::Animate: {
				return decay_e::Rotted;
			} case decay_e::Rotted: {
				return decay_e::Rotted;
			}
		}
	}

	constexpr cstr to_string(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return "rotted";
			} case decay_e::Animate: {
				return "animate";
			} case decay_e::Fresh: {
				return "fresh";
			}
		}
	}

	constexpr runes_t to_colored_string(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return runes_t{ to_string(decay_e::Rotted), colors::light::Red };
			} case decay_e::Animate: {
				return runes_t{ to_string(decay_e::Animate), colors::light::Magenta };
			} case decay_e::Fresh: {
				return runes_t{ to_string(decay_e::Fresh), colors::light::Green };
			}
		}
	}

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

	template<typename T> struct is_animate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_animate_v = is_animate<T>::value;

	template<typename T> concept AnimateEntity = NonNullEntity<T> && is_animate<T>::value;

	template<typename T> struct is_inanimate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_inanimate_v = is_inanimate<T>::value;

	template<typename T> concept InanimateEntity = NonNullEntity<T> && is_inanimate<T>::value;

	template<typename T> struct is_non_player_entity {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_non_player_v = is_non_player_entity<T>::value;

	template<typename T> concept NonPlayerEntity = NonNullEntity<T> && is_non_player_entity<T>::value;

	template<typename T> concept NPCEntity = NonNullEntity<T> && AnimateEntity<T> && NonPlayerEntity<T>;

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

	template<AnimateEntity EntityType> static inline constexpr bool has_death_sound{ false };

	template<AnimateEntity EntityType> static inline constexpr usize num_death_sounds{ 0 };

	template<AnimateEntity EntityType> static inline clip_pool_t<num_death_sounds<EntityType>> death_sounds;

	template<> inline constexpr glyph_t entity_glyphs<std::nullptr_t>{ 0x40, colors::White };
} // namespace necrowarp
