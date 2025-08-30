#pragma once

#include <necrowarp/dimensions.hpp>

#include <thread>

#include <necrowarp/game.hpp>
#include <necrowarp/tribulation.hpp>

#include <necrowarp/constants/enums.tpp>

namespace necrowarp {
	template<map_type_e MapType> inline void game_s::load() noexcept {
		terminate_process_turn();

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				constexpr map_type_e map_type{ determine_map<cval>() };

				load<cval>();

				entity_registry<map_type>.recalculate_goal_map();
				object_registry<map_type>.recalculate_goal_map();
		
				entity_registry<map_type>.store();
				object_registry<map_type>.store();
		
				phase.transition(phase_e::Playing);
		
				game_running = true;
				process_turn_async<map_type>();
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::load_async() noexcept { std::thread([]() -> void { load<MapType>(); }).detach(); }

	template<map_type_e MapType> inline void game_s::descend() noexcept {
		terminate_process_turn();

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				constexpr map_type_e map_type{ determine_map<cval>() };

				descend<cval>();

				entity_registry<map_type>.recalculate_goal_map();
				object_registry<map_type>.recalculate_goal_map();
		
				entity_registry<map_type>.store();
				object_registry<map_type>.store();

				update_camera<map_type>(camera<map_type>.get_center());
		
				phase.transition(phase_e::Playing);
		
				game_running = true;
				process_turn_async<map_type>();
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::descend_async() noexcept { std::thread([]() -> void { descend<MapType>(); }).detach(); }

	template<map_type_e MapType> inline void game_s::plunge() noexcept {
		terminate_process_turn();

		const bool trap_triggered{ current_dimension != dimension_e::Tribulation && plunge_target != dimension_e::Tribulation && tribulation_s::aggravate(random_engine) };

		if (trap_triggered) {
			hijacked_target = plunge_target;

			current_dimension = dimension_e::Tribulation;
		} else {
			if (hijacked_target != dimension_e::Abyss) {
				hijacked_target = dimension_e::Abyss;
			}

			current_dimension = plunge_target;
		}

		plunge_target = dimension_e::Abyss;
		plunge_flag = false;

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				constexpr map_type_e map_type{ determine_map<cval>() };

				game_map<map_type>.dependent reset<region_e::All>();
				fluid_map<map_type>.dependent reset<region_e::All>();

				entity_registry<map_type>.dependent clear<ALL_NON_PLAYER>();
				entity_registry<map_type>.dependent reset_goal_map<player_t>();

				entity_registry<map_type>.reset_unique_goal_maps();

				object_registry<map_type>.clear();

				plunge<cval>();

				entity_registry<map_type>.recalculate_goal_map();
				object_registry<map_type>.recalculate_goal_map();

				entity_registry<map_type>.store();
				object_registry<map_type>.store();

				update_camera<map_type>(camera<map_type>.get_center());

				phase.transition(phase_e::Playing);

				game_running = true;
				process_turn_async<map_type>();
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::plunge_async() noexcept { std::thread([]() -> void { plunge<MapType>(); }).detach(); }

	template<map_type_e MapType> inline void game_s::process_turn() noexcept {
		if (window.is_closing() || !game_running || !player_acted || descent_flag || plunge_flag) {
			return;
		}

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				constexpr map_type_e map_type{ determine_map<cval>() };

				buffer_access.lock<true>();

				entity_registry<map_type>.store();
				object_registry<map_type>.store();
		
				buffer_access.unlock();
		
				registry_access.lock<true>();
		
				processing_turn = true;
		
				entity_registry<map_type>.update();

				process_turn<cval>();

				if (fluid_map_dirty) {
					for (cauto position : fluid_map<MapType>.zone_offsets) {
						fluid_map<MapType>[position].recalculate_index(fluid_map<MapType>, position, [](fluid_cell_t value) -> bool { return value != fluid_cell_t{}; });
					}

					fluid_map_dirty = false;
				}

				registry_access.unlock();

				update_camera<map_type>(camera<map_type>.get_center());

				processing_turn = false;

				player_acted = false;
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::process_turn_async() noexcept {
		std::thread([]() -> void {
			do { process_turn<MapType>(); } while (game_running);
		}).detach();
	}

	template<map_type_e MapType> inline void game_s::unload() noexcept {
		terminate_process_turn();

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr dimension_e cval{ val };

			if constexpr (is_material<cval>::value) {
				unload<cval>();
			}
		}, static_cast<dimension_e>(current_dimension));
	}

	template<map_type_e MapType> inline void game_s::unload_async() noexcept { std::thread([]() -> void { unload<MapType>(); }).detach(); }

	template<map_type_e MapType> inline void game_s::update() noexcept {
		sine_wave.update<wave_e::Sine>(Clock::elapsed());
		stealth_wave.update<wave_e::Sine>(Clock::elapsed());

		animation_controller.tick<MapType>();

		slideshow.tick<MapType>();

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
