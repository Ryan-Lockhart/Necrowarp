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

	// a lesser minion that loses offensive potency as it decays
	struct skeleton_t;
	// a lesser minion that will drown the map in ichor
	struct cultist_t;
	// a lesser minion that harries enemies with its speed
	struct bloodhound_t;
	// a lesser minion that gains defensive potency as it galvanises
	struct animated_suit_t;

	// exalted minions

	// an exalted minion that reflects all damage while taking hardly any
	struct bonespur_t;
	// an exalted minion that strikes (fear into) all enemies around it
	struct wraith_t;
	// an exalted minion that consumes enemies with less health
	struct flesh_golem_t;
	// an exalted minion that gains defensive potency as it galvanises
	struct death_knight_t;

	// do-gooders

	// a common adversary that provides the basic catalysts
	struct adventurer_t;

	// a common adversary that provides metal
	struct mercenary_t;

	// a common adversary that grows off of the flesh of its comrades
	struct thetwo_t;

	// a specialist adversary that attacks from range
	struct ranger_t;

	// a specialist adversary that traverses the interstice to avoid detection
	struct skulker_t;

	// a powerful adversary that is undefeatable in decisive battle
	struct battle_monk_t;

	// a powerful adversary that is inextinguishable amidst the heat of battle
	struct berserker_t;

	// a dangerous adversary that crushes lesser minions without effort
	struct paladin_t;

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
		thetwo_t, \
		ranger_t, \
		skulker_t, \
		battle_monk_t, \
		paladin_t, \
		berserker_t
	
	#define ALL_GOOD \
		ALL_GOOD_NPCS

	#define ALL_NON_PLAYER \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS

	#define ALL_ANIMATED_ENTITIES \
		animated_suit_t, \
		death_knight_t
	
	#define ALL_VIGILANT_ENTITIES \
		player_t, \
		cultist_t, \
		bloodhound_t, \
		wraith_t, \
		death_knight_t

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
		Thetwo,
		Ranger,
		Skulker,
		BattleMonk,
		Paladin,
		Berserker,
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
			} case entity_e::Thetwo: {
				return "thetwo";
			} case entity_e::Ranger: {
				return "ranger";
			} case entity_e::Skulker: {
				return "skulker";
			} case entity_e::BattleMonk: {
				return "battle monk";
			} case entity_e::Paladin: {
				return "paladin";
			} case entity_e::Berserker: {
				return "berserker";
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
				return runes_t{ string, colors::materials::light::Blood };
			} case entity_e::AnimatedSuit: {
				return runes_t{ string, colors::metals::Tin };
			} case entity_e::Bonespur: {
				return runes_t{ string, colors::Marble };
			} case entity_e::Wraith: {
				return runes_t{ string, colors::light::Green };
			} case entity_e::FleshGolem: {
				return runes_t{ string, colors::materials::dark::Blood };
			} case entity_e::DeathKnight: {
				return runes_t{ string, colors::metals::Iron };
			} case entity_e::Adventurer: {
				return runes_t{ string, colors::metals::Bronze };
			} case entity_e::Mercenary: {
				return runes_t{ string, colors::dark::Yellow };
			} case entity_e::Thetwo: {
				return runes_t{ string, colors::metals::Brass };
			} case entity_e::Ranger: {
				return runes_t{ string, colors::dark::Green };
			} case entity_e::Skulker: {
				return runes_t{ string, colors::dark::Grey };
			} case entity_e::BattleMonk: {
				return runes_t{ string, colors::light::Yellow };
			} case entity_e::Paladin: {
				return runes_t{ string, colors::metals::Steel };
			} case entity_e::Berserker: {
				return runes_t{ string, colors::dark::Orange };
			}
		}
	}

	constexpr usize EntityTypeCount{ static_cast<usize>(entity_e::Berserker) + 1 };

	constexpr usize NPCSizeCap{ 16 };

	using entity_group_t = u32;

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
		Thetwo = Mercenary << 1,
		Ranger = Thetwo << 1,
		Skulker = Ranger << 1,
		BattleMonk = Skulker << 1,
		Paladin = BattleMonk << 1,
		Berserker = Paladin << 1,
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

	template<typename T> concept AnimatedEntity = NonNullEntity<T> && globals::has_animation<T>::value;

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

	template<typename T> struct is_elusive {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_elusive_v = is_elusive<T>::value;

	template<typename T> concept ElusiveEntity = NonNullEntity<T> && is_elusive<T>::value;

	template<typename T> struct is_fast {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_fast_v = is_fast<T>::value;

	template<typename T> concept FastEntity = NonNullEntity<T> && is_fast<T>::value;

	template<typename T> struct is_serene {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_serene_v = is_serene<T>::value;

	template<typename T> concept SereneEntity = NonNullEntity<T> && is_serene<T>::value;

	template<typename T> struct is_berker {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_berker_v = is_berker<T>::value;

	template<typename T> concept BerkerEntity = NonNullEntity<T> && is_berker<T>::value;

	template<typename T> struct is_armored {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_armored_v = is_armored<T>::value;

	template<typename T> concept ArmoredEntity = NonNullEntity<T> && is_armored<T>::value;

	template<typename T> struct is_cleaver {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_cleaver_v = is_cleaver<T>::value;

	template<typename T> concept CleaverEntity = NonNullEntity<T> && is_cleaver<T>::value;

	template<typename T> struct is_sneaky {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_sneaky_v = is_sneaky<T>::value;

	template<typename T> concept SneakyEntity = NonNullEntity<T> && is_sneaky<T>::value;

	template<typename T> struct is_vigilant {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_vigilant_v = is_vigilant<T>::value;

	template<typename T> concept VigilantEntity = NonNullEntity<T> && is_vigilant<T>::value;

	template<typename T> struct is_bleeder {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_bleeder_v = is_bleeder<T>::value;

	template<typename T> concept BleederEntity = NonNullEntity<T> && is_bleeder<T>::value;

	template<typename T> struct fluid_type {
		static constexpr fluid_e type = fluid_e::None;
	};

	template<typename T> constexpr fluid_e fluid_type_v = fluid_type<T>::type;

	template<typename T> struct is_devourable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_devourable_v = is_devourable<T>::value;

	template<typename T> concept DevourableEntity = NonNullEntity<T> && is_devourable<T>::value;

	template<typename T> struct is_concussable {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_concussable_v = is_concussable<T>::value;

	template<typename T> concept ConcussableEntity = NonNullEntity<T> && is_concussable<T>::value;

	template<typename T> struct is_spatterable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_spatterable_v = is_spatterable<T>::value;

	template<typename T> concept SpatterableEntity = NonNullEntity<T> && is_spatterable<T>::value;

	template<typename T> struct is_updateable {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_updateable_v = is_updateable<T>::value;

	template<typename T> concept UpdateableEntity = NonNullEntity<T> && is_updateable<T>::value;

	template<typename T, entity_e EntityType> struct is_entity_type {
		static constexpr bool value = false;
	};

	template<> struct is_entity_type<std::nullptr_t, entity_e::None> {
		static constexpr bool value = true;
	};

	template<typename T, entity_e EntityType> constexpr bool is_entity_type_v = is_entity_type<T, EntityType>::value;

	template<NonNullEntity EntityType>
		requires (!globals::has_animation<EntityType>::value && !globals::has_variants<EntityType>::value)
	inline constexpr glyph_t entity_glyphs;

	template<NonNullEntity EntityType> static inline constexpr bool has_death_sound{ false };

	template<NonNullEntity EntityType> static inline constexpr usize num_death_sounds{ 0 };

	template<NonNullEntity EntityType> static inline clip_pool_t<num_death_sounds<EntityType>> death_sounds;
} // namespace necrowarp
