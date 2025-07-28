#pragma once

#include <bleak.hpp>

#include <cstddef>

#include <necrowarp/constants/enums.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t;

	// lesser minions

	// calcitic domain (bones): a lesser minion that loses potency as it decays
	struct skeleton_t;
	// spectral domain (ichor): a lesser minion that will drown the map in ichor
	struct cultist_t;
	// sanguine domain (blood): a lesser minion that harries enemies with its speed
	struct bloodhound_t;
	// galvanic domain (metal): a lesser minion that gains potency as it galvanises
	struct animated_suit_t;
	// ravenous domain (flesh): a lesser minion that may replicate itself
	struct abomination_t;
	// wretched domain (filth): a lesser minion that grapples enemies
	struct draugaz_t;
	// cerebral domain (cerebra): a lesser minion that explodes on death
 // struct furtive_horror_t;
	// infernal domain (ectoplasm): a lesser minion that spreads ectoplasm
 // struct chromalese_t;

	// exalted minions

	// calcitic domain (bones): an exalted minion that reflects all damage while taking hardly any
	struct bonespur_t;
	// spectral domain (ichor): an exalted minion that strikes all enemies around it
	struct wraith_t;
	// sanguine domain (blood): an exalted minion that consumes the blood of enemies it attacks and kills
	struct hemogheist_t;
	// galvanic domain (metal): an exalted minion that gains potency as it galvanises
	struct death_knight_t;
	// ravenous domain (flesh): an exalted minion that consumes enemies with less health
	struct flesh_golem_t;
	// wretched domain (filth): an exalted minion that spreads filth to vicitms
	struct dreadwurm_t;
	// cerebral domain (cerebra): a lesser minion that controls the mind of its host
 // struct hamaz_t;
	// infernal domain (ectoplasm): an exalted minion that kills instantaneously
 // struct isoscol_t;

	// do-gooders

	// a common adversary that provides the basic catalysts
	struct adventurer_t;

	// a common adversary that provides metal
	struct mercenary_t;

	// a specialist adversary that attacks from range
	struct ranger_t;

	// a specialist adversary that traverses the interstice to avoid detection
	struct skulker_t;

	// a support adversary that soothes their comrades' addled minds
	struct mist_lady_t;

	// a support adversary that inspires their comrades to fight harder
	struct banner_bearer_t;

	// a powerful adversary that is undefeatable in decisive battle
	struct battle_monk_t;

	// a powerful adversary that is inextinguishable amidst the heat of battle
	struct berserker_t;

	// a dangerous adversary that crushes lesser minions without effort
	struct paladin_t;

	// neutral creatures

	// a common adversary that grows off of the flesh the dead
	struct thetwo_t;

	#define ALL_EVIL_NPCS \
		skeleton_t, \
		cultist_t, \
		bloodhound_t, \
		animated_suit_t, \
		abomination_t, \
		draugaz_t, \
		bonespur_t, \
		wraith_t, \
		hemogheist_t, \
		death_knight_t, \
		flesh_golem_t, \
		dreadwurm_t
	
	#define ALL_EVIL \
		player_t, \
		ALL_EVIL_NPCS

	#define ALL_GOOD_NPCS \
		adventurer_t, \
		mercenary_t, \
		ranger_t, \
		skulker_t, \
		mist_lady_t, \
		banner_bearer_t, \
		battle_monk_t, \
		berserker_t, \
		paladin_t
	
	#define ALL_GOOD \
		ALL_GOOD_NPCS

	#define ALL_NEUTRAL_NPCS \
		thetwo_t

	#define ALL_NEUTRAL \
		ALL_NEUTRAL_NPCS

	#define ALL_NON_EVIL_NPCS \
		ALL_GOOD_NPCS, \
		ALL_NEUTRAL_NPCS

	#define ALL_NON_EVIL \
		ALL_GOOD, \
		ALL_NEUTRAL

	#define ALL_NON_GOOD_NPCS \
		ALL_EVIL_NPCS, \
		ALL_NEUTRAL_NPCS

	#define ALL_NON_GOOD \
		ALL_EVIL, \
		ALL_NEUTRAL

	#define ALL_NON_NEUTRAL_NPCS \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS

	#define ALL_NON_NEUTRAL \
		ALL_EVIL, \
		ALL_GOOD

	#define ALL_NON_PLAYER \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS, \
		ALL_NEUTRAL_NPCS

	#define ALL_ANIMATED_ENTITIES \
		animated_suit_t, \
		hemogheist_t, \
		death_knight_t
	
	#define ALL_VIGILANT_ENTITIES \
		player_t, \
		cultist_t, \
		bloodhound_t, \
		wraith_t, \
		hemogheist_t, \
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
		Abomination,
		Draugaz,
	 // Hamaz,
	 // Chromalese,

		Bonespur,
		Wraith,
		Hemogheist,
		DeathKnight,
		FleshGolem,
		Dreadwurm,
	 // FurtiveHorror,
	 // Isoscol,

		Adventurer,
		Mercenary,
		Ranger,
		Skulker,
		MistLady,
		BannerBearer,
		BattleMonk,
		Berserker,
		Paladin,
		
		Thetwo,
	};

	template<plurality_e Plurality = plurality_e::Singular> static constexpr cstr to_string(entity_e type) noexcept;

	template<> constexpr cstr to_string<plurality_e::Singular>(entity_e type) noexcept {
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
			} case entity_e::Abomination: {
				return "abomination";
			} case entity_e::Draugaz: {
				return "draugaz";
			}/* case entity_e::Hamaz: {
				return "hamaz";
			} case entity_e::Chromalese: {
				return "chromalese";
			}*/ case entity_e::Bonespur: {
				return "bonespur";
			} case entity_e::Wraith: {
				return "wraith";
			} case entity_e::Hemogheist: {
				return "hemogheist";
			} case entity_e::DeathKnight: {
				return "death knight";
			} case entity_e::FleshGolem: {
				return "flesh golem";
			} case entity_e::Dreadwurm: {
				return "dreadwurm";
			}/* case entity_e::FurtiveHorror: {
				return "furtive horror";
			} case entity_e::Isoscol: {
				return "isoscol";
			}*/ case entity_e::Adventurer: {
				return "adventurer";
			} case entity_e::Mercenary: {
				return "mercenary";
			} case entity_e::Ranger: {
				return "ranger";
			} case entity_e::Skulker: {
				return "skulker";
			} case entity_e::MistLady: {
				return "lady of the mist";
			} case entity_e::BannerBearer: {
				return "banner bearer";
			} case entity_e::BattleMonk: {
				return "battle monk";
			} case entity_e::Berserker: {
				return "berserker";
			} case entity_e::Paladin: {
				return "paladin";
			} case entity_e::Thetwo: {
				return "thetwo";
			}
		}
	}

	template<> constexpr cstr to_string<plurality_e::Multiple>(entity_e type) noexcept {
		switch (type) {
			case entity_e::None: {
				return "none";
			} case entity_e::Player: {
				return "players";
			} case entity_e::Skeleton: {
				return "skeletons";
			} case entity_e::Cultist: {
				return "cultists";
			} case entity_e::Bloodhound: {
				return "bloodhounds";
			} case entity_e::AnimatedSuit: {
				return "animated suits of armor";
			} case entity_e::Abomination: {
				return "abominations";
			} case entity_e::Draugaz: {
				return "draugr";
			}/* case entity_e::Hamaz: {
				return "hamr";
			} case entity_e::Chromalese: {
				return "chromalesia";
			}*/ case entity_e::Bonespur: {
				return "bonespurs";
			} case entity_e::Wraith: {
				return "wraithes";
			} case entity_e::Hemogheist: {
				return "hemogheist";
			} case entity_e::DeathKnight: {
				return "death knights";
			} case entity_e::FleshGolem: {
				return "flesh golems";
			} case entity_e::Dreadwurm: {
				return "dreadwurms";
			}/* case entity_e::FurtiveHorror: {
				return "furtive horrors";
			} case entity_e::Isoscol: {
				return "isoscoline";
			}*/ case entity_e::Adventurer: {
				return "adventurers";
			} case entity_e::Mercenary: {
				return "mercenaries";
			} case entity_e::Ranger: {
				return "rangers";
			} case entity_e::Skulker: {
				return "skulkers";
			} case entity_e::MistLady: {
				return "ladies of the mist";
			} case entity_e::BannerBearer: {
				return "banner bearers";
			} case entity_e::BattleMonk: {
				return "battle monks";
			} case entity_e::Berserker: {
				return "berserkers";
			} case entity_e::Paladin: {
				return "paladins";
			} case entity_e::Thetwo: {
				return "thetwo";
			}
		}
	}

	static constexpr color_t to_color(entity_e type) noexcept {
		switch (type) {
			case entity_e::None: {
				return colors::Grey;
			} case entity_e::Player: {
				return colors::Magenta;
			} case entity_e::Skeleton: {
				return colors::White;
			} case entity_e::Cultist: {
				return colors::dark::Magenta;
			} case entity_e::Bloodhound: {
				return colors::materials::light::Fluids<fluid_e::Blood>;
			} case entity_e::AnimatedSuit: {
				return colors::metals::Tin;
			} case entity_e::Abomination: {
				return colors::dark::Orange;
			} case entity_e::Draugaz: {
				return mix(colors::Orange, colors::Green);
			}/* case entity_e::Hamaz: {
				return colors::materials::light::Ectoplasm;
			} case entity_e::Chromalese: {
				return colors::materials::dark::Ectoplasm;
			}*/ case entity_e::Bonespur: {
				return colors::Marble;
			} case entity_e::Wraith: {
				return colors::light::Green;
			} case entity_e::Hemogheist: {
				return colors::materials::dark::Fluids<fluid_e::Blood>;
			} case entity_e::DeathKnight: {
				return colors::metals::Iron;
			} case entity_e::FleshGolem: {
				return mix(colors::Orange, colors::materials::Fluids<fluid_e::Blood>);
			} case entity_e::Dreadwurm: {
				return mix(colors::Orange, colors::light::Grey);
			}/* case entity_e::FurtiveHorror: {
				return mix(colors::Blue, colors::light::Grey);
			} case entity_e::Isoscol: {
				return mix(colors::Green, colors::light::Grey);
			}*/ case entity_e::Adventurer: {
				return colors::metals::Bronze;
			} case entity_e::Mercenary: {
				return colors::dark::Yellow;
			} case entity_e::Ranger: {
				return colors::dark::Green;
			} case entity_e::Skulker: {
				return colors::dark::Grey;
			} case entity_e::MistLady: {
				return colors::dark::Green;
			} case entity_e::BannerBearer: {
				return colors::dark::Grey;
			} case entity_e::BattleMonk: {
				return colors::light::Yellow;
			} case entity_e::Berserker: {
				return colors::dark::Orange;
			} case entity_e::Paladin: {
				return colors::metals::Steel;
			} case entity_e::Thetwo: {
				return colors::metals::Brass;
			}
		}
	}

	template<plurality_e Plurality = plurality_e::Singular> static constexpr runes_t to_colored_string(entity_e type) noexcept {
		return magic_enum::enum_switch(
			[](auto val) -> runes_t {
				constexpr entity_e cval{ val };

				return runes_t{ to_string<Plurality>(cval), to_color(cval) };
			}, type
		);
	}

	constexpr usize EntityTypeCount{ static_cast<usize>(entity_e::Thetwo) };

	constexpr usize NPCSizeCap{ 8 };

	using entity_group_t = u32;

	enum struct entity_group_e : entity_group_t {
		None = 0,

		Player = 1 << 0,

		Skeleton = Player << 1,
		Cultist = Skeleton << 1,
		Bloodhound = Cultist << 1,
		AnimatedSuit = Bloodhound << 1,
		Abomination = AnimatedSuit << 1,
		Draugaz = Abomination << 1,
	 // Hamaz = Draugaz << 1,
	 // Chromalese = Hamaz << 1,

		Bonespur = Draugaz << 1, // Chromalese << 1,
		Wraith = Bonespur << 1,
		Hemogheist = Wraith << 1,
		DeathKnight = Hemogheist << 1,
		FleshGolem = DeathKnight << 1,
		Dreadwurm = FleshGolem << 1,
	 // FurtiveHorror = Dreadwurm << 1,
	 // Isoscol = FurtiveHorror << 1,

		Adventurer = Dreadwurm << 1, // Isoscol << 1,
		Mercenary = Adventurer << 1,
		Ranger = Mercenary << 1,
		Skulker = Ranger << 1,
		MistLady = Skulker << 1,
		BannerBearer = MistLady << 1,
		BattleMonk = BannerBearer << 1,
		Berserker = BattleMonk << 1,
		Paladin = Berserker << 1,

		Thetwo = Paladin << 1,
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

	constexpr entity_e at(entity_group_e group, usize index) noexcept;

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

	template<typename T> struct is_evil {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_evil_v = is_evil<T>::value;

	template<typename T> concept EvilEntity = NonNullEntity<T> && is_evil<T>::value;

	template<typename T> struct is_good {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_good_v = is_good<T>::value;

	template<typename T> concept GoodEntity = NonNullEntity<T> && is_good<T>::value;

	template<typename T> struct is_neutral {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_neutral_v = is_neutral<T>::value;

	template<typename T> concept NeutralEntity = NonNullEntity<T> && is_neutral<T>::value;
	
	template<typename T> struct is_non_evil {
		static constexpr bool value = !is_evil<T>::value && (is_good<T>::value || is_neutral<T>::value);
	};

	template<typename T> constexpr bool is_non_evil_v = is_non_evil<T>::value;

	template<typename T> concept NonEvilEntity = NonNullEntity<T> && is_non_evil<T>::value;
	
	template<typename T> struct is_non_good {
		static constexpr bool value = !is_good<T>::value && (is_evil<T>::value || is_neutral<T>::value);
	};

	template<typename T> constexpr bool is_non_good_v = is_non_good<T>::value;

	template<typename T> concept NonGoodEntity = NonNullEntity<T> && is_non_good<T>::value;
	
	template<typename T> struct is_non_neutral {
		static constexpr bool value = !is_neutral<T>::value && (is_evil<T>::value || is_good<T>::value);
	};

	template<typename T> constexpr bool is_non_neutral_v = is_non_neutral<T>::value;

	template<typename T> concept NonNeutralEntity = NonNullEntity<T> && is_non_neutral<T>::value;

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

	template<typename T> struct is_holy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_holy_v = is_holy<T>::value;

	template<typename T> concept HolyEntity = NonNullEntity<T> && is_holy<T>::value;

	template<typename T> struct is_unholy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_unholy_v = is_unholy<T>::value;

	template<typename T> concept UnholyEntity = NonNullEntity<T> && is_unholy<T>::value;

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

	template<typename T> struct is_volumetric {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_volumetric_v = is_volumetric<T>::value;

	template<typename T> concept VolumetricEntity = NonNullEntity<T> && is_volumetric<T>::value;

	template<typename T> struct is_bleeder {
		static constexpr bool value = false;
		static constexpr fluid_e type = fluid_e::None;

		static constexpr bool conditional = false;
	};

	template<typename T> constexpr bool is_bleeder_v = is_bleeder<T>::value && is_bleeder<T>::type != fluid_e::None;

	template<typename T> concept BleederEntity = NonNullEntity<T> && is_bleeder_v<T>;

	template<typename T> struct is_thirsty {
		static constexpr bool value = false;
		static constexpr fluid_e type = fluid_e::None;
	};

	template<typename T> constexpr bool is_thirsty_v = is_thirsty<T>::value && is_thirsty<T>::type != fluid_e::None;

	template<typename T> concept ThirstyEntity = NonNullEntity<T> && is_thirsty_v<T>;

	template<typename T, typename U> constexpr bool is_thirsty_for = is_thirsty_v<T> && is_bleeder_v<U> && contains(is_thirsty<T>::type, is_bleeder<U>::type);

	template<ThirstyEntity Thirster, BleederEntity Bleeder> constexpr fluid_e thirsting_for = shares(is_thirsty<Thirster>::type, is_bleeder<Bleeder>::type);

	template<typename T> struct is_devourable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_devourable_v = is_devourable<T>::value;

	template<typename T> concept DevourableEntity = NonNullEntity<T> && is_devourable<T>::value;

	template<typename T> struct is_incorporeal {
		static constexpr bool value = false;
		static constexpr bool conditional = false;
	};

	template<typename T> constexpr bool is_incorporeal_v = is_incorporeal<T>::value;

	template<typename T> concept IncorporealEntity = NonNullEntity<T> && is_bleeder_v<T>;

	template<typename T> struct is_afflictable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_afflictable_v = is_afflictable<T>::value;

	template<typename T> concept AfflicatableEntity = NonNullEntity<T> && is_afflictable<T>::value;

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

	template<NonNullEntity EntityType> struct entity_t {
		offset_t position;
		ptr<EntityType> entity;

		template<region_e Region> inline bool is_valid() const noexcept;

		inline bool is_newborn() const noexcept;

		inline bool is_deceased() const noexcept;

		inline bool is_concussed() const noexcept;

		inline bool is_afflicted() const noexcept;

		inline std::optional<affliction_e> get_affliction() const noexcept;

		template<affliction_e Affliction> inline bool has_affliction() const noexcept;
	};

	template<death_e Death> struct death_info_t{
		const bool perished;

		constexpr death_info_t(bool perished) : perished{ perished } {}
	};

	template<> struct death_info_t<death_e::Devoured>{
		const bool devoured;
		const i8 protein;

		constexpr death_info_t(bool devoured) : devoured{ devoured }, protein{ 0 } {}
		constexpr death_info_t(bool devoured, i8 protein) : devoured{ devoured }, protein{ protein } {}
	};

	namespace globals {
		template<entity_e Entity>
			requires (Entity != entity_e::None && is_non_evil<typename to_entity_type<Entity>::type>::value)
		constexpr bool OopsAll{ false };
	}
} // namespace necrowarp
