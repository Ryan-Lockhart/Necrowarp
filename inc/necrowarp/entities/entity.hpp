#pragma once

#include <bleak.hpp>

#include <necrowarp/constants/enums.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t;

	// lesser minions

	// calcitic domain (bones): a lesser minion that loses potency as it decays
	struct skeleton_t;
	// spectral domain (ichor): a lesser minion that will leash to the player
	struct cultist_t;
	// sanguine domain (blood): a lesser minion that harries enemies with its speed
	struct bloodhound_t;
	// galvanic domain (metal): a lesser minion that gains potency as it galvanises
	struct animated_suit_t;
	// ravenous domain (flesh): a lesser minion that may replicate itself
	struct abomination_t;
	// wretched domain (filth): a lesser minion that grapples enemies
	struct draugaz_t;
	// cerebral domain (cerebra): a lesser minion that projects the mind of its progenitor
	struct hamaz_t;
	// infernal domain (ectoplasm): a lesser minion that is difficult to hit and cannot be dodged
	struct chromalese_t;

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
	// wretched domain (filth): an exalted minion that spreads filth across the map
	struct dreadwurm_t;
	// cerebral domain (cerebra): an exalted minion that causes nearby enemies to fumble their attacks
	struct furtive_horror_t;
	// infernal domain (ectoplasm): an exalted minion that kills instantaneously and warps to the nearest enemy
	struct isoscel_t;

	// do-gooders

	// a common adversary that provides the basic catalysts
	struct adventurer_t;

	// a common adversary that provides metal
	struct mercenary_t;

	// a specialist adversary that attacks from range
	struct ranger_t;

	// a specialist adversary that traverses the interstice to avoid detection
	struct skulker_t;

	// a specialist adversary that blocks maligned energies
	struct mansling_t;

	// a support adversary that soothes their comrades' addled minds
	struct mist_lady_t;

	// a support adversary that inspires their comrades to fight harder
	struct banner_bearer_t;

	// a support adversary that restores their comrades broken bodies
	struct medicus_t;

	// a powerful adversary that is undefeatable in decisive battle
	struct battle_monk_t;

	// a powerful adversary that is inextinguishable amidst the heat of battle
	struct berserker_t;

	// a powerful adversary that is fueled by maligned energies
	struct hexeater_t;

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
		hamaz_t, \
		chromalese_t, \
		bonespur_t, \
		wraith_t, \
		hemogheist_t, \
		death_knight_t, \
		flesh_golem_t, \
		dreadwurm_t, \
		furtive_horror_t, \
		isoscel_t
	
	#define ALL_EVIL \
		player_t, \
		ALL_EVIL_NPCS

	#define ALL_GOOD_NPCS \
		adventurer_t, \
		mercenary_t, \
		ranger_t, \
		skulker_t, \
		mansling_t, \
		mist_lady_t, \
		banner_bearer_t, \
		medicus_t, \
		battle_monk_t, \
		berserker_t, \
		hexeater_t, \
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
		hamaz_t, \
		chromalese_t, \
		hemogheist_t, \
		death_knight_t, \
		furtive_horror_t, \
		isoscel_t, \
		mansling_t, \
		hexeater_t
	
	#define ALL_VIGILANT_ENTITIES \
		player_t, \
		cultist_t, \
		bloodhound_t, \
		chromalese_t, \
		wraith_t, \
		hemogheist_t, \
		death_knight_t, \
		isoscel_t

	#define ALL_ENTITIES \
		player_t, \
		ALL_NON_PLAYER

	enum struct entity_e : u8 {
		Player = 0,

		Skeleton,
		Cultist,
		Bloodhound,
		AnimatedSuit,
		Abomination,
		Draugaz,
		Hamaz,
		Chromalese,

		Bonespur,
		Wraith,
		Hemogheist,
		DeathKnight,
		FleshGolem,
		Dreadwurm,
		FurtiveHorror,
		Isoscel,

		Adventurer,
		Mercenary,
		Ranger,
		Skulker,
		Mansling,
		MistLady,
		BannerBearer,
		Medicus,
		BattleMonk,
		Berserker,
		Hexeater,
		Paladin,
		
		Thetwo,
	};

	template<plurality_e Plurality = plurality_e::Singular> static constexpr cstr to_string(entity_e type) noexcept;

	template<> constexpr cstr to_string<plurality_e::Singular>(entity_e type) noexcept {
		switch (type) {
			case entity_e::Player: {
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
			} case entity_e::Hamaz: {
				return "hamaz";
			} case entity_e::Chromalese: {
				return "chromalese";
			} case entity_e::Bonespur: {
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
			} case entity_e::FurtiveHorror: {
				return "furtive horror";
			} case entity_e::Isoscel: {
				return "isoscel";
			} case entity_e::Adventurer: {
				return "adventurer";
			} case entity_e::Mercenary: {
				return "mercenary";
			} case entity_e::Ranger: {
				return "ranger";
			} case entity_e::Skulker: {
				return "skulker";
			} case entity_e::Mansling: {
				return "mansling";
			} case entity_e::MistLady: {
				return "lady of the mist";
			} case entity_e::BannerBearer: {
				return "banner bearer";
			} case entity_e::Medicus: {
				return "medicus";
			} case entity_e::BattleMonk: {
				return "battle monk";
			} case entity_e::Berserker: {
				return "berserker";
			} case entity_e::Hexeater: {
				return "hexeater";
			} case entity_e::Paladin: {
				return "paladin";
			} case entity_e::Thetwo: {
				return "thetwo";
			}
		}
	}

	template<> constexpr cstr to_string<plurality_e::Multiple>(entity_e type) noexcept {
		switch (type) {
			case entity_e::Player: {
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
			} case entity_e::Hamaz: {
				return "hamr";
			} case entity_e::Chromalese: {
				return "chromalesia";
			} case entity_e::Bonespur: {
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
			} case entity_e::FurtiveHorror: {
				return "furtive horrors";
			} case entity_e::Isoscel: {
				return "isosceline";
			} case entity_e::Adventurer: {
				return "adventurers";
			} case entity_e::Mercenary: {
				return "mercenaries";
			} case entity_e::Ranger: {
				return "rangers";
			} case entity_e::Skulker: {
				return "skulkers";
			} case entity_e::Mansling: {
				return "manslings";
			} case entity_e::MistLady: {
				return "ladies of the mist";
			} case entity_e::BannerBearer: {
				return "banner bearers";
			} case entity_e::Medicus: {
				return "medicii";
			} case entity_e::BattleMonk: {
				return "battle monks";
			} case entity_e::Berserker: {
				return "berserkers";
			} case entity_e::Hexeater: {
				return "hexeaters";
			} case entity_e::Paladin: {
				return "paladins";
			} case entity_e::Thetwo: {
				return "thetwo";
			}
		}
	}

	static constexpr color_t to_color(entity_e type) noexcept {
		switch (type) {
			case entity_e::Player: {
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
			} case entity_e::Hamaz: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::materials::Fluids<fluid_e::Ichor>);
			} case entity_e::Chromalese: {
				return mix(colors::materials::Fluids<fluid_e::Ectoplasm>, colors::materials::Fluids<fluid_e::Blood>);
			} case entity_e::Bonespur: {
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
			} case entity_e::FurtiveHorror: {
				return mix(colors::Blue, colors::light::Grey);
			} case entity_e::Isoscel: {
				return mix(colors::Green, colors::light::Grey);
			} case entity_e::Adventurer: {
				return colors::metals::Bronze;
			} case entity_e::Mercenary: {
				return colors::dark::Yellow;
			} case entity_e::Ranger: {
				return colors::dark::Green;
			} case entity_e::Skulker: {
				return colors::dark::Grey;
			} case entity_e::Mansling: {
				return mix(colors::Grey, colors::Magenta);
			} case entity_e::MistLady: {
				return colors::dark::Green;
			} case entity_e::BannerBearer: {
				return colors::dark::Grey;
			} case entity_e::Medicus: {
				return mix(colors::Red, colors::White);
			} case entity_e::BattleMonk: {
				return colors::light::Yellow;
			} case entity_e::Berserker: {
				return colors::dark::Orange;
			} case entity_e::Hexeater: {
				return mix(colors::Orange, colors::Blue);
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

	template<typename T> struct is_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_entity_v = is_entity<T>::value;

	template<typename T> concept Entity = is_entity<T>::value;

	template<Entity T> struct to_entity_enum {
		static constexpr entity_e value{};
	};

	template<Entity T> constexpr entity_e to_entity_enum_v = to_entity_enum<T>::value;
	
	template<entity_e EntityType> struct to_entity_type;

	template<entity_e EntityType> using to_entity_type_t = typename to_entity_type<EntityType>::type;

	template<typename T> concept AnimatedEntity = Entity<T> && globals::has_animation<T>::value;

	template<typename T> struct is_evil {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_evil_v = is_evil<T>::value;

	template<typename T> concept EvilEntity = Entity<T> && is_evil<T>::value;

	template<typename T> struct is_good {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_good_v = is_good<T>::value;

	template<typename T> concept GoodEntity = Entity<T> && is_good<T>::value;

	template<typename T> struct is_neutral {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_neutral_v = is_neutral<T>::value;

	template<typename T> concept NeutralEntity = Entity<T> && is_neutral<T>::value;
	
	template<typename T> struct is_non_evil {
		static constexpr bool value = !is_evil<T>::value && (is_good<T>::value || is_neutral<T>::value);
	};

	template<typename T> constexpr bool is_non_evil_v = is_non_evil<T>::value;

	template<typename T> concept NonEvilEntity = Entity<T> && is_non_evil<T>::value;
	
	template<typename T> struct is_non_good {
		static constexpr bool value = !is_good<T>::value && (is_evil<T>::value || is_neutral<T>::value);
	};

	template<typename T> constexpr bool is_non_good_v = is_non_good<T>::value;

	template<typename T> concept NonGoodEntity = Entity<T> && is_non_good<T>::value;
	
	template<typename T> struct is_non_neutral {
		static constexpr bool value = !is_neutral<T>::value && (is_evil<T>::value || is_good<T>::value);
	};

	template<typename T> constexpr bool is_non_neutral_v = is_non_neutral<T>::value;

	template<typename T> concept NonNeutralEntity = Entity<T> && is_non_neutral<T>::value;

	template<typename T> struct is_non_player {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_non_player_v = is_non_player<T>::value;

	template<typename T> concept NonPlayerEntity = Entity<T> && is_non_player<T>::value;

	template<typename T> concept NPCEntity = Entity<T> && NonPlayerEntity<T>;

	template<typename T> struct is_npc_entity {
		static constexpr bool value = NPCEntity<T>;
	};

	template<typename T> constexpr bool is_npc_entity_v = is_npc_entity<T>::value;

	template<typename T> struct is_player {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_player_v = is_player<T>::value;

	template<typename T> concept PlayerEntity = Entity<T> && is_player<T>::value;

	template<typename T> struct is_combatant {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_combatant_v = is_combatant<T>::value;

	template<typename T> concept CombatantEntity = Entity<T> && is_combatant<T>::value;

	template<typename T> struct is_holy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_holy_v = is_holy<T>::value;

	template<typename T> concept HolyEntity = Entity<T> && is_holy<T>::value;

	template<typename T> struct is_unholy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_unholy_v = is_unholy<T>::value;

	template<typename T> concept UnholyEntity = Entity<T> && is_unholy<T>::value;

	template<typename T> struct is_docile {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_docile_v = is_docile<T>::value;

	template<typename T> concept DocileEntity = Entity<T> && is_docile<T>::value;

	template<typename T> struct is_fodder {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_fodder_v = is_fodder<T>::value;

	template<typename T> concept FodderEntity = Entity<T> && is_fodder<T>::value;

	template<typename T> struct is_clumsy {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_clumsy_v = is_clumsy<T>::value;

	template<typename T> concept ClumsyEntity = Entity<T> && is_clumsy<T>::value;

	template<typename T> struct is_elusive {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_elusive_v = is_elusive<T>::value;

	template<typename T> concept ElusiveEntity = Entity<T> && is_elusive<T>::value;

	template<typename T> struct is_inevadable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_inevadable_v = is_inevadable<T>::value;

	template<typename T> concept InevadableEntity = Entity<T> && is_inevadable<T>::value;

	template<typename T> struct is_fast {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_fast_v = is_fast<T>::value;

	template<typename T> concept FastEntity = Entity<T> && is_fast<T>::value;

	template<typename T> struct is_serene {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_serene_v = is_serene<T>::value;

	template<typename T> concept SereneEntity = Entity<T> && is_serene<T>::value;

	template<typename T> struct is_berker {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_berker_v = is_berker<T>::value;

	template<typename T> concept BerkerEntity = Entity<T> && is_berker<T>::value;

	template<typename T> struct is_armored {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_armored_v = is_armored<T>::value;

	template<typename T> concept ArmoredEntity = Entity<T> && is_armored<T>::value;

	template<typename T> struct is_cleaver {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_cleaver_v = is_cleaver<T>::value;

	template<typename T> concept CleaverEntity = Entity<T> && is_cleaver<T>::value;

	template<typename T> struct is_primeval {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_primeval_v = is_primeval<T>::value;

	template<typename T> concept PrimevalEntity = Entity<T> && is_primeval<T>::value;

	template<typename T> struct is_sneaky {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_sneaky_v = is_sneaky<T>::value;

	template<typename T> concept SneakyEntity = Entity<T> && is_sneaky<T>::value;

	template<typename T> struct is_vigilant {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_vigilant_v = is_vigilant<T>::value;

	template<typename T> concept VigilantEntity = Entity<T> && is_vigilant<T>::value;

	template<typename T> struct is_volumetric {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_volumetric_v = is_volumetric<T>::value;

	template<typename T> concept VolumetricEntity = Entity<T> && is_volumetric<T>::value;

	template<typename T> struct is_bleeder {
		static constexpr bool value = false;
		static constexpr fluid_e type = fluid_e::None;

		static constexpr bool conditional = false;
	};

	template<typename T> constexpr bool is_bleeder_v = is_bleeder<T>::value && is_bleeder<T>::type != fluid_e::None;

	template<typename T> concept BleederEntity = Entity<T> && is_bleeder_v<T>;

	template<typename T> struct is_sludger {
		static constexpr bool value = false;
		static constexpr fluid_e type = fluid_e::None;

		static constexpr bool conditional = false;
	};

	template<typename T> constexpr bool is_sludger_v = is_sludger<T>::value && is_sludger<T>::type != fluid_e::None;

	template<typename T> concept SludgerEntity = Entity<T> && is_sludger_v<T>;

	template<typename T> struct is_thirsty {
		static constexpr bool value = false;
		static constexpr fluid_e type = fluid_e::None;
	};

	template<typename T> constexpr bool is_thirsty_v = is_thirsty<T>::value && is_thirsty<T>::type != fluid_e::None;

	template<typename T> concept ThirstyEntity = Entity<T> && is_thirsty_v<T>;

	template<typename T, typename U> constexpr bool is_thirsty_for = is_thirsty_v<T> && is_bleeder_v<U> && contains(is_thirsty<T>::type, is_bleeder<U>::type);

	template<ThirstyEntity Thirster, BleederEntity Bleeder> constexpr fluid_e thirsting_for = shares(is_thirsty<Thirster>::type, is_bleeder<Bleeder>::type);

	template<typename T> struct is_devourable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_devourable_v = is_devourable<T>::value;

	template<typename T> concept DevourableEntity = Entity<T> && is_devourable<T>::value;

	template<typename T> struct is_stockable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_stockable_v = is_stockable<T>::value;

	template<typename T> concept StockableEntity = Entity<T> && is_stockable<T>::value;

	template<typename T> struct is_incorporeal {
		static constexpr bool value = false;
		static constexpr bool conditional = false;
	};

	template<typename T> constexpr bool is_incorporeal_v = is_incorporeal<T>::value;

	template<typename T> concept IncorporealEntity = Entity<T> && is_bleeder_v<T>;

	template<typename T> struct is_encouragable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_encouragable_v = is_encouragable<T>::value;

	template<typename T> concept EncouragableEntity = Entity<T> && is_encouragable<T>::value;

	template<typename T> struct is_afflictable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_afflictable_v = is_afflictable<T>::value;

	template<typename T> concept AfflicatableEntity = Entity<T> && is_afflictable<T>::value;

	template<typename T> struct is_abominable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_abominable_v = is_abominable<T>::value;

	template<typename T> concept AbominableEntity = Entity<T> && is_abominable<T>::value;

	template<typename T> struct is_concussable {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_concussable_v = is_concussable<T>::value;

	template<typename T> concept ConcussableEntity = Entity<T> && is_concussable<T>::value;

	template<typename T> struct is_spatterable {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_spatterable_v = is_spatterable<T>::value;

	template<typename T> concept SpatterableEntity = Entity<T> && is_spatterable<T>::value;

	template<typename T> struct is_updateable {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_updateable_v = is_updateable<T>::value;

	template<typename T> concept UpdateableEntity = Entity<T> && is_updateable<T>::value;

	template<typename T, entity_e EntityType> struct is_entity_type {
		static constexpr bool value = false;
	};

	template<typename T, entity_e EntityType> constexpr bool is_entity_type_v = is_entity_type<T, EntityType>::value;

	template<Entity EntityType>
		requires (!globals::has_animation<EntityType>::value && !globals::has_variants<EntityType>::value)
	inline constexpr glyph_t entity_glyphs;

	template<Entity EntityType> struct entity_t {
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

	struct omni_entity_t;

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
			requires (is_good<typename to_entity_type<Entity>::type>::value)
		constexpr bool OopsAll{ false };
	}
} // namespace necrowarp
