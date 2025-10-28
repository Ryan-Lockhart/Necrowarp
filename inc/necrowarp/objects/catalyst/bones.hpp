#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/decay.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<bones_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<bones_t> {
		static constexpr object_e value = object_e::Bones;
	};

	template<> struct is_object_type<bones_t, object_e::Bones> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Bones> {
		using type = bones_t;
	};

	template<> struct to_object_group<object_e::Bones> {
		static constexpr object_group_e value = object_group_e::Bones;
	};

	struct bones_t {
		template<decay_e Decay> static constexpr u8 DecayEpoch{};

		template<> inline constexpr u8 DecayEpoch<decay_e::Fresh>{ 32 };
		template<> inline constexpr u8 DecayEpoch<decay_e::Animate>{ 64 };
		template<> inline constexpr u8 DecayEpoch<decay_e::Rotted>{ 128 };

	  private:
		decay_e state;
		u8 lifespan;

		static constexpr u8 determine_lifespan(decay_e decay) noexcept {
			return magic_enum::enum_switch([&](auto val) -> u8 {
				constexpr decay_e cval{ val };

				return DecayEpoch<cval>;
			}, decay);
		}

	  public:
		inline bones_t() noexcept : state{ decay_e::Fresh }, lifespan{ DecayEpoch<decay_e::Fresh> } {}

		inline bones_t(decay_e state) noexcept : state{ state }, lifespan{ determine_lifespan(state) } {}

		inline decay_e get_state() const noexcept { return state; }

		inline i8 get_lifespan() const noexcept { return lifespan; }

		inline f32 get_percentile() const noexcept {
			return magic_enum::enum_switch([&](auto val) -> f32 {
				constexpr decay_e cval{ val };

				return clamp<f32>(static_cast<f32>(lifespan) / static_cast<f32>(DecayEpoch<cval>) * 100.0f, 0.1f, 100.0f);
			}, state);
		}

		template<map_type_e MapType> inline bool decay(offset_t position) noexcept;

		template<decay_e Decay> inline bool is() const noexcept { return state == Decay; }

		inline std::string to_string() const noexcept { return std::format("{} {} [{:.1f}%]", necrowarp::to_string(state), necrowarp::to_string<plurality_e::Multiple>(object_e::Bones), get_percentile()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(state) };

			colored_string
				.concatenate(runes_t{ " " })
				.concatenate(necrowarp::to_colored_string<plurality_e::Multiple>(object_e::Bones))
				.concatenate(runes_t{ std::format(" [{:.1f}%]", get_percentile()) });

			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case decay_e::Rotted: {
					return glyphs::RottedBones;
				} case decay_e::Animate: {
					return glyphs::AnimateBones;
				} case decay_e::Fresh: {
					return glyphs::FreshBones;
				}
			}
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Bones; }
	};
} // namespace necrowarp
