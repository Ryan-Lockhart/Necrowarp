#pragma once

#include <bleak.hpp>

#include <random>

#include <necrowarp/cell.hpp>
#include <necrowarp/globals.hpp>
#include <necrowarp/phase.hpp>
#include <necrowarp/dimensions/dimension.hpp>

#include <magic_enum/magic_enum_utility.hpp>

namespace necrowarp {
	using namespace bleak;

	constexpr usize default_seed{ 1337 };

	constexpr bool use_fixed_seed{ true };

	static inline usize random_seed{ std::random_device{}() };

	static inline std::mt19937_64 random_engine{ use_fixed_seed ? default_seed : random_seed };

	static inline std::mt19937_64 map_engine{ use_fixed_seed ? default_seed : random_seed };

	struct api_state_s {
		static constexpr u32 app_id{ 3631430 };
		static inline usize user_id{ 0 };
		static inline i32 pipe_id{ 0 };
	} static inline api_state;

	enum struct attribute_e : u8 {
		Energy,
		Armor,
		Divinity,
		Phantasm
	};

	template<attribute_e Type> struct attribute_t {
	  private:
		i16 minimum;
		i16 maximum;

		bool enabled;

	  public:
		inline bool is_enabled() const noexcept { return enabled; };

		inline i16 current_minimum() const noexcept { return minimum; };
		inline i16 current_maximum() const noexcept { return maximum; };

		inline void enable(i16 min, i16 max) noexcept {
			if (is_enabled()) {
				return;
			}

			minimum = min;
			maximum = max;

			enabled = true;
		};

		inline void disable() noexcept {
			if (!is_enabled()) {
				return;
			}

			enabled = false;

			minimum = 0;
			maximum = 0;
		};

		inline void set(i16 min, i16 max) noexcept {
			if (!is_enabled()) {
				return;
			}

			minimum = min;
			maximum = max;
		}
	};

	struct cheats_t {
	  private:
		bool enabled;

	  public:
	  	attribute_t<attribute_e::Energy> energy;
	  	attribute_t<attribute_e::Armor> armor;
	  	attribute_t<attribute_e::Divinity> divinity;
	  	attribute_t<attribute_e::Phantasm> phantasm;

		bool no_hit;
		bool free_costs;
		bool bypass_invocations;

		inline bool is_enabled() const noexcept { return globals::CheatsAllowed && enabled; }

		inline void activate() noexcept {
			if (!globals::CheatsAllowed) {
				return;
			}

			enabled = true;
		}

		inline void deactivate() noexcept {
			enabled = false;
		}
	};

	enum struct wave_size_e : u8 {
		Miniscule,
		Tiny,
		Small,
		Medium,
		Large,
		Huge,
		Massive
	};

	struct game_stats_t {
		template<wave_size_e Size> static constexpr i16 WaveSize{ 0 };
	
		template<> inline constexpr i16 WaveSize<wave_size_e::Miniscule>{ 4 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Tiny>{ 8 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Small>{ 16 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Medium>{ 32 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Large>{ 64 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Huge>{ 128 };
		template<> inline constexpr i16 WaveSize<wave_size_e::Massive>{ 256 };
	
		static constexpr i16 MinimumWaveSize{ WaveSize<wave_size_e::Miniscule> };
		static constexpr i16 MaximumWaveSize{ WaveSize<wave_size_e::Massive> };
	
		static constexpr i16 StartingWaveSize{ 4 };

		static constexpr u16 SpawnDistributionLow{ 0 };
		static constexpr u16 SpawnDistributionHigh{ 100 };

		std::uniform_int_distribution<u16> spawn_dis{ SpawnDistributionLow, SpawnDistributionHigh };

		template<RandomEngine Generator> inline u8 spawn_chance(ref<Generator> engine) { return static_cast<u8>(spawn_dis(engine)); };

		cheats_t cheats{};

		usize game_seed{};

		usize game_depth{ 0 };

		usize wave_size{ static_cast<usize>(StartingWaveSize) };

		inline wave_size_e determine_wave_size() const noexcept {
			if (wave_size <= WaveSize<wave_size_e::Massive> && wave_size > WaveSize<wave_size_e::Huge>) {
				return wave_size_e::Massive;
			} else if (wave_size <= WaveSize<wave_size_e::Huge> && wave_size > WaveSize<wave_size_e::Large>) {
				return wave_size_e::Huge;
			} else if (wave_size <= WaveSize<wave_size_e::Large> && wave_size > WaveSize<wave_size_e::Medium>) {
				return wave_size_e::Large;
			} else if (wave_size <= WaveSize<wave_size_e::Medium> && wave_size > WaveSize<wave_size_e::Small>) {
				return wave_size_e::Medium;
			} else if (wave_size <= WaveSize<wave_size_e::Small> && wave_size > WaveSize<wave_size_e::Tiny>) {
				return wave_size_e::Small;
			} else if (wave_size <= WaveSize<wave_size_e::Tiny> && wave_size > WaveSize<wave_size_e::Miniscule>) {
				return wave_size_e::Tiny;
			} else {
				return wave_size_e::Miniscule;
			}
		}

		usize spawns_remaining{ static_cast<usize>(StartingWaveSize) };

		inline usize has_spawns() const noexcept { return spawns_remaining > 0; }

		i16	player_kills{ 0 };
		i16	minion_kills{ 0 };

		inline i16 total_kills() const noexcept { return player_kills + minion_kills; };

		inline void update_wave_size() noexcept { wave_size = clamp(static_cast<i16>(StartingWaveSize + total_kills() / globals::KillsPerPopulation), MinimumWaveSize, MaximumWaveSize); }

		inline i8 kills_until_next_energy_slot() const noexcept { return abs(minion_kills % globals::KillsPerEnergySlot - globals::KillsPerEnergySlot); }

		inline i8 kills_until_next_armor_slot() const noexcept { return abs(player_kills % globals::KillsPerArmorSlot - globals::KillsPerArmorSlot); }

		inline i8 kills_until_next_divinity_turn() const noexcept { return abs(total_kills() % globals::KillsPerDivinityTurn - globals::KillsPerDivinityTurn); }
		
		inline i8 kills_until_next_phantasm_turn() const noexcept { return abs(total_kills() % globals::KillsPerPhantasmTurn - globals::KillsPerPhantasmTurn); }

		inline i16 current_reinforcements() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerReinforcement, globals::MinimumReinforcements, globals::MaximumReinforcements); }
		
		inline bool has_reinforcements() const noexcept { return current_reinforcements() > 0; }

		inline void reset() noexcept {
			game_depth = 0;

			game_seed = use_fixed_seed ? default_seed : std::random_device{}();

			random_engine.seed(game_seed);
			map_engine.seed(game_seed);

			wave_size = StartingWaveSize;
			spawns_remaining = StartingWaveSize;

			player_kills = 0;
			minion_kills = 0;
		}
	};

	enum struct steam_achievement_e : u8 {
		BasicSkeletonSummoning,

		AcquireBoneArmor,

		ChaoticWarpUsage,
		PanicWarpUsage,
		PanicWarpDeath,

		PreciseWarpUsage,
		PreciseWarpToBones,
		PreciseWarpToArmorMinion,

		BasicAnnihilateUsage,
		ExceptionalAnnihilateUsage,

		BasicRepulseUsage,
		ExceptionalRepulseUsage,

		CalcifyWallIntoBones,
		CalcifyBonesIntoWall,
		CalcifyBonesWithEnemy,

		BasicIncorporealizeUsage,
		ExceptionalIncorporealizeUsage,
		RecorporealizeDeath,

		CompleteBasicTutorial,
		CompleteAdvancedTutorial,

		DescendDownLadder,

		ReceiveDivineIntervention,

		DefeatOverworldSiege,

		EnterAncientVault,

		CollectKharoplasm,

		ReceiveEldritchAudience,

		DefeatElbikezzir,

		DefeatRebelEncampment,

		ReceiveEldritchMonologue,

		DefeatPraethornyn,

		ReceiveImperialAudience,

		ReturnToRaetun,

		CalciticEnshackling,
		CalciticUnshackling,

		SpectralEnshackling,
		SpectralUnshackling,

		SanguineEnshackling,
		SanguineUnshackling,

		GalvanicEnshackling,
		GalvanicUnshackling,

		RavenousEnshackling,
		RavenousUnshackling,

		WretchedEnshackling,
		WretchedUnshackling,

		CerebralEnshackling,
		CerebralUnshackling,

		InfernalEnshackling,
		InfernalUnshackling,

		LesserSkeletonSummoning,
		GreaterSkeletonSummoning,

		LesserBonespurSummoning,
		GreaterBonespurSummoning,

		LesserCultistSummoning,
		GreaterCultistSummoning,

		LesserWraithSummoning,
		GreaterWraithSummoning,

		LesserBloodhoundSummoning,
		GreaterBloodhoundSummoning,

		LesserHemogheistSummoning,
		GreaterHemogheistSummoning,

		LesserAnimatedSuitSummoning,
		GreaterAnimatedSuitSummoning,

		LesserDeathKnightSummoning,
		GreaterDeathKnightSummoning,

		LesserAbominationSummoning,
		GreaterAbominationSummoning,

		LesserFleshGolemSummoning,
		GreaterFleshGolemSummoning,

		LesserDraugazSummoning,
		GreaterDraugazSummoning,

		LesserDreadwurmSummoning,
		GreaterDreadwurmSummoning,

		LesserHamazSummoning,
		GreaterHamazSummoning,

		LesserFurtiveHorrorSummoning,
		GreaterFurtiveHorrorSummoning,

		LesserChromaleseSummoning,
		GreaterChromaleseSummoning,

		LesserIsoscelSummoning,
		GreaterIsoscelSummoning,

		BoneConsumption,
		MetalConsumption,
		FleshConsumption,
		CerebraConsumption,

		SolidCatalystConsumption,

		BloodConsumption,
		IchorConsumption,
		FilthConsumption,
		EctoplasmConsumption,

		FluidCatalystConsumption,
	};

	constexpr const u8 AchievementCount{ static_cast<u8>(steam_achievement_e::FluidCatalystConsumption) + 1 };

	template<steam_achievement_e Achievement> static constexpr cstr api_key{ "" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BasicSkeletonSummoning>{ "basic_skeleton_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::AcquireBoneArmor>{ "acquire_bone_armor" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ChaoticWarpUsage>{ "chaotic_warp_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::PanicWarpUsage>{ "panic_warp_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::PanicWarpDeath>{ "panic_warp_death" };

	template<> inline constexpr cstr api_key<steam_achievement_e::PreciseWarpUsage>{ "precise_warp_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::PreciseWarpToBones>{ "precise_warp_to_bones" };
	template<> inline constexpr cstr api_key<steam_achievement_e::PreciseWarpToArmorMinion>{ "precise_warp_to_armor_minion" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BasicAnnihilateUsage>{ "basic_annihilate_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::ExceptionalAnnihilateUsage>{ "exceptional_annihilate_usage" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BasicRepulseUsage>{ "basic_repulse_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::ExceptionalRepulseUsage>{ "exceptional_repulse_usage" };

	template<> inline constexpr cstr api_key<steam_achievement_e::CalcifyWallIntoBones>{ "calcify_wall_into_bones" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CalcifyBonesIntoWall>{ "calcify_bones_into_wall" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CalcifyBonesWithEnemy>{ "calcify_bones_with_enemy" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BasicIncorporealizeUsage>{ "basic_incorporealize_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::ExceptionalIncorporealizeUsage>{ "exceptional_incorporealize_usage" };
	template<> inline constexpr cstr api_key<steam_achievement_e::RecorporealizeDeath>{ "recorporealize_death" };

	template<> inline constexpr cstr api_key<steam_achievement_e::CompleteBasicTutorial>{ "complete_basic_tutorial" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CompleteAdvancedTutorial>{ "complete_advanced_tutorial" };

	template<> inline constexpr cstr api_key<steam_achievement_e::DescendDownLadder>{ "descend_down_ladder" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ReceiveDivineIntervention>{ "receive_divine_intervention" };

	template<> inline constexpr cstr api_key<steam_achievement_e::DefeatOverworldSiege>{ "defeat_overworld_siege" };

	template<> inline constexpr cstr api_key<steam_achievement_e::EnterAncientVault>{ "enter_ancient_vault" };

	template<> inline constexpr cstr api_key<steam_achievement_e::CollectKharoplasm>{ "collect_kharoplasm" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ReceiveEldritchAudience>{ "receive_eldritch_audience" };

	template<> inline constexpr cstr api_key<steam_achievement_e::DefeatElbikezzir>{ "defeat_elbikezzir" };

	template<> inline constexpr cstr api_key<steam_achievement_e::DefeatRebelEncampment>{ "defeat_rebel_encampment" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ReceiveEldritchMonologue>{ "receive_eldritch_monologue" };

	template<> inline constexpr cstr api_key<steam_achievement_e::DefeatPraethornyn>{ "defeat_praethornyn" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ReceiveImperialAudience>{ "receive_imperial_audience" };

	template<> inline constexpr cstr api_key<steam_achievement_e::ReturnToRaetun>{ "return_to_raetun" };

	template<> inline constexpr cstr api_key<steam_achievement_e::CalciticEnshackling>{ "calcitic_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CalciticUnshackling>{ "calcitic_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::SpectralEnshackling>{ "spectral_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::SpectralUnshackling>{ "spectral_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::SanguineEnshackling>{ "sanguine_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::SanguineUnshackling>{ "sanguine_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::GalvanicEnshackling>{ "galvanic_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GalvanicUnshackling>{ "galvanic_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::RavenousEnshackling>{ "ravenous_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::RavenousUnshackling>{ "ravenous_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::WretchedEnshackling>{ "wretched_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::WretchedUnshackling>{ "wretched_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::CerebralEnshackling>{ "cerebral_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CerebralUnshackling>{ "cerebral_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::InfernalEnshackling>{ "infernal_enshackling" };
	template<> inline constexpr cstr api_key<steam_achievement_e::InfernalUnshackling>{ "infernal_unshackling" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserSkeletonSummoning>{ "lesser_skeleton_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterSkeletonSummoning>{ "greater_skeleton_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserBonespurSummoning>{ "lesser_bonespur_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterBonespurSummoning>{ "greater_bonespur_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserCultistSummoning>{ "lesser_cultist_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterCultistSummoning>{ "greater_cultist_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserWraithSummoning>{ "lesser_wraith_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterWraithSummoning>{ "greater_wraith_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserBloodhoundSummoning>{ "lesser_bloodhound_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterBloodhoundSummoning>{ "greater_bloodhound_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserHemogheistSummoning>{ "lesser_hemogheist_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterHemogheistSummoning>{ "greater_hemogheist_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserAnimatedSuitSummoning>{ "lesser_animated_suit_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterAnimatedSuitSummoning>{ "greater_animated_suit_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserDeathKnightSummoning>{ "lesser_death_knight_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterDeathKnightSummoning>{ "greater_death_knight_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserAbominationSummoning>{ "lesser_abomination_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterAbominationSummoning>{ "greater_abomination_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserFleshGolemSummoning>{ "lesser_flesh_golem_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterFleshGolemSummoning>{ "greater_flesh_golem_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserDraugazSummoning>{ "lesser_draugaz_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterDraugazSummoning>{ "greater_draugaz_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserDreadwurmSummoning>{ "lesser_dreadwurm_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterDreadwurmSummoning>{ "greater_dreadwurm_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserHamazSummoning>{ "lesser_hamaz_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterHamazSummoning>{ "greater_hamaz_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserFurtiveHorrorSummoning>{ "lesser_furtive_horror_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterFurtiveHorrorSummoning>{ "greater_furtive_horror_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserChromaleseSummoning>{ "lesser_chromalese_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterChromaleseSummoning>{ "greater_chromalese_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::LesserIsoscelSummoning>{ "lesser_isoscel_summoning" };
	template<> inline constexpr cstr api_key<steam_achievement_e::GreaterIsoscelSummoning>{ "greater_isoscel_summoning" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BoneConsumption>{ "bone_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::MetalConsumption>{ "metal_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::FleshConsumption>{ "flesh_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::CerebraConsumption>{ "cerebra_consumption" };

	template<> inline constexpr cstr api_key<steam_achievement_e::SolidCatalystConsumption>{ "solid_catalyst_consumption" };

	template<> inline constexpr cstr api_key<steam_achievement_e::BloodConsumption>{ "blood_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::IchorConsumption>{ "ichor_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::FilthConsumption>{ "filth_consumption" };
	template<> inline constexpr cstr api_key<steam_achievement_e::EctoplasmConsumption>{ "ectoplasm_consumption" };

	template<> inline constexpr cstr api_key<steam_achievement_e::FluidCatalystConsumption>{ "fluid_catalyst_consumption" };

	template<steam_achievement_e Achievement> static constexpr cstr display_name{ "" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BasicSkeletonSummoning>{ "The Summoning Begins" };

	template<> inline constexpr cstr display_name<steam_achievement_e::AcquireBoneArmor>{ "Bony Breastplate" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ChaoticWarpUsage>{ "Vanishing Act" };
	template<> inline constexpr cstr display_name<steam_achievement_e::PanicWarpUsage>{ "Fool's Gambit" };
	template<> inline constexpr cstr display_name<steam_achievement_e::PanicWarpDeath>{ "Wrong Place, Wrong Time" };

	template<> inline constexpr cstr display_name<steam_achievement_e::PreciseWarpUsage>{ "Intentionality" };
	template<> inline constexpr cstr display_name<steam_achievement_e::PreciseWarpToBones>{ "Double Jump" };
	template<> inline constexpr cstr display_name<steam_achievement_e::PreciseWarpToArmorMinion>{ "Remote Armament" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BasicAnnihilateUsage>{ "Path of Destruction" };
	template<> inline constexpr cstr display_name<steam_achievement_e::ExceptionalAnnihilateUsage>{ "Splashback" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BasicRepulseUsage>{ "Personal Space" };
	template<> inline constexpr cstr display_name<steam_achievement_e::ExceptionalRepulseUsage>{ "Otherworldly Check" };

	template<> inline constexpr cstr display_name<steam_achievement_e::CalcifyWallIntoBones>{ "Calcium Deposit" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CalcifyBonesIntoWall>{ "Constructive Influence" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CalcifyBonesWithEnemy>{ "Petrifying!" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BasicIncorporealizeUsage>{ "Abyssal Adventure" };
	template<> inline constexpr cstr display_name<steam_achievement_e::ExceptionalIncorporealizeUsage>{ "Toodles!" };
	template<> inline constexpr cstr display_name<steam_achievement_e::RecorporealizeDeath>{ "Matter Rearrangement" };

	template<> inline constexpr cstr display_name<steam_achievement_e::CompleteBasicTutorial>{ "Nascent Necromancer" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CompleteAdvancedTutorial>{ "The Prodigal Pupil" };

	template<> inline constexpr cstr display_name<steam_achievement_e::DescendDownLadder>{ "And Yet Deeper We Must Delve" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ReceiveDivineIntervention>{ "Destined Death" };

	template<> inline constexpr cstr display_name<steam_achievement_e::DefeatOverworldSiege>{ "A Minor Cuncation" };

	template<> inline constexpr cstr display_name<steam_achievement_e::EnterAncientVault>{ "Once Unbidden; Hence Unsealed" };

	template<> inline constexpr cstr display_name<steam_achievement_e::CollectKharoplasm>{ "A Perturbing Prize" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ReceiveEldritchAudience>{ "Odious Tidings" };

	template<> inline constexpr cstr display_name<steam_achievement_e::DefeatElbikezzir>{ "Barotrauma" };

	template<> inline constexpr cstr display_name<steam_achievement_e::DefeatRebelEncampment>{ "Rebel Razing" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ReceiveEldritchMonologue>{ "Soliloquy of the Damned" };

	template<> inline constexpr cstr display_name<steam_achievement_e::DefeatPraethornyn>{ "Sandy Slumber" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ReceiveImperialAudience>{ "The Pale Princess" };

	template<> inline constexpr cstr display_name<steam_achievement_e::ReturnToRaetun>{ "Reunion" };

	template<> inline constexpr cstr display_name<steam_achievement_e::CalciticEnshackling>{ "calcitic_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CalciticUnshackling>{ "calcitic_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::SpectralEnshackling>{ "spectral_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::SpectralUnshackling>{ "spectral_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::SanguineEnshackling>{ "sanguine_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::SanguineUnshackling>{ "sanguine_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::GalvanicEnshackling>{ "galvanic_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GalvanicUnshackling>{ "galvanic_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::RavenousEnshackling>{ "ravenous_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::RavenousUnshackling>{ "ravenous_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::WretchedEnshackling>{ "wretched_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::WretchedUnshackling>{ "wretched_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::CerebralEnshackling>{ "cerebral_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CerebralUnshackling>{ "cerebral_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::InfernalEnshackling>{ "infernal_enshackling" };
	template<> inline constexpr cstr display_name<steam_achievement_e::InfernalUnshackling>{ "infernal_unshackling" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserSkeletonSummoning>{ "lesser_skeleton_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterSkeletonSummoning>{ "greater_skeleton_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserBonespurSummoning>{ "lesser_bonespur_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterBonespurSummoning>{ "greater_bonespur_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserCultistSummoning>{ "lesser_cultist_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterCultistSummoning>{ "greater_cultist_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserWraithSummoning>{ "lesser_wraith_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterWraithSummoning>{ "greater_wraith_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserBloodhoundSummoning>{ "lesser_bloodhound_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterBloodhoundSummoning>{ "greater_bloodhound_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserHemogheistSummoning>{ "lesser_hemogheist_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterHemogheistSummoning>{ "greater_hemogheist_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserAnimatedSuitSummoning>{ "lesser_animated_suit_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterAnimatedSuitSummoning>{ "greater_animated_suit_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserDeathKnightSummoning>{ "lesser_death_knight_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterDeathKnightSummoning>{ "greater_death_knight_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserAbominationSummoning>{ "lesser_abomination_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterAbominationSummoning>{ "greater_abomination_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserFleshGolemSummoning>{ "lesser_flesh_golem_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterFleshGolemSummoning>{ "greater_flesh_golem_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserDraugazSummoning>{ "lesser_draugaz_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterDraugazSummoning>{ "greater_draugaz_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserDreadwurmSummoning>{ "lesser_dreadwurm_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterDreadwurmSummoning>{ "greater_dreadwurm_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserHamazSummoning>{ "lesser_hamaz_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterHamazSummoning>{ "greater_hamaz_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserFurtiveHorrorSummoning>{ "lesser_furtive_horror_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterFurtiveHorrorSummoning>{ "greater_furtive_horror_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserChromaleseSummoning>{ "lesser_chromalese_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterChromaleseSummoning>{ "greater_chromalese_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::LesserIsoscelSummoning>{ "lesser_isoscel_summoning" };
	template<> inline constexpr cstr display_name<steam_achievement_e::GreaterIsoscelSummoning>{ "greater_isoscel_summoning" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BoneConsumption>{ "Karkaphage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::FleshConsumption>{ "Sarkaphage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::MetalConsumption>{ "Ferrophage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::CerebraConsumption>{ "Myalaphage" };

	template<> inline constexpr cstr display_name<steam_achievement_e::SolidCatalystConsumption>{ "Sterophage" };

	template<> inline constexpr cstr display_name<steam_achievement_e::BloodConsumption>{ "Hemophage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::IchorConsumption>{ "Ichophage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::FilthConsumption>{ "Rupaphage" };
	template<> inline constexpr cstr display_name<steam_achievement_e::EctoplasmConsumption>{ "Ectophage" };

	template<> inline constexpr cstr display_name<steam_achievement_e::FluidCatalystConsumption>{ "Yrgophage" };

	static inline constexpr cstr to_api_key(steam_achievement_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr steam_achievement_e cval{ val };

			return api_key<cval>;
		}, type);
	}

	static inline constexpr cstr to_display_name(steam_achievement_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr steam_achievement_e cval{ val };

			return display_name<cval>;
		}, type);
	}

	enum struct steam_stat_e : u8 {
		PlayerKills = 0,
		MinionKills,

		PlayerDeaths,

		ChaoticWarps,
		PreciseWarps,

		Calcifications,
		Repulsions,
		Annihilations,
		Incorporealizations,

		CalciticInvocations,
		SpectralInvocations,
		SanguineInvocations,
		GalvanicInvocations,
		RavenousInvocations,
		WretchedInvocations,
		CerebralInvocations,
		InfernalInvocations,

		NecromanticAscensions,
		CalamitousRetaliations,

		MetersMoved,
		MetersWarped,
		LowestDepth,
		PortalsTraversed,

		BonesConsumed,
		FleshConsumed,
		MetalConsumed,
		CerebraConsumed,

		BloodConsumed,
		IchorConsumed,
		FilthConsumed,
		EctoplasmConsumed,

		SkeletonsConsumed,
		BonespursConsumed,

		GrimoiresCollected,

		LaddersUnshackled,
		LaddersShackled,

		AdventurersSlain,
		MercenariesSlain,
		RangersSlain,
		SkulkersSlain,
		ManslingsSlain,
		MistLadiesSlain,
		BannerBearersSlain,
		MediciiSlain,
		BattleMonksSlain,
		BerserkersSlain,
		HexeatersSlain,
		PaladinsSlain,

		ThetwoSlain,
	};

	template<steam_stat_e Stat> struct to_stat_type;

	template<> struct to_stat_type<steam_stat_e::PlayerKills> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MinionKills> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::PlayerDeaths> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::ChaoticWarps> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PreciseWarps> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::Calcifications> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::Repulsions> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::Annihilations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::Incorporealizations> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::CalciticInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::SpectralInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::SanguineInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::GalvanicInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::RavenousInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::WretchedInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::CerebralInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::InfernalInvocations> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::NecromanticAscensions> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::CalamitousRetaliations> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::MetersMoved> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::MetersWarped> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::LowestDepth> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PortalsTraversed> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::BonesConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::FleshConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MetalConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::CerebraConsumed> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::BloodConsumed> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::IchorConsumed> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::FilthConsumed> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::EctoplasmConsumed> { using type = f32; };

	template<> struct to_stat_type<steam_stat_e::SkeletonsConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BonespursConsumed> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::GrimoiresCollected> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::LaddersUnshackled> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::LaddersShackled> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::AdventurersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MercenariesSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::RangersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::SkulkersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::ManslingsSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MistLadiesSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BannerBearersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MediciiSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BattleMonksSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BerserkersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::HexeatersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PaladinsSlain> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::ThetwoSlain> { using type = i32; };

	template<steam_stat_e StatType> using to_stat_type_t = typename to_stat_type<StatType>::type;

	static inline constexpr cstr to_api_str(steam_stat_e type) noexcept {
		switch (type) {
			case steam_stat_e::PlayerKills: {
				return "player_kills";
			} case steam_stat_e::MinionKills: {
				return "minion_kills";
			} case steam_stat_e::PlayerDeaths: {
				return "player_deaths";
			} case steam_stat_e::ChaoticWarps: {
				return "chaotic_warps";
			} case steam_stat_e::PreciseWarps: {
				return "precise_warps";
			} case steam_stat_e::Calcifications: {
				return "calcifications";
			} case steam_stat_e::Repulsions: {
				return "repulsions";
			} case steam_stat_e::Annihilations: {
				return "annihilations";
			} case steam_stat_e::Incorporealizations: {
				return "incorporealizations";
			} case steam_stat_e::CalciticInvocations: {
				return "calcitic_invocations";
			} case steam_stat_e::SpectralInvocations: {
				return "spectral_invocations";
			} case steam_stat_e::SanguineInvocations: {
				return "sanguine_invocations";
			} case steam_stat_e::GalvanicInvocations: {
				return "galvanic_invocations";
			} case steam_stat_e::RavenousInvocations: {
				return "ravenous_invocations";
			} case steam_stat_e::WretchedInvocations: {
				return "wretched_invocations";
			} case steam_stat_e::CerebralInvocations: {
				return "cerebral_invocations";
			} case steam_stat_e::InfernalInvocations: {
				return "infernal_invocations";
			} case steam_stat_e::NecromanticAscensions: {
				return "necromantic_ascensions";
			} case steam_stat_e::CalamitousRetaliations: {
				return "calamitous_retaliations";
			} case steam_stat_e::MetersMoved: {
				return "meters_moved";
			} case steam_stat_e::MetersWarped: {
				return "meters_warped";
			} case steam_stat_e::LowestDepth: {
				return "lowest_depth";
			} case steam_stat_e::PortalsTraversed: {
				return "portals_traversed";
			} case steam_stat_e::BonesConsumed: {
				return "bones_consumed";
			} case steam_stat_e::FleshConsumed: {
				return "flesh_consumed";
			} case steam_stat_e::MetalConsumed: {
				return "metal_consumed";
			} case steam_stat_e::CerebraConsumed: {
				return "cerebra_consumed";
			} case steam_stat_e::BloodConsumed: {
				return "blood_consumed";
			} case steam_stat_e::IchorConsumed: {
				return "ichor_consumed";
			} case steam_stat_e::FilthConsumed: {
				return "flith_consumed";
			} case steam_stat_e::EctoplasmConsumed: {
				return "ectoplasm_consumed";
			} case steam_stat_e::SkeletonsConsumed: {
				return "skeletons_consumed";
			} case steam_stat_e::BonespursConsumed: {
				return "bonespurs_consumed";
			} case steam_stat_e::GrimoiresCollected: {
				return "grimoires_collected";
			} case steam_stat_e::LaddersUnshackled: {
				return "ladders_unshackled";
			} case steam_stat_e::LaddersShackled: {
				return "ladders_shackled";
			} case steam_stat_e::AdventurersSlain: {
				return "adventurers_slain";
			} case steam_stat_e::MercenariesSlain: {
				return "mercenaries_slain";
			} case steam_stat_e::RangersSlain: {
				return "rangers_slain";
			} case steam_stat_e::SkulkersSlain: {
				return "skulkers_slain";
			} case steam_stat_e::ManslingsSlain: {
				return "manslings_slain";
			} case steam_stat_e::MistLadiesSlain: {
				return "mist_ladies_slain";
			} case steam_stat_e::BannerBearersSlain: {
				return "banner_bearers_slain";
			} case steam_stat_e::MediciiSlain: {
				return "medicii_slain";
			} case steam_stat_e::BattleMonksSlain: {
				return "battle_monks_slain";
			} case steam_stat_e::BerserkersSlain: {
				return "berserkers_slain";
			} case steam_stat_e::HexeatersSlain: {
				return "hexeaters_slain";
			} case steam_stat_e::PaladinsSlain: {
				return "paladins_slain";
			} case steam_stat_e::ThetwoSlain: {
				return "thetwo_slain";
			}
		}
	}

	static inline constexpr cstr to_display_str(steam_stat_e type) noexcept {
		switch (type) {
			case steam_stat_e::PlayerKills: {
				return "Player Kills";
			} case steam_stat_e::MinionKills: {
				return "Minion Kills";
			} case steam_stat_e::PlayerDeaths: {
				return "Player Deaths";
			} case steam_stat_e::ChaoticWarps: {
				return "Chaotic Warps";
			} case steam_stat_e::PreciseWarps: {
				return "Precise Warps";
			} case steam_stat_e::Annihilations: {
				return "Annihilations";
			} case steam_stat_e::Repulsions: {
				return "Repulsions";
			} case steam_stat_e::Calcifications: {
				return "Calcifications";
			} case steam_stat_e::Incorporealizations: {
				return "Incorporealizations";
			} case steam_stat_e::CalciticInvocations: {
				return "Calcitic Invocations";
			} case steam_stat_e::SpectralInvocations: {
				return "Spectral Invocations";
			} case steam_stat_e::SanguineInvocations: {
				return "Sanguine Invocations";
			} case steam_stat_e::GalvanicInvocations: {
				return "Galvanic Invocations";
			} case steam_stat_e::RavenousInvocations: {
				return "Ravenous Invocations";
			} case steam_stat_e::WretchedInvocations: {
				return "Wretched Invocations";
			} case steam_stat_e::CerebralInvocations: {
				return "Cerebral Invocations";
			} case steam_stat_e::InfernalInvocations: {
				return "Infernal Invocations";
			} case steam_stat_e::NecromanticAscensions: {
				return "Necromantic Ascensions";
			} case steam_stat_e::CalamitousRetaliations: {
				return "Calamitous Retaliations";
			} case steam_stat_e::MetersMoved: {
				return "Meters Moved";
			} case steam_stat_e::MetersWarped: {
				return "Meters Warped";
			} case steam_stat_e::LowestDepth: {
				return "Lowest Depth";
			} case steam_stat_e::PortalsTraversed: {
				return "Portals Traversed";
			} case steam_stat_e::BonesConsumed: {
				return "Bones Consumed";
			} case steam_stat_e::FleshConsumed: {
				return "Flesh Consumed";
			} case steam_stat_e::MetalConsumed: {
				return "Metal Consumed";
			} case steam_stat_e::CerebraConsumed: {
				return "Cerebra Consumed";
			} case steam_stat_e::BloodConsumed: {
				return "Blood Consumed";
			} case steam_stat_e::IchorConsumed: {
				return "Ichor Consumed";
			} case steam_stat_e::FilthConsumed: {
				return "Filth Consumed";
			} case steam_stat_e::EctoplasmConsumed: {
				return "Ectoplasm Consumed";
			} case steam_stat_e::SkeletonsConsumed: {
				return "Skeletons Consumed";
			} case steam_stat_e::BonespursConsumed: {
				return "Bonespurs Consumed";
			} case steam_stat_e::GrimoiresCollected: {
				return "Grimoires Collected";
			} case steam_stat_e::LaddersUnshackled: {
				return "Ladders Unshackled";
			} case steam_stat_e::LaddersShackled: {
				return "Ladders Shackled";
			} case steam_stat_e::AdventurersSlain: {
				return "Adventurers Slain";
			} case steam_stat_e::MercenariesSlain: {
				return "Mercenaries Slain";
			} case steam_stat_e::RangersSlain: {
				return "Rangers Slain";
			} case steam_stat_e::SkulkersSlain: {
				return "Skulkers Slain";
			} case steam_stat_e::ManslingsSlain: {
				return "Manslings Slain";
			} case steam_stat_e::MistLadiesSlain: {
				return "Mist Ladies Slain";
			} case steam_stat_e::BannerBearersSlain: {
				return "Banner Bearers Slain";
			} case steam_stat_e::MediciiSlain: {
				return "Medicii Slain";
			} case steam_stat_e::BattleMonksSlain: {
				return "Battle Monks Slain";
			} case steam_stat_e::BerserkersSlain: {
				return "Berserkers Slain";
			} case steam_stat_e::HexeatersSlain: {
				return "Hexeaters Slain";
			} case steam_stat_e::PaladinsSlain: {
				return "Paladins Slain";
			} case steam_stat_e::ThetwoSlain: {
				return "Thetwo Slain";
			}
		}
	}

	template<steam_stat_e Type, typename T>
		requires is_one_of<T, i32, f32>::value
	struct steam_stat_t {
		using value_type = T;

		static inline constexpr steam_stat_e type{ Type };

		static inline constexpr cstr api_name{ to_api_str(Type) };
		static inline constexpr cstr display_name{ to_display_str(Type) };

		static inline value_type cached_value;
		static inline value_type initial_value;

		inline T get_value() const noexcept  {
			if constexpr (IsSteamless) {
				return T{};
			}

			std::optional<T> maybe_value{ steam::user_stats::get_stat<T>(api_name) };

			if (!maybe_value.has_value()) {
				error_log.add("[WARNING]: returning cached value of \"{}\"...", api_name);

				return cached_value;
			}

			return cached_value = maybe_value.value();
		}

		inline bool set_value(value_type value) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return steam::user_stats::set_stat(api_name, value);
		}

		inline ref<steam_stat_t<Type, T>> operator=(T other) noexcept {
			set_value(other);

			return *this;
		}

		inline ref<steam_stat_t<Type, T>> operator+=(T other) noexcept {
			const T value = get_value();

			set_value(value + other);

			return *this;
		}

		inline ref<steam_stat_t<Type, T>> operator-=(T other) noexcept {
			const T value = get_value();
			
			set_value(value - other);

			return *this;
		}

		inline ref<steam_stat_t<Type, T>> operator++() noexcept {
			const T value = get_value();

			set_value(value + 1);

			return *this;
		}

		inline ref<steam_stat_t<Type, T>> operator--() noexcept {
			const T value = get_value();
			
			set_value(value - 1);

			return *this;
		}

		inline T operator++(int) noexcept {
			const T value = get_value();

			set_value(value + 1);

			return value;
		}

		inline T operator--(int) noexcept {
			const T value = get_value();
			
			set_value(value - 1);

			return value;
		}
	};

	struct steam_stats_s {
		static STEAM_CALLBACK(steam_stats_s, on_user_stats_unloaded, UserStatsUnloaded_t);
		static STEAM_CALLBACK(steam_stats_s, on_user_stats_received, UserStatsReceived_t);
		static STEAM_CALLBACK(steam_stats_s, on_user_stats_stored, UserStatsStored_t);
		static STEAM_CALLBACK(steam_stats_s, on_global_stats_received, GlobalStatsReceived_t);

		template<steam_stat_e Type> static inline steam_stat_t<Type, typename to_stat_type<Type>::type> stats{};
		
		static inline ptr<ISteamUserStats> user_stats() {
			if constexpr (IsSteamless) {
				return nullptr;
			}

			ptr<ISteamUserStats> interface = SteamUserStats();

			if (interface == nullptr) {
				error_log.add("[ERROR]: unable to access steam user stats api!");
			}

			return interface;
		}

		static inline bool is_unlocked(steam_achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			bool unlocked{ false };

			return user_stats()->GetAchievement(to_api_key(achievement), &unlocked) && unlocked;
		}

		static inline bool reset(steam_achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return user_stats()->ClearAchievement(to_api_key(achievement));
		}

		static inline void reset() noexcept {
			if constexpr (IsSteamless) {
				return;
			}

			magic_enum::enum_for_each<steam_achievement_e>([&](auto val) {
				constexpr steam_achievement_e cval{ val };

				user_stats()->ClearAchievement(api_key<cval>);
			});
		}

		static inline bool unlock(steam_achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return user_stats()->SetAchievement(to_api_key(achievement));
		}

		static inline void unlock() noexcept {
			if constexpr (IsSteamless) {
				return;
			}

			magic_enum::enum_for_each<steam_achievement_e>([&](auto val) {
				constexpr steam_achievement_e cval{ val };

				user_stats()->SetAchievement(api_key<cval>);
			});
		}

		static inline bool store() noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return user_stats()->StoreStats();
		}

		static inline void transcribe() noexcept {
			if constexpr (IsSteamless) {
				return;
			}

			magic_enum::enum_for_each<steam_stat_e>([] (auto val) -> void {
				constexpr steam_stat_e Stat{ val };
				
				stats<Stat>.initial_value = stats<Stat>.get_value();
			});
		}

		static inline bool is_user_id_valid(usize user_id) noexcept { return user_id == api_state.user_id; }

		static inline bool is_user_id_valid(CSteamID user_id) noexcept { return steam::user::cast_steam_id(user_id) == api_state.user_id; }

		static inline bool is_game_id_valid(usize game_id) noexcept { return game_id == api_state.app_id; }

		static inline bool is_valid(usize game_id, CSteamID user_id) noexcept { return game_id == api_state.app_id && user_id == api_state.user_id; }
	};

	using steam_stats = steam_stats_s;

	inline void steam_stats_s::on_user_stats_unloaded(ptr<UserStatsUnloaded_t> callback) {
		if (callback == nullptr || !is_user_id_valid(callback->m_steamIDUser)) {
			return;
		}
	}

	inline void steam_stats_s::on_user_stats_received(ptr<UserStatsReceived_t> callback) {
		if (callback == nullptr || !is_user_id_valid(callback->m_steamIDUser) || callback->m_eResult == k_EResultFail) {
			return;
		}
	}

	inline void steam_stats_s::on_user_stats_stored(ptr<UserStatsStored_t> callback) {
		if (callback == nullptr || !is_game_id_valid(callback->m_nGameID) || callback->m_eResult == k_EResultFail) {
			return;
		}
	}

	inline void steam_stats_s::on_global_stats_received(ptr<GlobalStatsReceived_t> callback) {
		if (callback == nullptr || !is_game_id_valid(callback->m_nGameID) || callback->m_eResult == k_EResultFail) {
			return;
		}
	}
} // namespace necrowarp
