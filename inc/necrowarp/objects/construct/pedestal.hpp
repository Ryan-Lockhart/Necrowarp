#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>
#include <necrowarp/literature.hpp>

#include <necrowarp/constants/enums/grimoire.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_blocking<pedestal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<pedestal_t> {
		static constexpr object_e value = object_e::Pedestal;
	};

	template<> struct is_object_type<pedestal_t, object_e::Pedestal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Pedestal> {
		using type = pedestal_t;
	};

	template<> struct to_object_group<object_e::Pedestal> {
		static constexpr object_group_e value = object_group_e::Pedestal;
	};

	struct pedestal_t {
		const grimoire_e grimoire;

	  private:
		static inline std::uniform_int_distribution<u16> grimoire_dis{ static_cast<u16>(grimoire_e::ChaoticWarp), static_cast<u16>(grimoire_e::CalamitousRetaliation) };

		template<RandomEngine Generator> static inline grimoire_e random_grimoire(ref<Generator> engine) noexcept { return grimoire_dis(engine); }

		bool looted;

	  public:
		inline pedestal_t(grimoire_e grimoire) noexcept : grimoire{ grimoire }, looted{ false } {}

		template<RandomEngine Generator> inline pedestal_t(ref<Generator> engine) noexcept : grimoire{ random_grimoire(engine) }, looted{ false } {}

		inline bool is_looted() const noexcept { return looted; }

		inline bool can_loot() const noexcept { return !looted && literature::can_acquire(grimoire); }

		inline bool loot() noexcept {
			if (!can_loot()) {
				return false;
			}

			looted = literature::acquire(grimoire);

			return !looted;
		}

		inline std::string to_string() const noexcept {
			return std::format("{} ({})", necrowarp::to_string(object_e::Pedestal), looted ? "empty" : necrowarp::to_string(grimoire));
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Pedestal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(looted ? runes_t{ "empty", colors::dark::Grey } : necrowarp::to_colored_string(grimoire))
				.concatenate(runes_t{ ")" });

			return colored_string;
		}

		inline keyframe_t current_keyframe() const noexcept { return keyframe_t{ indices::Pedestal, static_cast<u8>(grimoire) }; }

		inline void draw(offset_t position) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position);
			}
		}

		inline void draw(offset_t position, offset_t offset) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position + offset);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position + offset);
			}
		}

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (looted) {
				game_atlas.draw(glyphs::Pedestal, position + offset, nudge);
			} else {
				animated_atlas.draw(current_keyframe(), colors::White, position + offset, nudge);
			}
		}

		constexpr operator object_e() const noexcept { return object_e::Pedestal; }
	};
} // namespace necrowarp
