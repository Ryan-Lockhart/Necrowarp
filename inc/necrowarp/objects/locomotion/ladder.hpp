#pragma once

#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<ladder_t> {
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

	template<> inline constexpr glyph_t object_glyphs<ladder_t>{ glyphs::UpLadder };

	enum struct verticality_e : u8 {
		Up,
		Down
	};

	constexpr cstr to_string(verticality_e type) noexcept {
		switch (type) {
			case verticality_e::Up: {
				return "up";
			} case verticality_e::Down: {
				return "down";
			} default: {
				return "unknown";
			}
		}
	}

	enum struct shackle_e : u8 {
		None,
		Calcitic,
		Spectral,
		Sanguine,
		Galvanic
	};

	constexpr cstr to_string(shackle_e type) noexcept {
		switch (type) {
			case shackle_e::Calcitic: {
				return "spooky shackle";
			} case shackle_e::Spectral: {
				return "eldritch shackle";
			} case shackle_e::Sanguine: {
				return "bloody shackle";
			} case shackle_e::Galvanic: {
				return "glimmering shackle";
			} default: {
				return "unshackled";
			}
		}
	}

	constexpr runes_t to_colored_string(shackle_e type) noexcept {
		const cstr string{ to_string(type) };
		switch (type) {
			case shackle_e::Calcitic: {
				return runes_t{ string, colors::metals::shackles::Calcitic };
			} case shackle_e::Spectral: {
				return runes_t{ string, colors::metals::shackles::Spectral };
			} case shackle_e::Sanguine: {
				return runes_t{ string, colors::metals::shackles::Sanguine };
			} case shackle_e::Galvanic: {
				return runes_t{ string, colors::metals::shackles::Galvanic };
			} default: {
				return runes_t{ string, colors::metals::Iron };
			}
		}
	}

	static inline std::uniform_int_distribution<u16> shackle_dis{ static_cast<u16>(shackle_e::Calcitic), static_cast<u16>(shackle_e::Galvanic) };

	template<RandomEngine Generator> static inline shackle_e random_shackle(ref<Generator> generator) noexcept { return static_cast<shackle_e>(shackle_dis(generator)); }

	struct ladder_t {
	  private:
		inline void draw_shackle(offset_t pos) const noexcept {
			switch (shackle) {
				case shackle_e::Calcitic: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Calcitic }, pos);
					return;
				} case shackle_e::Spectral: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Spectral }, pos);
					return;
				} case shackle_e::Sanguine: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Sanguine }, pos);
					return;
				} case shackle_e::Galvanic: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Galvanic }, pos);
					return;
				} default: {
					return;
				}
			}
		}

		inline void draw_shackle(offset_t pos, offset_t offset) const noexcept {
			switch (shackle) {
				case shackle_e::Calcitic: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Calcitic }, pos, offset);
					return;
				} case shackle_e::Spectral: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Spectral }, pos, offset);
					return;
				} case shackle_e::Sanguine: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Sanguine }, pos, offset);
					return;
				} case shackle_e::Galvanic: {
					entity_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Galvanic }, pos, offset);
					return;
				} default: {
					return;
				}
			}
		}
		
	  public:
		offset_t position;
		
		verticality_e verticality;
		shackle_e shackle;

		inline ladder_t(offset_t position) noexcept : position{ position }, verticality{ verticality_e::Up }, shackle{ shackle_e::None } {}

		inline ladder_t(offset_t position, verticality_e verticality, bool random = false) noexcept :
			position{ position },
			verticality{ verticality },
			shackle{ random ? random_shackle(random_engine) : shackle_e::None }
		{}

		inline ladder_t(offset_t position, verticality_e verticality, shackle_e shackle) noexcept : position{ position }, verticality{ verticality }, shackle{ shackle } {}

		inline bool is_up_ladder() const noexcept { return verticality == verticality_e::Up; }

		inline bool is_down_ladder() const noexcept { return verticality == verticality_e::Down; }

		inline bool has_shackle() const noexcept { return shackle != shackle_e::None; }

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

		inline glyph_t current_glyph() const noexcept { return is_up_ladder() ? object_glyphs<ladder_t> : glyphs::DownLadder; }

		inline void enshackle() noexcept {
			if (has_shackle()) {
				return;
			}
			
			shackle = random_shackle(random_engine);

			++steam_stats_s::stats<steam_stat_e::LaddersShackled, i32>;
		}

		inline void enshackle(shackle_e type) noexcept {
			if (has_shackle() || type == shackle_e::None) {
				return;
			}
			
			shackle = type;

			++steam_stats_s::stats<steam_stat_e::LaddersShackled, i32>;
		}

		inline void unshackle() noexcept {
			if (!has_shackle()) {
				return;
			}

			shackle = shackle_e::None;

			++steam_stats_s::stats<steam_stat_e::LaddersUnshackled, i32>;
		};

		inline void draw() const noexcept {
			entity_atlas.draw(current_glyph(), position);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(position);
		}

		inline void draw(offset_t offset) const noexcept {
			entity_atlas.draw(current_glyph(), position, offset);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(position, offset);
		}

		inline void draw(cref<camera_t> camera) const noexcept {
			const offset_t pos{ position + camera.get_offset() };

			entity_atlas.draw(current_glyph(), pos);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(pos);
		}

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept {
			const offset_t pos{ position + camera.get_offset() };

			entity_atlas.draw(current_glyph(), pos, offset);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(pos, offset);
		}

		constexpr operator object_e() const noexcept { return object_e::Ladder; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<ladder_t> ladder) noexcept { return offset_t::std_hasher::operator()(ladder.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<ladder_t> lhs, cref<ladder_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<ladder_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<ladder_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
