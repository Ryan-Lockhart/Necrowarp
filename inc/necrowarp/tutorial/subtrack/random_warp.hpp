#pragma once

#include <necrowarp/tutorial/subtrack/common.hpp>

namespace necrowarp::tutorial {
	using namespace bleak;

	template<> struct subtrack_s<track_e::ChaoticWarp> {
		static constexpr track_e current_track{ track_e::ChaoticWarp };

		using subtrack_t = subtrack_s<current_track>;

		using subtrack_base = u8;

		enum struct subtrack_e : subtrack_base {
			Explanation,
			Demonstration,
			Practice,
			Usage,
			Advanced
		};

		static constexpr cstr to_string(subtrack_e subtrack) noexcept {
			switch (subtrack) {
				case subtrack_e::Explanation: {
					return "explanation";
				} case subtrack_e::Demonstration: {
					return "demonstration";
				} case subtrack_e::Practice: {
					return "practice";
				} case subtrack_e::Usage: {
					return "usage";
				} case subtrack_e::Advanced: {
					return "advanced";
				}
			}
		}

		static constexpr cstr to_chapter(subtrack_e subtrack) noexcept {
			switch (subtrack) {
				case subtrack_e::Explanation: {
					return "one";
				} case subtrack_e::Demonstration: {
					return "two";
				} case subtrack_e::Practice: {
					return "three";
				} case subtrack_e::Usage: {
					return "four";
				} case subtrack_e::Advanced: {
					return "five";
				}
			}
		}

		static constexpr subtrack_e starting_subtrack{ subtrack_e::Explanation };
		static constexpr subtrack_e ending_subtrack{ subtrack_e::Advanced };

		static inline std::string get_title(subtrack_e subtrack) {
			return std::format("part {} of {}: {}", subtrack_t::to_chapter(subtrack), to_chapter(ending_subtrack), to_string(subtrack));
		}

		static inline subtrack_e current_subtrack{ starting_subtrack };

		static inline bool on_starting_subtrack() noexcept { return current_subtrack == starting_subtrack; }

		static inline bool on_ending_subtrack() noexcept { return current_subtrack == ending_subtrack; }

		static inline std::string get_current_title() { return get_title(current_subtrack); }

		static inline void restart() noexcept { current_subtrack = starting_subtrack; }

		static inline void skip() noexcept { current_subtrack = ending_subtrack; }

		static inline void advance() noexcept;

		static inline void reverse() noexcept;
	};
} // namespace necrowarp::tutorial
