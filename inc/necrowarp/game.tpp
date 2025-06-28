#pragma once

#include <necrowarp/dimensions.hpp>

#include <necrowarp/game.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline void game_s::plunge() noexcept {
		terminate_process_turn();

		current_dimension = plunge_target;

		plunge_target = dimension_e::Abyss;
		plunge_flag = false;

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				registry_access.lock();

				constexpr map_type_e map_type{ determine_map<cval>() };

				game_map<map_type>.dependent reset<zone_region_e::All>();
				fluid_map<map_type>.dependent reset<zone_region_e::All>();

				entity_registry<map_type>.dependent reset<ALL_NON_PLAYER>();
				entity_registry<map_type>.dependent reset_goal_map<player_t>();

				entity_registry<map_type>.reset_alignment_goal_maps();

				object_registry<map_type>.reset();

				plunge<cval>();

				entity_registry<map_type>.recalculate_goal_map();
				object_registry<map_type>.recalculate_goal_map();

				phase.transition(phase_e::Playing);

				registry_access.unlock();

				game_running = true;
				process_turn_async<map_type>();
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::plunge_async() noexcept { std::thread([]() -> void { plunge<MapType>(); }).detach(); }

	template<map_type_e MapType> inline void game_s::update() noexcept {
		sine_wave.update<wave_e::Sine>(Clock::elapsed());

		animation_controller.tick<MapType>();

		ui_registry.update<MapType>();

		if (descent_flag) {
			phase.transition(phase_e::Loading);
			phase.previous_phase = phase_e::Loading;

			descend_async<MapType>();
		}

		if (plunge_flag && plunge_target != dimension_e::Abyss && plunge_target != current_dimension) {
			phase.transition(phase_e::Loading);
			phase.previous_phase = phase_e::Loading;

			plunge_async<MapType>();
		}

		if (phase.current_phase == phase_e::Loading && phase.previous_phase != phase_e::Loading) {
			phase.previous_phase = phase_e::Loading;

			load_async<MapType>();
		}

		if (phase.current_phase == phase_e::GameOver && phase.previous_phase != phase_e::GameOver) {
			phase.previous_phase = phase_e::GameOver;

			unload_async<MapType>();
		}

#if !defined(STEAMLESS)
		if (stat_store_timer.ready()) {
			steam_stats::store();

			stat_store_timer.record();
		}
#endif
	}

	inline int game_s::run() noexcept {
		if (!subsystem.is_initialized()) {
			error_log.add("[ERROR]: a subsystem failed to initialize! see error(s) above for details...");

			return EXIT_FAILURE;
		}

		startup();

		do {
			magic_enum::enum_switch([&](auto val) -> void {
				constexpr dimension_e cval{ val };

				if constexpr (is_material<cval>::value) {
					constexpr map_type_e map_type{ determine_map<cval>() };

					input<map_type>();
					update<map_type>();
					render<map_type>();
				}
			}, static_cast<dimension_e>(current_dimension));
		} while (window.is_running());

		shutdown();

		return EXIT_SUCCESS;
	};
} // namespace necrowarp
