#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

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

	template<> inline constexpr glyph_t object_glyphs<arrow_t>{ glyphs::Arrow };

	struct arrow_t {
	  private:
		static inline std::bernoulli_distribution snap_dis{ 0.25 };

	  public:
		template<RandomEngine Generator> static inline bool snap(ref<Generator> generator) noexcept { return snap_dis(generator); }

		offset_t position;

		inline arrow_t(offset_t position) noexcept : position{ position } {}

		inline void draw() const noexcept { game_atlas.draw(object_glyphs<arrow_t>, position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(object_glyphs<arrow_t>, position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(object_glyphs<arrow_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(object_glyphs<arrow_t>, position + camera.get_offset(), offset); }

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
