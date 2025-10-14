#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/bloomage.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<flora_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<flora_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<flora_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<flora_t> {
		static constexpr object_e value = object_e::Flora;
	};

	template<> struct is_object_type<flora_t, object_e::Flora> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Flora> {
		using type = flora_t;
	};

	template<> struct to_object_group<object_e::Flora> {
		static constexpr object_group_e value = object_group_e::Flora;
	};

	struct flora_t {
	  public:
		const bloomage_e bloomage;

	  	static constexpr i8 ProteinValue{ 1 };

	  private:
		static inline std::uniform_int_distribution<u16> bloomage_dis{ static_cast<u16>(bloomage_e::SanguineFlower), static_cast<u16>(bloomage_e::InfernalMushroom) };

		template<RandomEngine Generator> static inline bloomage_e random_bloomage(ref<Generator> generator) noexcept { return static_cast<bloomage_e>(bloomage_dis(generator)); }

	  public:
		inline flora_t(bloomage_e bloomage) noexcept : bloomage{ bloomage } {}

		template<RandomEngine Generator> inline flora_t(ref<Generator> engine) noexcept : bloomage{ random_bloomage(engine) } {}

		inline std::string to_string() const noexcept { return std::format("{}", necrowarp::to_string(bloomage)); }

		inline runes_t to_colored_string() const noexcept { return necrowarp::to_colored_string(bloomage); }

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Flora, static_cast<u8>(bloomage) }; }

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Flora; }
	};
} // namespace necrowarp
