#pragma once

#include <bleak.hpp>
#include <format>

namespace necrowarp {
	using namespace bleak;

	namespace tutorial {
		using track_base = u8;

		enum struct track_e : track_base {
			Introduction,
			Locomotion,
			ChaoticWarp,
			PreciseWarp,
			Interaction,
			Catalysts,
			Invocations,
			Ladders,
			Portals,
			Opposition,
			Scaling,
			Patronage,
			Culmination
		};

		static constexpr cstr to_string(track_e track) noexcept {
			switch (track) {
				case track_e::Introduction: {
					return "introduction";
				} case track_e::Locomotion: {
					return "locomotion";
				} case track_e::ChaoticWarp: {
					return "chaotic warp";
				} case track_e::PreciseWarp: {
					return "precise warp";
				} case track_e::Interaction: {
					return "interaction";
				} case track_e::Catalysts: {
					return "catalysts";
				} case track_e::Invocations: {
					return "invocations";
				} case track_e::Ladders: {
					return "ladders";
				} case track_e::Portals: {
					return "portals";
				} case track_e::Opposition: {
					return "opposition";
				} case track_e::Scaling: {
					return "scaling";
				} case track_e::Patronage: {
					return "patronage";
				} case track_e::Culmination: {
					return "culmination";
				}
			}
		}

		static constexpr cstr to_chapter(track_e track) noexcept {
			switch (track) {
				case track_e::Introduction: {
					return "one";
				} case track_e::Locomotion: {
					return "two";
				} case track_e::ChaoticWarp: {
					return "three";
				} case track_e::PreciseWarp: {
					return "four";
				} case track_e::Interaction: {
					return "five";
				} case track_e::Catalysts: {
					return "six";
				} case track_e::Invocations: {
					return "seven";
				} case track_e::Ladders: {
					return "eight";
				} case track_e::Portals: {
					return "nine";
				} case track_e::Opposition: {
					return "ten";
				} case track_e::Scaling: {
					return "eleven";
				} case track_e::Patronage: {
					return "twelve";
				} case track_e::Culmination: {
					return "thirteen";
				}
			}
		}

		ref<track_e> static inline operator++(ref<track_e> track) noexcept { return track = static_cast<track_e>(static_cast<track_base>(track) + 1), track; }

		ref<track_e> static inline operator--(ref<track_e> track) noexcept { return track = static_cast<track_e>(static_cast<track_base>(track) - 1), track; }

		static constexpr track_e starting_track{ track_e::Introduction };
		static constexpr track_e ending_track{ track_e::Culmination };

		static inline std::string get_title(track_e track) {
			return std::format("chapter {} of {}: {}", to_chapter(track), to_chapter(ending_track), to_string(track));
		}

		static inline track_e current_track{ starting_track };

		static inline bool on_starting_track() noexcept { return current_track == starting_track; }

		static inline bool on_ending_track() noexcept { return current_track == ending_track; }

		static inline std::string get_current_title() { return get_title(current_track); }

		static inline void restart() noexcept { current_track = starting_track; }

		static inline void skip() noexcept { current_track = ending_track; }

		static inline void advance() noexcept {
			if (on_ending_track()) {
				return;
			}

			++current_track;
		}

		static inline void reverse() noexcept {
			if (on_starting_track()) {
				return;
			}

			--current_track;
		}
	} // tutorial
} // namespace necrowarp
