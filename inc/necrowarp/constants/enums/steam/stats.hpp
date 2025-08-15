#pragma once

#include <necrowarp/constants/enums/base.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct stat_e : u8 {
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

	template<stat_e Stat> struct to_stat_type;

	template<> struct to_stat_type<stat_e::PlayerKills> { using type = i32; };
	template<> struct to_stat_type<stat_e::MinionKills> { using type = i32; };

	template<> struct to_stat_type<stat_e::PlayerDeaths> { using type = i32; };

	template<> struct to_stat_type<stat_e::ChaoticWarps> { using type = i32; };
	template<> struct to_stat_type<stat_e::PreciseWarps> { using type = i32; };

	template<> struct to_stat_type<stat_e::Calcifications> { using type = i32; };
	template<> struct to_stat_type<stat_e::Repulsions> { using type = i32; };
	template<> struct to_stat_type<stat_e::Annihilations> { using type = i32; };
	template<> struct to_stat_type<stat_e::Incorporealizations> { using type = i32; };

	template<> struct to_stat_type<stat_e::CalciticInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::SpectralInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::SanguineInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::GalvanicInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::RavenousInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::WretchedInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::CerebralInvocations> { using type = i32; };
	template<> struct to_stat_type<stat_e::InfernalInvocations> { using type = i32; };

	template<> struct to_stat_type<stat_e::NecromanticAscensions> { using type = i32; };
	template<> struct to_stat_type<stat_e::CalamitousRetaliations> { using type = i32; };

	template<> struct to_stat_type<stat_e::MetersMoved> { using type = f32; };
	template<> struct to_stat_type<stat_e::MetersWarped> { using type = f32; };
	template<> struct to_stat_type<stat_e::LowestDepth> { using type = i32; };
	template<> struct to_stat_type<stat_e::PortalsTraversed> { using type = i32; };

	template<> struct to_stat_type<stat_e::BonesConsumed> { using type = i32; };
	template<> struct to_stat_type<stat_e::FleshConsumed> { using type = i32; };
	template<> struct to_stat_type<stat_e::MetalConsumed> { using type = i32; };
	template<> struct to_stat_type<stat_e::CerebraConsumed> { using type = i32; };

	template<> struct to_stat_type<stat_e::BloodConsumed> { using type = f32; };
	template<> struct to_stat_type<stat_e::IchorConsumed> { using type = f32; };
	template<> struct to_stat_type<stat_e::FilthConsumed> { using type = f32; };
	template<> struct to_stat_type<stat_e::EctoplasmConsumed> { using type = f32; };

	template<> struct to_stat_type<stat_e::SkeletonsConsumed> { using type = i32; };
	template<> struct to_stat_type<stat_e::BonespursConsumed> { using type = i32; };

	template<> struct to_stat_type<stat_e::GrimoiresCollected> { using type = i32; };

	template<> struct to_stat_type<stat_e::LaddersUnshackled> { using type = i32; };
	template<> struct to_stat_type<stat_e::LaddersShackled> { using type = i32; };

	template<> struct to_stat_type<stat_e::AdventurersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::MercenariesSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::RangersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::SkulkersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::ManslingsSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::MistLadiesSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::BannerBearersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::MediciiSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::BattleMonksSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::BerserkersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::HexeatersSlain> { using type = i32; };
	template<> struct to_stat_type<stat_e::PaladinsSlain> { using type = i32; };

	template<> struct to_stat_type<stat_e::ThetwoSlain> { using type = i32; };

	template<stat_e StatType> using to_stat_type_t = typename to_stat_type<StatType>::type;

	namespace stats {
		template<stat_e Stat> static constexpr cstr api_key{ "" };
		template<stat_e Stat> static constexpr cstr display_name{ "" };
	}

	template<> inline constexpr cstr stats::api_key<stat_e::PlayerKills>{ "player_kills" };
	template<> inline constexpr cstr stats::api_key<stat_e::MinionKills>{ "minion_kills" };

	template<> inline constexpr cstr stats::api_key<stat_e::PlayerDeaths>{ "player_deaths" };

	template<> inline constexpr cstr stats::api_key<stat_e::ChaoticWarps>{ "chaotic_warps" };
	template<> inline constexpr cstr stats::api_key<stat_e::PreciseWarps>{ "precise_warps" };

	template<> inline constexpr cstr stats::api_key<stat_e::Calcifications>{ "calcifications" };
	template<> inline constexpr cstr stats::api_key<stat_e::Repulsions>{ "repulsions" };

	template<> inline constexpr cstr stats::api_key<stat_e::Annihilations>{ "annihilations" };
	template<> inline constexpr cstr stats::api_key<stat_e::Incorporealizations>{ "incorporealizations" };

	template<> inline constexpr cstr stats::api_key<stat_e::CalciticInvocations>{ "calcitic_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::SpectralInvocations>{ "spectral_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::SanguineInvocations>{ "sanguine_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::GalvanicInvocations>{ "galvanic_invocations" };

	template<> inline constexpr cstr stats::api_key<stat_e::RavenousInvocations>{ "ravenous_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::WretchedInvocations>{ "wretched_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::CerebralInvocations>{ "cerebral_invocations" };
	template<> inline constexpr cstr stats::api_key<stat_e::InfernalInvocations>{ "infernal_invocations" };

	template<> inline constexpr cstr stats::api_key<stat_e::NecromanticAscensions>{ "necromantic_ascensions" };
	template<> inline constexpr cstr stats::api_key<stat_e::CalamitousRetaliations>{ "calamitous_retaliations" };

	template<> inline constexpr cstr stats::api_key<stat_e::MetersMoved>{ "meters_moved" };
	template<> inline constexpr cstr stats::api_key<stat_e::MetersWarped>{ "meters_warped" };

	template<> inline constexpr cstr stats::api_key<stat_e::LowestDepth>{ "lowest_depth" };

	template<> inline constexpr cstr stats::api_key<stat_e::PortalsTraversed>{ "portals_traversed" };

	template<> inline constexpr cstr stats::api_key<stat_e::BonesConsumed>{ "bones_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::FleshConsumed>{ "flesh_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::MetalConsumed>{ "metal_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::CerebraConsumed>{ "cerebra_consumed" };

	template<> inline constexpr cstr stats::api_key<stat_e::BloodConsumed>{ "blood_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::IchorConsumed>{ "ichor_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::FilthConsumed>{ "flith_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::EctoplasmConsumed>{ "ectoplasm_consumed" };

	template<> inline constexpr cstr stats::api_key<stat_e::SkeletonsConsumed>{ "skeletons_consumed" };
	template<> inline constexpr cstr stats::api_key<stat_e::BonespursConsumed>{ "bonespurs_consumed" };

	template<> inline constexpr cstr stats::api_key<stat_e::GrimoiresCollected>{ "grimoires_collected" };

	template<> inline constexpr cstr stats::api_key<stat_e::LaddersUnshackled>{ "ladders_unshackled" };
	template<> inline constexpr cstr stats::api_key<stat_e::LaddersShackled>{ "ladders_shackled" };

	template<> inline constexpr cstr stats::api_key<stat_e::AdventurersSlain>{ "adventurers_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::MercenariesSlain>{ "mercenaries_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::RangersSlain>{ "rangers_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::SkulkersSlain>{ "skulkers_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::ManslingsSlain>{ "manslings_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::MistLadiesSlain>{ "mist_ladies_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::BannerBearersSlain>{ "banner_bearers_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::MediciiSlain>{ "medicii_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::BattleMonksSlain>{ "battle_monks_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::BerserkersSlain>{ "berserkers_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::HexeatersSlain>{ "hexeaters_slain" };
	template<> inline constexpr cstr stats::api_key<stat_e::PaladinsSlain>{ "paladins_slain" };

	template<> inline constexpr cstr stats::api_key<stat_e::ThetwoSlain>{ "thetwo_slain" };

	template<> inline constexpr cstr stats::display_name<stat_e::PlayerKills>{ "Player Kills" };
	template<> inline constexpr cstr stats::display_name<stat_e::MinionKills>{ "Minion Kills" };

	template<> inline constexpr cstr stats::display_name<stat_e::PlayerDeaths>{ "Player Deaths" };

	template<> inline constexpr cstr stats::display_name<stat_e::ChaoticWarps>{ "Chaotic Warps" };
	template<> inline constexpr cstr stats::display_name<stat_e::PreciseWarps>{ "Precise Warps" };

	template<> inline constexpr cstr stats::display_name<stat_e::Annihilations>{ "Annihilations" };
	template<> inline constexpr cstr stats::display_name<stat_e::Repulsions>{ "Repulsions" };

	template<> inline constexpr cstr stats::display_name<stat_e::Calcifications>{ "Calcifications" };
	template<> inline constexpr cstr stats::display_name<stat_e::Incorporealizations>{ "Incorporealizations" };

	template<> inline constexpr cstr stats::display_name<stat_e::CalciticInvocations>{ "Calcitic Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::SpectralInvocations>{ "Spectral Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::SanguineInvocations>{ "Sanguine Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::GalvanicInvocations>{ "Galvanic Invocations" };

	template<> inline constexpr cstr stats::display_name<stat_e::RavenousInvocations>{ "Ravenous Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::WretchedInvocations>{ "Wretched Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::CerebralInvocations>{ "Cerebral Invocations" };
	template<> inline constexpr cstr stats::display_name<stat_e::InfernalInvocations>{ "Infernal Invocations" };

	template<> inline constexpr cstr stats::display_name<stat_e::NecromanticAscensions>{ "Necromantic Ascensions" };
	template<> inline constexpr cstr stats::display_name<stat_e::CalamitousRetaliations>{ "Calamitous Retaliations" };

	template<> inline constexpr cstr stats::display_name<stat_e::MetersMoved>{ "Meters Moved" };
	template<> inline constexpr cstr stats::display_name<stat_e::MetersWarped>{ "Meters Warped" };

	template<> inline constexpr cstr stats::display_name<stat_e::LowestDepth>{ "Lowest Depth" };

	template<> inline constexpr cstr stats::display_name<stat_e::PortalsTraversed>{ "Portals Traversed" };

	template<> inline constexpr cstr stats::display_name<stat_e::BonesConsumed>{ "Bones Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::FleshConsumed>{ "Flesh Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::MetalConsumed>{ "Metal Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::CerebraConsumed>{ "Cerebra Consumed" };

	template<> inline constexpr cstr stats::display_name<stat_e::BloodConsumed>{ "Blood Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::IchorConsumed>{ "Ichor Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::FilthConsumed>{ "Filth Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::EctoplasmConsumed>{ "Ectoplasm Consumed" };

	template<> inline constexpr cstr stats::display_name<stat_e::SkeletonsConsumed>{ "Skeletons Consumed" };
	template<> inline constexpr cstr stats::display_name<stat_e::BonespursConsumed>{ "Bonespurs Consumed" };

	template<> inline constexpr cstr stats::display_name<stat_e::GrimoiresCollected>{ "Grimoires Collected" };

	template<> inline constexpr cstr stats::display_name<stat_e::LaddersUnshackled>{ "Ladders Unshackled" };
	template<> inline constexpr cstr stats::display_name<stat_e::LaddersShackled>{ "Ladders Shackled" };

	template<> inline constexpr cstr stats::display_name<stat_e::AdventurersSlain>{ "Adventurers Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::MercenariesSlain>{ "Mercenaries Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::RangersSlain>{ "Rangers Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::SkulkersSlain>{ "Skulkers Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::ManslingsSlain>{ "Manslings Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::MistLadiesSlain>{ "Mist Ladies Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::BannerBearersSlain>{ "Banner Bearers Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::MediciiSlain>{ "Medicii Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::BattleMonksSlain>{ "Battle Monks Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::BerserkersSlain>{ "Berserkers Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::HexeatersSlain>{ "Hexeaters Slain" };
	template<> inline constexpr cstr stats::display_name<stat_e::PaladinsSlain>{ "Paladins Slain" };

	template<> inline constexpr cstr stats::display_name<stat_e::ThetwoSlain>{ "Thetwo Slain" };

	constexpr cstr to_string(stat_e type) noexcept;

	constexpr cstr to_api_key(stat_e type) noexcept;
} // namespace necrowarp
