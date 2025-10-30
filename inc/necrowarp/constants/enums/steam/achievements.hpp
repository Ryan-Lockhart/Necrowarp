#pragma once

#include <necrowarp/constants/enums/base.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct achievement_e : u8 {
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
		CalcifyBonesUnderSelf,

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

		PatheticSkeletonSummoning,
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
		ExaltedDeathKnightSummoning,

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
		ExaltedHamazSummoning,

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

	constexpr const u8 AchievementCount{ static_cast<u8>(achievement_e::FluidCatalystConsumption) + 1 };

	namespace achievements {
		template<achievement_e Achievement> static constexpr cstr api_key{ "" };

		template<achievement_e Achievement> static constexpr cstr display_name{ "" };
	}

	template<> inline constexpr cstr achievements::api_key<achievement_e::BasicSkeletonSummoning>{ "basic_skeleton_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::AcquireBoneArmor>{ "acquire_bone_armor" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ChaoticWarpUsage>{ "chaotic_warp_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::PanicWarpUsage>{ "panic_warp_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::PanicWarpDeath>{ "panic_warp_death" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::PreciseWarpUsage>{ "precise_warp_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::PreciseWarpToBones>{ "precise_warp_to_bones" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::PreciseWarpToArmorMinion>{ "precise_warp_to_armor_minion" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::BasicAnnihilateUsage>{ "basic_annihilate_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::ExceptionalAnnihilateUsage>{ "exceptional_annihilate_usage" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::BasicRepulseUsage>{ "basic_repulse_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::ExceptionalRepulseUsage>{ "exceptional_repulse_usage" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::CalcifyWallIntoBones>{ "calcify_wall_into_bones" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CalcifyBonesIntoWall>{ "calcify_bones_into_wall" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CalcifyBonesWithEnemy>{ "calcify_bones_with_enemy" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CalcifyBonesUnderSelf>{ "calcify_bones_under_self" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::BasicIncorporealizeUsage>{ "basic_incorporealize_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::ExceptionalIncorporealizeUsage>{ "exceptional_incorporealize_usage" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::RecorporealizeDeath>{ "recorporealize_death" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::CompleteBasicTutorial>{ "complete_basic_tutorial" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CompleteAdvancedTutorial>{ "complete_advanced_tutorial" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::DescendDownLadder>{ "descend_down_ladder" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ReceiveDivineIntervention>{ "receive_divine_intervention" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::DefeatOverworldSiege>{ "defeat_overworld_siege" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::EnterAncientVault>{ "enter_ancient_vault" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::CollectKharoplasm>{ "collect_kharoplasm" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ReceiveEldritchAudience>{ "receive_eldritch_audience" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::DefeatElbikezzir>{ "defeat_elbikezzir" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::DefeatRebelEncampment>{ "defeat_rebel_encampment" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ReceiveEldritchMonologue>{ "receive_eldritch_monologue" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::DefeatPraethornyn>{ "defeat_praethornyn" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ReceiveImperialAudience>{ "receive_imperial_audience" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::ReturnToRaetun>{ "return_to_raetun" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::CalciticEnshackling>{ "calcitic_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CalciticUnshackling>{ "calcitic_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::SpectralEnshackling>{ "spectral_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::SpectralUnshackling>{ "spectral_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::SanguineEnshackling>{ "sanguine_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::SanguineUnshackling>{ "sanguine_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::GalvanicEnshackling>{ "galvanic_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GalvanicUnshackling>{ "galvanic_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::RavenousEnshackling>{ "ravenous_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::RavenousUnshackling>{ "ravenous_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::WretchedEnshackling>{ "wretched_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::WretchedUnshackling>{ "wretched_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::CerebralEnshackling>{ "cerebral_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CerebralUnshackling>{ "cerebral_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::InfernalEnshackling>{ "infernal_enshackling" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::InfernalUnshackling>{ "infernal_unshackling" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::PatheticSkeletonSummoning>{ "pathetic_skeleton_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserSkeletonSummoning>{ "lesser_skeleton_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterSkeletonSummoning>{ "greater_skeleton_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserBonespurSummoning>{ "lesser_bonespur_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterBonespurSummoning>{ "greater_bonespur_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserCultistSummoning>{ "lesser_cultist_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterCultistSummoning>{ "greater_cultist_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserWraithSummoning>{ "lesser_wraith_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterWraithSummoning>{ "greater_wraith_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserBloodhoundSummoning>{ "lesser_bloodhound_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterBloodhoundSummoning>{ "greater_bloodhound_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserHemogheistSummoning>{ "lesser_hemogheist_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterHemogheistSummoning>{ "greater_hemogheist_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserAnimatedSuitSummoning>{ "lesser_animated_suit_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterAnimatedSuitSummoning>{ "greater_animated_suit_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserDeathKnightSummoning>{ "lesser_death_knight_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterDeathKnightSummoning>{ "greater_death_knight_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::ExaltedDeathKnightSummoning>{ "exalted_death_knight_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserAbominationSummoning>{ "lesser_abomination_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterAbominationSummoning>{ "greater_abomination_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserFleshGolemSummoning>{ "lesser_flesh_golem_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterFleshGolemSummoning>{ "greater_flesh_golem_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserDraugazSummoning>{ "lesser_draugaz_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterDraugazSummoning>{ "greater_draugaz_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserDreadwurmSummoning>{ "lesser_dreadwurm_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterDreadwurmSummoning>{ "greater_dreadwurm_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserHamazSummoning>{ "lesser_hamaz_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterHamazSummoning>{ "greater_hamaz_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::ExaltedHamazSummoning>{ "exalted_hamaz_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserFurtiveHorrorSummoning>{ "lesser_furtive_horror_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterFurtiveHorrorSummoning>{ "greater_furtive_horror_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserChromaleseSummoning>{ "lesser_chromalese_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterChromaleseSummoning>{ "greater_chromalese_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::LesserIsoscelSummoning>{ "lesser_isoscel_summoning" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::GreaterIsoscelSummoning>{ "greater_isoscel_summoning" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::BoneConsumption>{ "bone_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::MetalConsumption>{ "metal_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::FleshConsumption>{ "flesh_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::CerebraConsumption>{ "cerebra_consumption" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::SolidCatalystConsumption>{ "solid_catalyst_consumption" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::BloodConsumption>{ "blood_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::IchorConsumption>{ "ichor_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::FilthConsumption>{ "filth_consumption" };
	template<> inline constexpr cstr achievements::api_key<achievement_e::EctoplasmConsumption>{ "ectoplasm_consumption" };

	template<> inline constexpr cstr achievements::api_key<achievement_e::FluidCatalystConsumption>{ "fluid_catalyst_consumption" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BasicSkeletonSummoning>{ "The Summoning Begins" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::AcquireBoneArmor>{ "Bony Breastplate" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ChaoticWarpUsage>{ "Vanishing Act" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::PanicWarpUsage>{ "Fool's Gambit" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::PanicWarpDeath>{ "Wrong Place, Wrong Time" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::PreciseWarpUsage>{ "Intentionality" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::PreciseWarpToBones>{ "Double Jump" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::PreciseWarpToArmorMinion>{ "Remote Rearmament" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BasicAnnihilateUsage>{ "Path of Destruction" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::ExceptionalAnnihilateUsage>{ "Splashback" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BasicRepulseUsage>{ "Personal Space" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::ExceptionalRepulseUsage>{ "Otherworldly Check" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::CalcifyWallIntoBones>{ "Calcium Deposit" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CalcifyBonesIntoWall>{ "Constructive Influence" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CalcifyBonesWithEnemy>{ "Petrifying!" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CalcifyBonesUnderSelf>{ "The Training Wheels Were Off" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BasicIncorporealizeUsage>{ "Abyssal Adventure" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::ExceptionalIncorporealizeUsage>{ "Toodles!" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::RecorporealizeDeath>{ "Matter Rearrangement" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::CompleteBasicTutorial>{ "A Nascent Necromancer" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CompleteAdvancedTutorial>{ "The Prodigal Pupil" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::DescendDownLadder>{ "And Yet Deeper We Must Delve" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ReceiveDivineIntervention>{ "Destined Death; Defied" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::DefeatOverworldSiege>{ "A Minor Cuncation" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::EnterAncientVault>{ "Once Unbidden; Hence Unsealed" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::CollectKharoplasm>{ "A Perturbing Prize" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ReceiveEldritchAudience>{ "Odious Tidings" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::DefeatElbikezzir>{ "Barotrauma" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::DefeatRebelEncampment>{ "Rebel Razing" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ReceiveEldritchMonologue>{ "Soliloquy of the Damned" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::DefeatPraethornyn>{ "Sandy Slumber" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ReceiveImperialAudience>{ "The Pale Princess" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::ReturnToRaetun>{ "Reunion" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::CalciticEnshackling>{ "Barricade of Bone" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CalciticUnshackling>{ "A Hole Lot of Rattling" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::SpectralEnshackling>{ "Isn't It Incorporeal?" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::SpectralUnshackling>{ "A Chilling Draft" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::SanguineEnshackling>{ "Bloodbindings" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::SanguineUnshackling>{ "Vivisection" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::GalvanicEnshackling>{ "Unbreakable Chains" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GalvanicUnshackling>{ "The Riddle of Steel" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::RavenousEnshackling>{ "Don't Touch It" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::RavenousUnshackling>{ "Raw and Wriggling" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::WretchedEnshackling>{ "Bulbous Barrier" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::WretchedUnshackling>{ "Exotic Stench" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::CerebralEnshackling>{ "Spellbound" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CerebralUnshackling>{ "Free the Mind" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::InfernalEnshackling>{ "Infernal Irreverance" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::InfernalUnshackling>{ "Disquietude Dispelled" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::PatheticSkeletonSummoning>{ "Kind of a Waste" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserSkeletonSummoning>{ "Skeleton Crew" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterSkeletonSummoning>{ "Next Stop: the Bone Zone" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserBonespurSummoning>{ "Right to the Point" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterBonespurSummoning>{ "28 STAB WOUNDS (SELF-INFLICTED)" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserCultistSummoning>{ "Enshrined Depravity" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterCultistSummoning>{ "Sycophantic Symposium" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserWraithSummoning>{ "Intersticial Interloper" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterWraithSummoning>{ "Summoned from Beyond" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserBloodhoundSummoning>{ "Ankle-biter" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterBloodhoundSummoning>{ "The Harrying" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserHemogheistSummoning>{ "Eternal Thirst" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterHemogheistSummoning>{ "Phantom of the Operating Theater" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserAnimatedSuitSummoning>{ "Hollow at Heart" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterAnimatedSuitSummoning>{ "Chosen of the Void" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserDeathKnightSummoning>{ "A Pale Gaze" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterDeathKnightSummoning>{ "Death Incarnate" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::ExaltedDeathKnightSummoning>{ "Malevolent Masterwork" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserAbominationSummoning>{ "A Mob of Meat" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterAbominationSummoning>{ "Abominable Multiplicity" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserFleshGolemSummoning>{ "The Shambling Horror" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterFleshGolemSummoning>{ "Mountain of Flesh" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserDraugazSummoning>{ "Desiccate and Decayed" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterDraugazSummoning>{ "The Grave Legion" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserDreadwurmSummoning>{ "Eldritch Chimera" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterDreadwurmSummoning>{ "Dread Incarnate" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserHamazSummoning>{ "The Mind's Eye" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterHamazSummoning>{ "Refraction of the Soul" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::ExaltedHamazSummoning>{ "Turning the Tide" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserFurtiveHorrorSummoning>{ "Throwing Hands" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterFurtiveHorrorSummoning>{ "Purple People Eater" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserChromaleseSummoning>{ "Living Color" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterChromaleseSummoning>{ "Death Fog" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::LesserIsoscelSummoning>{ "Euclidean Solecism" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::GreaterIsoscelSummoning>{ "Angular Annihilation" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BoneConsumption>{ "Karkaphage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::FleshConsumption>{ "Sarkaphage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::MetalConsumption>{ "Ferrophage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::CerebraConsumption>{ "Myalaphage" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::SolidCatalystConsumption>{ "Sterophage" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::BloodConsumption>{ "Hemophage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::IchorConsumption>{ "Ichophage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::FilthConsumption>{ "Rupaphage" };
	template<> inline constexpr cstr achievements::display_name<achievement_e::EctoplasmConsumption>{ "Ectophage" };

	template<> inline constexpr cstr achievements::display_name<achievement_e::FluidCatalystConsumption>{ "Yrgophage" };

	constexpr cstr to_string(achievement_e type) noexcept;

	constexpr cstr to_api_key(achievement_e type) noexcept;
} // namespace necrowarp
