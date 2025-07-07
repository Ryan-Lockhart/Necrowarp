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

	constexpr bool use_fixed_seed{ false };

	static inline std::mt19937_64 random_engine{ use_fixed_seed ? default_seed : std::random_device{}() };

	struct api_state_s {
		static constexpr u32 app_id{ 3631430 };
		static inline usize user_id{ 0 };
		static inline i32 pipe_id{ 0 };
	} static inline api_state;

	enum struct attribute_e : u8 {
		Energy,
		Armor,
		Divinity
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

	struct game_stats_t {
		cheats_t cheats{};

		usize game_seed{};

		usize game_depth{ 0 };

		usize wave_size{ static_cast<usize>(globals::StartingWaveSize) };
		usize spawns_remaining{ static_cast<usize>(globals::StartingWaveSize) };

		inline usize has_spawns() const noexcept { return spawns_remaining > 0; }

		i16	player_kills{ 0 };
		i16	minion_kills{ 0 };

		inline i16 total_kills() const noexcept { return player_kills + minion_kills; };

		inline i8 kills_until_next_energy_slot() const noexcept { return abs(minion_kills % globals::KillsPerEnergySlot - globals::KillsPerEnergySlot); }

		inline i8 kills_until_next_armor_slot() const noexcept { return abs(player_kills % globals::KillsPerArmorSlot - globals::KillsPerArmorSlot); }

		inline i8 kills_until_next_divinity_turn() const noexcept { return abs(total_kills() % globals::KillsPerDivinityTurn - globals::KillsPerDivinityTurn); }

		inline i16 current_reinforcements() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerReinforcement, globals::MinimumReinforcements, globals::MaximumReinforcements); }
		
		inline bool has_reinforcements() const noexcept { return current_reinforcements() > 0; }

		inline void reset() noexcept {
			game_seed = std::random_device{}();
			random_engine.seed(game_seed);

			wave_size = globals::StartingWaveSize;
			spawns_remaining = globals::StartingWaveSize;

			player_kills = 0;
			minion_kills = 0;
		}
	};

	enum struct steam_stat_e : u8 {
		PlayerKills = 0,
		MinionKills,

		PlayerDeaths,

		RandomWarps,
		TargetWarps,

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
		InfernalInvocations,
		CerebralInvocations,

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
		MistLadiesSlain,
		BannerBearersSlain,
		ThetwoSlain,
		BattleMonksSlain,
		BerserkersSlain,
		PaladinsSlain,
	};

	template<steam_stat_e Stat> struct to_stat_type;

	template<> struct to_stat_type<steam_stat_e::PlayerKills> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::MinionKills> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::PlayerDeaths> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::RandomWarps> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::TargetWarps> { using type = i32; };

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
	template<> struct to_stat_type<steam_stat_e::InfernalInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::CerebralInvocations> { using type = i32; };

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
	template<> struct to_stat_type<steam_stat_e::MistLadiesSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BannerBearersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::ThetwoSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BattleMonksSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BerserkersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PaladinsSlain> { using type = i32; };

	template<steam_stat_e StatType> using to_stat_type_t = typename to_stat_type<StatType>::type;

	static inline constexpr cstr to_api_str(steam_stat_e type) noexcept {
		switch (type) {
			case steam_stat_e::PlayerKills: {
				return "player_kills";
			} case steam_stat_e::MinionKills: {
				return "minion_kills";
			} case steam_stat_e::PlayerDeaths: {
				return "player_deaths";
			} case steam_stat_e::RandomWarps: {
				return "random_warps";
			} case steam_stat_e::TargetWarps: {
				return "target_warps";
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
			} case steam_stat_e::InfernalInvocations: {
				return "infernal_invocations";
			} case steam_stat_e::CerebralInvocations: {
				return "cerebral_invocations";
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
			} case steam_stat_e::ThetwoSlain: {
				return "thetwo_slain";
			} case steam_stat_e::MistLadiesSlain: {
				return "mist_ladies_slain";
			} case steam_stat_e::BannerBearersSlain: {
				return "banner_bearers_slain";
			} case steam_stat_e::BattleMonksSlain: {
				return "battle_monks_slain";
			} case steam_stat_e::BerserkersSlain: {
				return "berserkers_slain";
			} case steam_stat_e::PaladinsSlain: {
				return "paladins_slain";
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
			} case steam_stat_e::RandomWarps: {
				return "Random Warps";
			} case steam_stat_e::TargetWarps: {
				return "Target Warps";
			} case steam_stat_e::Calcifications: {
				return "Calcifications";
			} case steam_stat_e::Repulsions: {
				return "Repulsions";
			} case steam_stat_e::Annihilations: {
				return "Annihilations";
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
			} case steam_stat_e::InfernalInvocations: {
				return "Infernal Invocations";
			} case steam_stat_e::CerebralInvocations: {
				return "Cerebral Invocations";
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
			} case steam_stat_e::ThetwoSlain: {
				return "Thetwo Slain";
			} case steam_stat_e::MistLadiesSlain: {
				return "Mist Ladies Slain";
			} case steam_stat_e::BannerBearersSlain: {
				return "Banner Bearers Slain";
			} case steam_stat_e::BattleMonksSlain: {
				return "Battle Monks Slain";
			} case steam_stat_e::BerserkersSlain: {
				return "Berserkers Slain";
			} case steam_stat_e::PaladinsSlain: {
				return "Paladins Slain";
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
