#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<arrow_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<arrow_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<arrow_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<arrow_t> {
		static constexpr object_e value = object_e::Arrow;
	};

	template<> struct is_object_type<arrow_t, object_e::Arrow> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Arrow> {
		using type = arrow_t;
	};

	template<> struct to_object_group<object_e::Arrow> {
		static constexpr object_group_e value = object_group_e::Arrow;
	};

	struct arrow_t {
	  public:
	  	static constexpr i8 MinimumArrows{ 1 };
		static constexpr i8 MaximumArrows{ 16 };

		static constexpr i8 StartingArrows{ MinimumArrows };

	  private:
		static inline std::bernoulli_distribution snap_dis{ 0.25 };

		i8 count;

		inline void set_count(i8 amount) noexcept { count = clamp<i8>(amount, MinimumArrows, MaximumArrows); }

	  public:
		inline arrow_t() noexcept : count{ StartingArrows } {}
		
		inline arrow_t(i8 amount) noexcept : count{ amount } {}

		template<RandomEngine Generator> static inline bool snap(ref<Generator> generator) noexcept { return snap_dis(generator); }

		inline i8 stack_size() const noexcept { return count; }

		inline bool has_singular() const noexcept { return count == 1; }

		inline bool has_multiple() const noexcept { return count > 1; }

		inline bool is_full() const noexcept { return count >= MaximumArrows; }

		inline ref<arrow_t> operator+=(i8 amount) noexcept {
			set_count(count + amount);

			return *this;
		}

		inline ref<arrow_t> operator-=(i8 amount) noexcept {
			set_count(count - amount);

			return *this;
		}

		inline ref<arrow_t> operator++() noexcept {
			set_count(count + 1);

			return *this;
		}

		inline ref<arrow_t> operator--() noexcept {
			set_count(count - 1);

			return *this;
		}

		inline std::string to_string() const noexcept {
			return std::format("{}{}",
				has_singular() ?
					necrowarp::to_string<plurality_e::Singular>(object_e::Arrow) :
					necrowarp::to_string<plurality_e::Multiple>(object_e::Arrow),
				has_multiple() ?
					std::format(" [{}]", static_cast<int>(count)) : ""
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ has_singular() ?
				necrowarp::to_colored_string<plurality_e::Singular>(object_e::Arrow) :
				necrowarp::to_colored_string<plurality_e::Multiple>(object_e::Arrow)
			};

			if (has_multiple()) {
				colored_string.concatenate(runes_t{ std::format(" [{}]", static_cast<int>(count)) });
			}

			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Arrow, static_cast<u8>(count - 1) }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Arrow; }
	};
} // namespace necrowarp
