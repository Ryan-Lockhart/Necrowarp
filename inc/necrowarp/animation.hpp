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

	static inline constexpr usize slideshow_interval{ 1000 };

	struct entity_slideshow_s {
		static constexpr usize num_entities{ EntityTypeCount };

		static constexpr usize interval{ slideshow_interval };

	  private:
		static inline timer_t timer{ interval };

		static inline usize index{ 0 };

		static inline bool suspended{ false };
	  public:
		static inline void suspend() noexcept { suspended = true; }
		static inline void resume() noexcept { suspended = false; }

		static inline bool is_suspended() noexcept { return suspended; }

		static inline entity_e current_entity() noexcept { return static_cast<entity_e>(index % num_entities); }

		template<map_type_e MapType> static inline void tick() noexcept {
			if (!timer.ready()) {
				return;
			}

			timer.record();

			++index;
		}
	} static inline entity_slideshow;

	struct object_slideshow_s {
		static constexpr usize num_objects{ ObjectTypeCount };

		static constexpr usize interval{ slideshow_interval };

	  private:
		static inline timer_t timer{ interval };

		static inline usize index{ 1 };

		static inline bool suspended{ false };
	  public:
		static inline void suspend() noexcept { suspended = true; }
		static inline void resume() noexcept { suspended = false; }

		static inline bool is_suspended() noexcept { return suspended; }

		static inline object_e current_object() noexcept { return static_cast<object_e>(index % num_objects); }

		template<map_type_e MapType> static inline void tick() noexcept {
			if (!timer.ready()) {
				return;
			}

			timer.record();

			++index;
		}
	} static inline object_slideshow;
} //namespace necrowarp
