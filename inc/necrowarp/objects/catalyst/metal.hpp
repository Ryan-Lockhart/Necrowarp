#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/galvanisation.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<metal_t> {
		static constexpr bool value = true;
	};

	template<> struct has_animation<metal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<metal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<metal_t> {
		static constexpr object_e value = object_e::Metal;
	};

	template<> struct is_object_type<metal_t, object_e::Metal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Metal> {
		using type = metal_t;
	};

	template<> struct to_object_group<object_e::Metal> {
		static constexpr object_group_e value = object_group_e::Metal;
	};

	struct metal_t {
		keyframe_t idle_animation;

		const galvanisation_e state;

		static constexpr galvanisation_e DefaultGalvanisation{ galvanisation_e::Twisted };

	  private:
		static constexpr u8 get_index(galvanisation_e state) noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return indices::TwistedMetal;
				} case galvanisation_e::Shimmering: {
					return indices::ShimmeringMetal;
				} case galvanisation_e::Wriggling: {
					return indices::WrigglingMetal;
				} case galvanisation_e::Writhing: {
					return indices::WrithingMetal;
				}
			}
		}

	  public:
		inline metal_t() noexcept : idle_animation{ get_index(DefaultGalvanisation), random_engine, true }, state{ DefaultGalvanisation } {}

		inline metal_t(galvanisation_e state) noexcept : idle_animation{ get_index(state), random_engine, true }, state{ state } {}

		template<galvanisation_e Galvanisation> inline bool is() const noexcept { return state == Galvanisation; }

		inline std::string to_string() const noexcept { return std::format("{} ({})", necrowarp::to_string(object_e::Metal), necrowarp::to_string(state)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Metal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Metal; }
	};
} // namespace necrowarp
