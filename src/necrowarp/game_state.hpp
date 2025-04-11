#pragma once

#include "bleak/concepts.hpp"
#include <bleak.hpp>

#include <atomic>
#include <random>

#include <necrowarp/globals.hpp>
#include <necrowarp/phase.hpp>

#include <magic_enum/magic_enum_utility.hpp>

namespace necrowarp {
	using namespace bleak;

	struct api_state_s {
		static constexpr u32 app_id{ 3631430 };
		static inline usize user_id{ 0 };
		static inline i32 pipe_id{ 0 };
	} static inline api_state;

	static inline subsystem_s subsystem{};

	static inline phase_t phase{};

	static inline window_t window{ globals::GameName.c_str(), globals::window_size, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res/glyphs/glyphs_8x8.png" };
	static inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res/tiles/tileset_16x16.png" };
	static inline atlas_t<globals::IconsetSize> icon_atlas{ renderer, "res/icons/icons_32x32.png" };

	static inline std::mt19937 random_engine{};

	static inline zone_t<cell_state_t, globals::MapSize, globals::BorderSize> game_map{};

	static inline cursor_t cursor{ renderer, "res/sprites/cursor.png", colors::White };

	static inline grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res/sprites/grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };
	static inline grid_cursor_t<globals::CellSize> warp_cursor{ renderer, "res/sprites/grid_cursor.png", colors::Magenta, game_map.zone_origin, game_map.zone_extent };

	static inline bool draw_cursor{ true };
	static inline bool draw_warp_cursor{ false };

	static inline camera_t camera{ globals::game_grid_size(), extent_t::Zero, globals::camera_extent() };

	static inline bool camera_locked{ true };

	static inline bool camera_forced() noexcept { return globals::MapSize.w <= globals::game_grid_size().w || globals::MapSize.h <= globals::game_grid_size().h; }

	static inline bleak::timer_t input_timer{ 125.0 };
	static inline bleak::timer_t cursor_timer{ 125.0 };
	static inline bleak::timer_t epoch_timer{ 250.0 };
	static inline bleak::timer_t stat_store_timer{ 30000.0 };

	static inline wave_t sine_wave{ 1.0, 0.5, 1.0 };

	static inline mixer_s mixer{};
	
	static inline volatile std::atomic_bool player_acted{ false };
	static inline volatile std::atomic_bool processing_turn{ false };

	static inline volatile std::atomic_bool game_running{ false };

	static inline f32 blood_pool_volume() noexcept { return globals::blood_pool_dis(random_engine); }

	enum class attribute_e {
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
		bool is_enabled() const noexcept { return enabled; };

		i16 current_minimum() const noexcept { return minimum; };
		i16 current_maximum() const noexcept { return maximum; };

		void enable(i16 min, i16 max) noexcept {
			if (is_enabled()) {
				return;
			}

			minimum = min;
			maximum = max;

			enabled = true;
		};

		void disable() noexcept {
			if (!is_enabled()) {
				return;
			}

			enabled = false;

			minimum = 0;
			maximum = 0;
		};

		void set(i16 min, i16 max) noexcept {
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

		bool is_enabled() const noexcept { return globals::CheatsAllowed && enabled; }

		void activate() noexcept {
			if (!globals::CheatsAllowed) {
				return;
			}

			enabled = true;
		}

		void deactivate() noexcept {
			enabled = false;
		}
	};

	struct game_stats_t {
		cheats_t cheats{};

		usize game_seed{};

		usize game_depth{ 0 };

		usize wave_size{ static_cast<usize>(globals::map_config.starting_adventurers) };
		usize spawns_remaining{ static_cast<usize>(globals::map_config.starting_adventurers) };

		i16	player_kills{ 0 };
		i16	minion_kills{ 0 };

		i16	total_kills() const noexcept { return player_kills + minion_kills; };

		i16 current_reinforcements() const noexcept { return clamp<i16>(game_depth / globals::FloorsPerReinforcement, globals::MinimumReinforcements, globals::MaximumReinforcements); }
		
		bool has_reinforcements() const noexcept { return current_reinforcements() > 0; }

		void reset() noexcept {
			game_seed = std::random_device{}();
			random_engine.seed(game_seed);

			wave_size = globals::map_config.starting_adventurers;
			spawns_remaining = globals::map_config.starting_adventurers;

			player_kills = 0;
			minion_kills = 0;
		}
	} static inline game_stats{};

	enum class steam_stat_e : u8 {
		PlayerKills = 0,
		PlayerDeaths,

		RandomWarps,
		TargetWarps,

		CalciticInvocations,
		SpectralInvocations,
		SanguineInvocations,
		NecromanticAscensions,

		MetersMoved,
		MetersWarped,
		LowestDepth,

		SkullsConsumed,
		BloodConsumed,
		SkeletonsConsumed,
		WraithsConsumed,
		FleshGolemsConsumed,

		LaddersUnshackled,
		LaddersShackled,

		AdventurersSlain,
		PaladinsSlain,
		PriestsSlain,

		AdventurersResurrected,
		PaladinsOrdained,
		PriestsDepleted,

		MinionKills,

		SkullsExorcised,
	};

	template<steam_stat_e Stat> struct to_stat_type;

	template<> struct to_stat_type<steam_stat_e::PlayerKills> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PlayerDeaths> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::RandomWarps> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::TargetWarps> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::CalciticInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::SpectralInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::SanguineInvocations> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::NecromanticAscensions> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::MetersMoved> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::MetersWarped> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::LowestDepth> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::SkullsConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::BloodConsumed> { using type = f32; };
	template<> struct to_stat_type<steam_stat_e::SkeletonsConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::WraithsConsumed> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::FleshGolemsConsumed> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::LaddersUnshackled> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::LaddersShackled> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::AdventurersSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PaladinsSlain> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PriestsSlain> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::AdventurersResurrected> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PaladinsOrdained> { using type = i32; };
	template<> struct to_stat_type<steam_stat_e::PriestsDepleted> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::MinionKills> { using type = i32; };

	template<> struct to_stat_type<steam_stat_e::SkullsExorcised> { using type = i32; };

	template<steam_stat_e StatType> using to_stat_type_t = typename to_stat_type<StatType>::type;

	static inline constexpr cstr to_api_str(steam_stat_e type) noexcept {
		switch (type) {
			case steam_stat_e::PlayerKills: {
				return "player_kills";
			} case steam_stat_e::PlayerDeaths: {
				return "player_deaths";
			} case steam_stat_e::RandomWarps: {
				return "random_warps";
			} case steam_stat_e::TargetWarps: {
				return "target_warps";
			} case steam_stat_e::CalciticInvocations: {
				return "calcitic_invocations";
			} case steam_stat_e::SpectralInvocations: {
				return "spectral_invocations";
			} case steam_stat_e::SanguineInvocations: {
				return "sanguine_invocations";
			} case steam_stat_e::NecromanticAscensions: {
				return "necromantic_ascensions";
			} case steam_stat_e::MetersMoved: {
				return "meters_moved";
			} case steam_stat_e::MetersWarped: {
				return "meters_warped";
			} case steam_stat_e::LowestDepth: {
				return "lowest_depth";
			} case steam_stat_e::SkullsConsumed: {
				return "skulls_consumed";
			} case steam_stat_e::BloodConsumed: {
				return "blood_consumed";
			} case steam_stat_e::SkeletonsConsumed: {
				return "skeletons_consumed";
			} case steam_stat_e::WraithsConsumed: {
				return "wraiths_consumed";
			} case steam_stat_e::FleshGolemsConsumed: {
				return "flesh_golems_consumed";
			} case steam_stat_e::LaddersUnshackled: {
				return "ladders_unshackled";
			} case steam_stat_e::LaddersShackled: {
				return "ladders_shackled";
			} case steam_stat_e::AdventurersSlain: {
				return "adventurers_slain";
			} case steam_stat_e::PaladinsSlain: {
				return "paladins_slain";
			} case steam_stat_e::PriestsSlain: {
				return "priests_slain";
			} case steam_stat_e::AdventurersResurrected: {
				return "adventurers_resurrected";
			} case steam_stat_e::PaladinsOrdained: {
				return "paladins_ordained";
			} case steam_stat_e::PriestsDepleted: {
				return "priests_depleted";
			} case steam_stat_e::MinionKills: {
				return "minion_kills";
			} case steam_stat_e::SkullsExorcised: {
				return "skulls_exorcised";
			} default: {
				return "unknown";
			}
		}
	}

	static inline constexpr cstr to_display_str(steam_stat_e type) noexcept {
		switch (type) {
			case steam_stat_e::PlayerKills: {
				return "Player Kills";
			} case steam_stat_e::PlayerDeaths: {
				return "Player Deaths";
			} case steam_stat_e::RandomWarps: {
				return "Random Warps";
			} case steam_stat_e::TargetWarps: {
				return "Target Warps";
			} case steam_stat_e::CalciticInvocations: {
				return "Calcitic Invocations";
			} case steam_stat_e::SpectralInvocations: {
				return "Spectral Invocations";
			} case steam_stat_e::SanguineInvocations: {
				return "Sanguine Invocations";
			} case steam_stat_e::NecromanticAscensions: {
				return "Necromantic Ascensions";
			} case steam_stat_e::MetersMoved: {
				return "Meters Moved";
			} case steam_stat_e::MetersWarped: {
				return "Meters Warped";
			} case steam_stat_e::LowestDepth: {
				return "Lowest Depth";
			} case steam_stat_e::SkullsConsumed: {
				return "Skulls Consumed";
			} case steam_stat_e::BloodConsumed: {
				return "Blood Consumed";
			} case steam_stat_e::SkeletonsConsumed: {
				return "Skeletons Consumed";
			} case steam_stat_e::WraithsConsumed: {
				return "Wraiths Consumed";
			} case steam_stat_e::FleshGolemsConsumed: {
				return "Flesh Golems Consumed";
			} case steam_stat_e::LaddersUnshackled: {
				return "Ladders Unshackled";
			} case steam_stat_e::LaddersShackled: {
				return "Ladders Shackled";
			} case steam_stat_e::AdventurersSlain: {
				return "Adventurers Slain";
			} case steam_stat_e::PaladinsSlain: {
				return "Paladins Slain";
			} case steam_stat_e::PriestsSlain: {
				return "Priests Slain";
			} case steam_stat_e::AdventurersResurrected: {
				return "Adventurers Resurrected";
			} case steam_stat_e::PaladinsOrdained: {
				return "Paladins Ordained";
			} case steam_stat_e::PriestsDepleted: {
				return "Priests Depleted";
			} case steam_stat_e::MinionKills: {
				return "Minion Kills";
			} case steam_stat_e::SkullsExorcised: {
				return "Skulls Exorcised";
			} default: {
				return "Unknown";
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
		
		inline value_type get_value() const noexcept;

		inline bool set_value(value_type value) noexcept;

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

		template<steam_stat_e Type, typename T>
			requires (std::is_same<T, i32>::value || std::is_same<T, f32>::value)
		static inline steam_stat_t<Type, T> stats{};
		
		static inline ptr<ISteamUserStats> user_stats() {
			ptr<ISteamUserStats> interface = SteamUserStats();

			if (interface == nullptr) {
				error_log.add("[ERROR]: unable to access steam user stats api!");
			}

			return interface;
		}

		static inline bool store() noexcept { return user_stats()->StoreStats(); }

		static inline void transcribe() noexcept {
			magic_enum::enum_for_each<steam_stat_e>([] (auto val) {
				constexpr steam_stat_e Stat{ val };

				using Type = to_stat_type<Stat>::type;
				
				stats<Stat, Type>.initial_value = stats<Stat, Type>.get_value();
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
	
	template<steam_stat_e Type, typename T>
		requires is_one_of<T, i32, f32>::value
	inline T steam_stat_t<Type, T>::get_value() const noexcept  {
		std::optional<T> maybe_value{ steam::user_stats::get_stat<T>(api_name) };

		if (!maybe_value.has_value()) {
			error_log.add("[WARNING]: returning cached value of \"{}\"...", api_name);

			return cached_value;
		}

		return cached_value = maybe_value.value();
	}
	
	template<steam_stat_e Type, typename T> requires is_one_of<T, i32, f32>::value inline bool steam_stat_t<Type, T>::set_value(value_type value) noexcept { return steam::user_stats::set_stat(api_name, value); }
} // namespace necrowarp
