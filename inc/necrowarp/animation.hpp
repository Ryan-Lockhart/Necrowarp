#pragma once

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	struct animation_controller_s {
		static constexpr usize target_fps{ 12 };

		static constexpr usize interval{ static_cast<usize>(1000.0 / target_fps) };

	  private:
		static inline timer_t timer{ interval };

		static inline bool suspended{ false };
	  public:
		static inline void suspend() noexcept { suspended = true; }
		static inline void resume() noexcept { suspended = false; }

		static inline bool is_suspended() noexcept { return suspended; }

		template<map_type_e MapType> static inline void tick() noexcept {
			if (!timer.ready()) {
				return;
			}

			timer.record();

			entity_registry<MapType>.advance();
			object_registry<MapType>.advance();
		}
	} static inline animation_controller;
} //namespace necrowarp
