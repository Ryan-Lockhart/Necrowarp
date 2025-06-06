#pragma once

#include "bleak/constants/colors.hpp"
#include <necrowarp/objects/object.hpp>

#include <random>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<portal_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<portal_t> {
		static constexpr object_e value = object_e::Portal;
	};

	template<> struct is_object_type<portal_t, object_e::Portal> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Portal> {
		using type = portal_t;
	};

	template<> struct to_object_group<object_e::Portal> {
		static constexpr object_group_e value = object_group_e::Portal;
	};

	template<> inline constexpr glyph_t object_glyphs<portal_t>{ glyphs::CalmPortal };

	enum struct stability_e : u8 {
		Calm,
		Vocal,
		Turbulent
	};

	constexpr cstr to_string(stability_e type) noexcept {
		switch (type) {
			case stability_e::Calm: {
				return "calm";
			} case stability_e::Vocal: {
				return "vocal";
			} case stability_e::Turbulent: {
				return "turbulent";
			} default: {
				return "unknown";
			}
		}
	}

	constexpr runes_t to_colored_string(stability_e type) noexcept {
		const cstr string{ to_string(type) };
		switch (type) {
			case stability_e::Calm: {
				return runes_t{ string, colors::light::Blue };
			} case stability_e::Vocal: {
				return runes_t{ string, colors::light::Orange };
			} case stability_e::Turbulent: {
				return runes_t{ string, colors::light::Red };
			} default: {
				return runes_t{ string, colors::metals::Iron };
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

	struct portal_t {
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
		
		const stability_e stability;

		inline portal_t(offset_t position, bool random = false) noexcept :
			position{ position },
			stability{ random ? random_stability(random_engine) }
		{}

		inline portal_t(offset_t position, stability_e stability) noexcept :
			position{ position },
			stability{ stability }
		{}

		inline std::string to_string() const noexcept { return std::format("{} ({} | {})", necrowarp::to_string(object_e::Portal), necrowarp::to_string(verticality), necrowarp::to_string(shackle)); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(object_e::Portal) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(runes_t{ necrowarp::to_string(stability) })
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {}

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

		constexpr operator object_e() const noexcept { return object_e::Portal; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<portal_t> ladder) noexcept { return offset_t::std_hasher::operator()(ladder.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<portal_t> lhs, cref<portal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<portal_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<portal_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
