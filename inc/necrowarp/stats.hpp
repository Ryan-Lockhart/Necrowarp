#pragma once

#include <bleak.hpp>

#include <random>

#include <necrowarp/cell.hpp>
#include <necrowarp/globals.hpp>
#include <necrowarp/phase.hpp>
#include <necrowarp/dimensions/dimension.hpp>

#include <magic_enum/magic_enum_utility.hpp>

#include <necrowarp/constants/enums/steam/achievements.tpp>
#include <necrowarp/constants/enums/steam/stats.tpp>

namespace necrowarp {
	using namespace bleak;

	constexpr usize default_seed{ 1337 };

	constexpr bool use_fixed_seed{ !IsReleaseBuild };

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

		inline void enable_all() noexcept {
			no_hit = true;
			free_costs = true;
			bypass_invocations = true;

			activate();
		}

		inline void disable_all() noexcept {
			no_hit = false;
			free_costs = false;
			bypass_invocations = false;

			deactivate();
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

		f16 liters_of_blood{ 0.0f };
		f16 liters_of_ichor{ 0.0f };
		f16 liters_of_filth{ 0.0f };
		f16 liters_of_ectoplasm{ 0.0f };

		inline f16 liters_of_fluid() const noexcept { return liters_of_blood + liters_of_ichor + liters_of_filth + liters_of_ectoplasm; }

		i16 piles_of_bone{ 0 };
		i16 piles_of_flesh{ 0 };
		i16 piles_of_metal{ 0 };
		i16 piles_of_cerebra{ 0 };

		inline i16 piles_of_catalyst() const noexcept { return piles_of_bone + piles_of_flesh + piles_of_metal + piles_of_cerebra; }

		inline i16 total_kills() const noexcept { return player_kills + minion_kills; };

		inline void update_wave_size() noexcept { wave_size = clamp(static_cast<i16>(StartingWaveSize + total_kills() / globals::KillsPerPopulation), MinimumWaveSize, MaximumWaveSize); }

		inline i8 kills_until_next_energy_slot() const noexcept { return abs(minion_kills % globals::KillsPerEnergySlot - globals::KillsPerEnergySlot); }

		inline i8 kills_until_next_armor_slot() const noexcept { return abs(player_kills % globals::KillsPerArmorSlot - globals::KillsPerArmorSlot); }

		inline i8 kills_until_next_divinity_turn() const noexcept { return abs(total_kills() % globals::KillsPerDivinityTurn - globals::KillsPerDivinityTurn); }
		
		inline i8 kills_until_next_phantasm_turn() const noexcept { return abs(total_kills() % globals::KillsPerPhantasmTurn - globals::KillsPerPhantasmTurn); }

		inline i16 current_reinforcements() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerReinforcement, globals::MinimumReinforcements, globals::MaximumReinforcements); }

		inline i16 current_departure_epoch() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerDepartureEpoch, globals::MinimumDepartureEpoch, globals::MaximumDepartureEpoch); }

		inline i16 current_gateways() const noexcept { return clamp<i16>((game_depth / globals::FloorsPerGateway) + (total_kills() / globals::KillsPerGateway), globals::MinimumGateways, globals::MaximumGateways); }
		
		inline bool has_reinforcements() const noexcept { return current_reinforcements() > 0; }

		inline void reset() noexcept {
			game_depth = 0;

			game_seed = use_fixed_seed ? default_seed : std::random_device{}();

			random_engine.seed(game_seed);
			map_engine.seed(game_seed);

			wave_size = StartingWaveSize;
			spawns_remaining = StartingWaveSize;

			liters_of_blood = 0.0f;
			liters_of_ichor = 0.0f;
			liters_of_filth = 0.0f;
			liters_of_ectoplasm = 0.0f;

			piles_of_bone = 0;
			piles_of_flesh = 0;
			piles_of_metal = 0;
			piles_of_cerebra = 0;

			player_kills = 0;
			minion_kills = 0;
		}
	};

	template<stat_e Type, typename T>
		requires is_one_of<T, i32, f32>::value
	struct steam_stat_t {
		using value_type = T;

		static inline constexpr stat_e type{ Type };

		static inline value_type cached_value;
		static inline value_type initial_value;

		inline T get_value() const noexcept  {
			if constexpr (IsSteamless) {
				return T{};
			}

			std::optional<T> maybe_value{ steam::user_stats::get_stat<T>(stats::api_key<Type>) };

			if (!maybe_value.has_value()) {
				error_log.add("[WARNING]: returning cached value of \"{}\"...", stats::api_key<Type>);

				return cached_value;
			}

			return cached_value = maybe_value.value();
		}

		inline bool set_value(value_type value) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return steam::user_stats::set_stat(stats::api_key<Type>, value);
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

		template<stat_e Type> static inline steam_stat_t<Type, typename to_stat_type<Type>::type> stats{};
		
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

		static inline bool is_unlocked(achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			bool unlocked{ false };

			return user_stats()->GetAchievement(to_api_key(achievement), &unlocked) && unlocked;
		}

		static inline bool reset(achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return user_stats()->ClearAchievement(to_api_key(achievement));
		}

		static inline void reset() noexcept {
			if constexpr (IsSteamless) {
				return;
			}

			magic_enum::enum_for_each<achievement_e>([&](auto val) {
				constexpr achievement_e cval{ val };

				user_stats()->ClearAchievement(achievements::api_key<cval>);
			});

			store();
		}

		static inline bool unlock(achievement_e achievement) noexcept {
			if constexpr (IsSteamless) {
				return false;
			}

			return user_stats()->SetAchievement(to_api_key(achievement));
		}

		static inline void unlock() noexcept {
			if constexpr (IsSteamless) {
				return;
			}

			magic_enum::enum_for_each<achievement_e>([&](auto val) {
				constexpr achievement_e cval{ val };

				user_stats()->SetAchievement(achievements::api_key<cval>);
			});

			store();
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

			magic_enum::enum_for_each<stat_e>([] (auto val) -> void {
				constexpr stat_e Stat{ val };
				
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
