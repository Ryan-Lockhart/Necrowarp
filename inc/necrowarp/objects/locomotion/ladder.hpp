#pragma once

#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

#include <necrowarp/constants/enums/shackle.tpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<ladder_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<ladder_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<ladder_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<ladder_t> {
		static constexpr object_e value = object_e::Ladder;
	};

	template<> struct is_object_type<ladder_t, object_e::Ladder> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Ladder> {
		using type = ladder_t;
	};

	template<> struct to_object_group<object_e::Ladder> {
		static constexpr object_group_e value = object_group_e::Ladder;
	};

	enum struct verticality_e : u8 {
		Up,
		Down
	};

	template<verticality_e Verticality> constexpr glyph_t ladder_glyph;

	template<> inline constexpr glyph_t ladder_glyph<verticality_e::Up>{ glyphs::UpLadder };
	template<> inline constexpr glyph_t ladder_glyph<verticality_e::Down>{ glyphs::DownLadder };

	constexpr cstr to_string(verticality_e type) noexcept {
		switch (type) {
			case verticality_e::Up: {
				return "up";
			} case verticality_e::Down: {
				return "down";
			}
		}
	}

	struct ladder_t {
		keyframe_t idle_animation;

		const verticality_e verticality;
		shackle_e shackle;

	  private:
		static inline std::uniform_int_distribution<u16> shackle_dis{ static_cast<u16>(shackle_e::Calcitic), static_cast<u16>(shackle_e::Galvanic) };

		template<RandomEngine Generator> static inline shackle_e random_shackle(ref<Generator> generator) noexcept { return static_cast<shackle_e>(shackle_dis(generator)); }

		static constexpr u8 get_index(shackle_e shackle) noexcept {
			switch (shackle) {
				case shackle_e::Calcitic: {
					return indices::CalciticShackle;
				} case shackle_e::Spectral: {
					return indices::SpectralShackle;
				} case shackle_e::Sanguine: {
					return indices::SanguineShackle;
				} case shackle_e::Galvanic: {
					return indices::GalvanicShackle;
				} case shackle_e::Ravenous: {
					return indices::RavenousShackle;
				} case shackle_e::Wretched: {
					return indices::WretchedShackle;
				} case shackle_e::Cerebral: {
					return indices::CerebralShackle;
				} case shackle_e::Infernal: {
					return indices::InfernalShackle;
				} case shackle_e::Unshackled: {
					return 0;
				}				
			}
		}

		inline void sync_animation() noexcept {
			idle_animation.index = get_index(shackle);

			if (has_shackle()) {
				idle_animation.start();
			} else {
				idle_animation.stop();
			}
		}
	
	public:
		inline ladder_t() noexcept : idle_animation{ random_engine }, verticality{ verticality_e::Up }, shackle{ shackle_e::Unshackled } {
			sync_animation();
		}

		inline ladder_t(verticality_e verticality) noexcept : idle_animation{ random_engine }, verticality{ verticality }, shackle{ shackle_e::Unshackled } {
			sync_animation();
		}

		template<RandomEngine Generator> inline ladder_t(verticality_e verticality, ref<Generator> generator) noexcept : idle_animation{ random_engine }, verticality{ verticality }, shackle{ random_shackle(generator) } {
			sync_animation();
		}

		inline ladder_t(verticality_e verticality, shackle_e shackle) noexcept : idle_animation{ random_engine }, verticality{ verticality }, shackle{ shackle } {
			sync_animation();
		}

		inline bool is_up_ladder() const noexcept { return verticality == verticality_e::Up; }

		inline bool is_down_ladder() const noexcept { return verticality == verticality_e::Down; }

		inline bool has_shackle() const noexcept { return shackle != shackle_e::Unshackled; }

		inline std::string to_string() const noexcept { return std::format("{} ({} | {})", necrowarp::to_string(object_e::Ladder), necrowarp::to_string(verticality), necrowarp::to_string(shackle)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Ladder) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(runes_t{ necrowarp::to_string(verticality) })
				.concatenate(runes_t{ " | " })
				.concatenate(necrowarp::to_colored_string(shackle))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			return magic_enum::enum_switch([&](auto val) -> glyph_t {
				constexpr verticality_e cval{ val };

				return ladder_glyph<cval>;
			}, verticality);
		}

		inline void enshackle() noexcept;

		inline void enshackle(shackle_e type) noexcept;

		inline void unshackle() noexcept;

		inline void draw(offset_t position) const noexcept {
			game_atlas.draw(current_glyph(), position);

			if (has_shackle()) {
				animated_atlas.draw(idle_animation, colors::White, position);
			}
		}

		inline void draw(offset_t position, offset_t offset) const noexcept {
			game_atlas.draw(current_glyph(), position + offset);

			if (has_shackle()) {
				animated_atlas.draw(idle_animation, colors::White, position + offset);
			}
		}

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept {
			game_atlas.draw(current_glyph(), position + offset, nudge);

			if (has_shackle()) {
				animated_atlas.draw(idle_animation, colors::White, position + offset, nudge);
			}
		}

		constexpr operator object_e() const noexcept { return object_e::Ladder; }
	};
} // namespace necrowarp
