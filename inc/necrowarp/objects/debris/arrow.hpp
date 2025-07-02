#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<arrow_t> {
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

	template<> inline constexpr glyph_t object_glyphs<arrow_t>{};

	struct arrow_t {
	  public:
	  	static constexpr i8 MinimumArrows{ 1 };
		static constexpr i8 MaximumArrows{ 16 };

		static constexpr i8 StartingArrows{ MinimumArrows };

	  private:
		static inline std::bernoulli_distribution snap_dis{ 0.25 };

		i8 count;

		inline void set_count(i8 amount) noexcept { count = between<i8>(amount, MinimumArrows, MaximumArrows); }

	  public:
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

		offset_t position;

		inline arrow_t(offset_t position) noexcept : count{ StartingArrows }, position{ position } {}
		
		inline arrow_t(offset_t position, i8 amount) noexcept : count{ amount }, position{ position } {}

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

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Arrow, static_cast<u8>(count) }; }

		inline void draw() const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + camera.get_offset(), offset); }

		constexpr operator object_e() const noexcept { return object_e::Arrow; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<arrow_t> arrow) noexcept { return offset_t::std_hasher::operator()(arrow.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<arrow_t> lhs, cref<arrow_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<arrow_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<arrow_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
