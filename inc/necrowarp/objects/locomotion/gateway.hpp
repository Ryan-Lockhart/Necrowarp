#pragma once

#include <necrowarp/objects/object.hpp>
#include <necrowarp/entities/entity.hpp>

#include <necrowarp/patronage.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<gateway_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<gateway_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<gateway_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<gateway_t> {
		static constexpr object_e value = object_e::Gateway;
	};

	template<> struct is_object_type<gateway_t, object_e::Gateway> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Gateway> {
		using type = gateway_t;
	};

	template<> struct to_object_group<object_e::Gateway> {
		static constexpr object_group_e value = object_group_e::Gateway;
	};

	struct gateway_t {
		const entity_e entity;

		keyframe_t idle_animation;

	  private:
	  	const i16 initial;

		i16 stability;

		inline void set_stability(i16 value) noexcept { stability = clamp<i16>(value, 0, max_stability()); }

		template<entity_e Entity>
			requires is_good<typename to_entity_type<Entity>::type>::value
		static constexpr i16 MinimumStability{};

		template<entity_e Entity>
			requires is_good<typename to_entity_type<Entity>::type>::value
		static constexpr i16 MaximumStability{};

		template<RandomEngine Generator> static constexpr i16 determine_stability(entity_e entity, disposition_e disposition, ref<Generator> engine) noexcept;

	  public:
		inline gateway_t(entity_e entity, disposition_e disposition) noexcept;

		inline i16 get_stability() const noexcept { return stability; }

		inline bool has_stability() const noexcept { return stability > 0; }

		inline i16 max_stability() const noexcept { return initial; }

		inline void erode() noexcept { set_stability(stability - 1); }

		inline void erode(i16 amount) noexcept { set_stability(stability - amount); }

		template<map_type_e MapType> inline bool spawn(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{}] ({})", necrowarp::to_string(object_e::Gateway), stability, max_stability(), necrowarp::to_string(entity));
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Gateway) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{}] (", stability, max_stability()) })
				.concatenate(necrowarp::to_colored_string(entity))
				.concatenate(runes_t{ ")" });

			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Gateway; }
	};
} // namespace necrowarp
