#pragma once

#include <bleak.hpp>

#include <bit>
#include <format>
#include <string>

namespace necrowarp {
	using namespace bleak;

	enum class cell_trait_t : u8 {
		Open,
		Solid,
		Transperant,
		Opaque,
		Unseen,
		Seen,
		Unexplored,
		Explored,
		Rough,
		Smooth,
		Recedes,
		Protrudes
	};

	constexpr cstr to_string(cell_trait_t trait) noexcept {
		switch (trait) {
			case cell_trait_t::Open: {
				return "open";
			} case cell_trait_t::Solid: {
				return "solid";
			} case cell_trait_t::Transperant: {
				return "transperant";
			} case cell_trait_t::Opaque: {
				return "opaque";
			} case cell_trait_t::Unseen: {
				return "unseen";
			} case cell_trait_t::Seen: {
				return "seen";
			} case cell_trait_t::Unexplored: {
				return "unexplored";
			} case cell_trait_t::Explored: {
				return "explored";
			} case cell_trait_t::Rough: {
				return "rough";
			} case cell_trait_t::Smooth: {
				return "smooth";
			} case cell_trait_t::Recedes: {
				return "recedes";
			} case cell_trait_t::Protrudes: {
				return "protrudes";
			}
		}
	}

	struct cell_state_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };
		bool smooth : 1 { false };
		bool protrudes : 1 { false };
		bool : 2;

		constexpr cell_state_t() noexcept = default;

		constexpr cell_state_t(cref<cell_state_t> other) noexcept = default;

		constexpr cell_state_t(rval<cell_state_t> other) noexcept = default;

		constexpr ref<cell_state_t> operator=(cref<cell_state_t> other) noexcept = default;

		constexpr ref<cell_state_t> operator=(rval<cell_state_t> other) noexcept = default;

		template<typename... Traits>
			requires is_homogeneous<cell_trait_t, Traits...>::value && is_plurary<Traits...>::value
		constexpr cell_state_t(Traits... traits) {
			for (cref<cell_trait_t> trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline cell_state_t operator+(cref<cell_trait_t> trait) const noexcept {
			cell_state_t state{ *this };

			state.set(trait);

			return state;
		}

		constexpr inline cell_state_t operator-(cref<cell_trait_t> trait) const noexcept {
			cell_state_t state{ *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator+=(cref<cell_trait_t> trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline ref<cell_state_t> operator-=(cref<cell_trait_t> trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr void set(cref<cell_trait_t> trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = false;
				break;
			case cell_trait_t::Solid:
				solid = true;
				break;
			case cell_trait_t::Transperant:
				opaque = false;
				break;
			case cell_trait_t::Opaque:
				opaque = true;
				break;
			case cell_trait_t::Unseen:
				seen = false;
				break;
			case cell_trait_t::Seen:
				seen = true;
				break;
			case cell_trait_t::Unexplored:
				explored = false;
				break;
			case cell_trait_t::Explored:
				explored = true;
				break;
			case cell_trait_t::Rough:
				smooth = false;
				break;
			case cell_trait_t::Smooth:
				smooth = true;
				break;
			case cell_trait_t::Recedes:
				protrudes = false;
				break;
			case cell_trait_t::Protrudes:
				protrudes = true;
				break;
			default:
				break;
			}
		}

		constexpr void unset(cref<cell_trait_t> trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = true;
				break;
			case cell_trait_t::Solid:
				solid = false;
				break;
			case cell_trait_t::Transperant:
				opaque = true;
				break;
			case cell_trait_t::Opaque:
				opaque = false;
				break;
			case cell_trait_t::Unseen:
				seen = true;
				break;
			case cell_trait_t::Seen:
				seen = false;
				break;
			case cell_trait_t::Unexplored:
				explored = true;
				break;
			case cell_trait_t::Explored:
				explored = false;
				break;
			case cell_trait_t::Rough:
				smooth = true;
				break;
			case cell_trait_t::Smooth:
				smooth = false;
				break;
			case cell_trait_t::Protrudes:
				protrudes = true;
				break;
			case cell_trait_t::Recedes:
				protrudes = false;
				break;
			default:
				break;
			}
		}

		constexpr ref<cell_state_t> operator=(cell_trait_t trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr bool operator==(cref<cell_state_t> other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored && smooth == other.smooth && protrudes == other.protrudes;
		}

		constexpr bool operator!=(cref<cell_state_t> other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored || smooth != other.smooth || protrudes != other.protrudes;
		}

		constexpr bool operator==(cref<cell_trait_t> other) const noexcept { return contains(other); }

		constexpr bool operator!=(cref<cell_trait_t> other) const noexcept { return !contains(other); }

		constexpr bool contains(cell_trait_t trait) const noexcept {
			switch (trait) {
			case cell_trait_t::Solid:
				return solid;
			case cell_trait_t::Open:
				return !solid;
			case cell_trait_t::Opaque:
				return opaque;
			case cell_trait_t::Transperant:
				return !opaque;
			case cell_trait_t::Seen:
				return seen;
			case cell_trait_t::Unseen:
				return !seen;
			case cell_trait_t::Explored:
				return explored;
			case cell_trait_t::Unexplored:
				return !explored;
			case cell_trait_t::Smooth:
				return smooth;
			case cell_trait_t::Rough:
				return !smooth;
			case cell_trait_t::Protrudes:
				return protrudes;
			case cell_trait_t::Recedes:
				return !protrudes;
			default:
				return false;
			}
		}

		inline constexpr std::string to_tooltip() const {
			if (seen && explored) {
				return std::format(
					"The cell is physically {} and visibility is {}.\nIt is {} and {} by the player.\nThe {}{} is {} to the touch.",
					solid ? "blocked" : "open",
					opaque ? "obscured" : "unobscured",
					seen ? "in view" : "not in view",
					explored ? "was explored" : "remains unexplored",
					solid ? protrudes ? "protruding " : "receding " : "",
					solid ? "wall" : "floor",
					smooth ? "smooth" : "rough"
				);
			} else if (explored) {
				return std::format(
					"The cell is physically {} and visibility is {}.\nIt is {} and {} by the player.",
					solid ? "blocked" : "open",
					opaque ? "obscured" : "unobscured",
					seen ? "in view" : "not in view",
					explored ? "was explored" : "remains unexplored"
				);
			} else {
				return "The cell remains unexplored.";
			}
		}

		inline constexpr operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}, {}, {}]",
				solid ? "Solid" : "Open",
				opaque ? "Opaque" : "Transperant",
				seen ? "Seen" : "Unseen",
				explored ? "Explored" : "Unexplored",
				smooth ? "Smooth" : "Rough",
				protrudes ? "Protrudes" : "Recedes"
			);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position);

			if (!solid) {
				return;
			}

			const u8 glyph{ characters::auto_set(smooth, protrudes, zone.template calculate_index<neighbourhood_solver_t::Melded>(position, cell_trait_t::Solid)) };

			atlas.draw(glyph_t{ glyph, color_t{ 0xC0, alpha } }, position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position + offset);

			if (!solid) {
				return;
			}

			const u8 glyph{ characters::auto_set(smooth, protrudes, zone.template calculate_index<neighbourhood_solver_t::Melded>(position, cell_trait_t::Solid)) };

			atlas.draw(glyph_t{ glyph, color_t{ 0xC0, alpha } }, position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position + offset, nudge);

			if (!solid) {
				return;
			}

			const u8 glyph{ characters::auto_set(smooth, protrudes, zone.template calculate_index<neighbourhood_solver_t::Melded>(position, cell_trait_t::Solid)) };

			atlas.draw(glyph_t{ glyph, color_t{ 0xC0, alpha } }, position + offset, nudge);
		}

		struct hasher {
			static constexpr usize operator()(cref<cell_state_t> cell_state) noexcept { return hash_combine(std::bit_cast<u8>(cell_state)); }
		};
	};
} // namespace necrowarp

namespace bleak {
	template<> struct is_drawable<necrowarp::cell_state_t> {
		static bool constexpr value = true;
	};
} // namespace bleak
