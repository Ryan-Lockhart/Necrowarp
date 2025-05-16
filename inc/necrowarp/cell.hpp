#pragma once

#include <bleak.hpp>

#include <bit>
#include <format>
#include <string>

#include <necrowarp/globals.hpp>

namespace necrowarp {
	using namespace bleak;

	enum class fluid_type_e : u8 {
		None = 0,

		Blood = 1 << 0,
		Ichor = 1 << 1,

		BloodyIchor = Blood | Ichor,
	};

	constexpr cstr to_string(fluid_type_e fluid) noexcept {
		switch (fluid) {
			case fluid_type_e::None: {
				return "none";
			} case fluid_type_e::Blood: {
				return "blood";
			} case fluid_type_e::Ichor: {
				return "ichor";
			} case fluid_type_e::BloodyIchor: {
				return "bloody ichor";
			}
		}
	}

	static inline color_t fluid_color(fluid_type_e fluid) noexcept {
			switch (fluid) {
				case fluid_type_e::None: {
					return colors::Transparent;
				} case fluid_type_e::Blood: {
					return colors::materials::Blood;
				} case fluid_type_e::Ichor: {
					return colors::materials::Ichor;
				} case fluid_type_e::BloodyIchor: {
					return colors::materials::BloodyIchor;
				}
			}
		}

	constexpr runes_t to_colored_string(fluid_type_e fluid) noexcept {
		const cstr string{ to_string(fluid) };

		switch (fluid) {
			case fluid_type_e::None: {
				return runes_t{ string, colors::White };
			} case fluid_type_e::Blood: {
				return runes_t{ string, colors::materials::Blood };
			} case fluid_type_e::Ichor: {
				return runes_t{ string, colors::materials::Ichor };
			} case fluid_type_e::BloodyIchor: {
				return runes_t{ string, colors::materials::BloodyIchor };
			}
		}
	}

	struct fluid_cell_t {
		bool blood : 1 { false };
		bool ichor : 1 { false };
		bool : 6;

		constexpr fluid_cell_t() noexcept = default;

		constexpr fluid_cell_t(cref<fluid_cell_t> other) noexcept = default;
		constexpr fluid_cell_t(rval<fluid_cell_t> other) noexcept = default;

		constexpr ref<fluid_cell_t> operator=(cref<fluid_cell_t> other) noexcept = default;
		constexpr ref<fluid_cell_t> operator=(rval<fluid_cell_t> other) noexcept = default;

		inline bool contains(fluid_type_e fluid) const noexcept {
			switch (fluid) {
				case fluid_type_e::None: {
					return !blood && !ichor;
				} case fluid_type_e::Blood: {
					return blood;
				} case fluid_type_e::Ichor: {
					return ichor;
				} case fluid_type_e::BloodyIchor: {
					return blood && ichor;
				}
			}
		}

		inline void set(fluid_type_e fluid) noexcept {
			switch (fluid) {
				case fluid_type_e::None: {
					blood = false;
					ichor = false;
				} case fluid_type_e::Blood: {
					blood = true;
				} case fluid_type_e::Ichor: {
					ichor = true;
				} case fluid_type_e::BloodyIchor: {
					blood = true;
					ichor = true;
				}
			}
		}

		inline void unset(fluid_type_e fluid) noexcept {
			switch (fluid) {
				case fluid_type_e::None: {
					return;
				} case fluid_type_e::Blood: {
					blood = false;
				} case fluid_type_e::Ichor: {
					ichor = false;
				} case fluid_type_e::BloodyIchor: {
					blood = false;
					ichor = false;
				}
			}
		}

		template<typename... Fluids>
			requires is_homogeneous<fluid_type_e, Fluids...>::value && is_plurary<Fluids...>::value
		constexpr fluid_cell_t(Fluids... fluids) {
			for (fluid_type_e fluid : { fluids... }) {
				set(fluid);
			}
		}

		constexpr inline fluid_cell_t operator+(fluid_type_e fluid) const noexcept {
			fluid_cell_t state{ *this };

			state.set(fluid);

			return state;
		}

		constexpr inline fluid_cell_t operator-(fluid_type_e fluid) const noexcept {
			fluid_cell_t cell{ *this };

			cell.unset(fluid);

			return cell;
		}

		constexpr inline ref<fluid_cell_t> operator+=(fluid_type_e fluid) noexcept {
			set(fluid);

			return *this;
		}

		constexpr inline ref<fluid_cell_t> operator-=(fluid_type_e fluid) noexcept {
			unset(fluid);

			return *this;
		}

		constexpr ref<fluid_cell_t> operator=(fluid_type_e fluid) noexcept {
			set(fluid);

			return *this;
		}

		constexpr bool operator==(fluid_cell_t other) const noexcept { return blood == other.blood && ichor == other.ichor; }

		constexpr bool operator!=(fluid_cell_t other) const noexcept { return blood != other.blood || ichor != other.ichor; }

		constexpr bool operator==(fluid_type_e fluid) const noexcept { return contains(fluid); }

		constexpr bool operator!=(fluid_type_e fluid) const noexcept { return !contains(fluid); }

		constexpr explicit operator fluid_type_e() const noexcept {
			u8 fluid{ 0 };

			if (blood) { fluid |= static_cast<u8>(fluid_type_e::Blood); }
			if (ichor) { fluid |= static_cast<u8>(fluid_type_e::Ichor); }

			return static_cast<fluid_type_e>(fluid);
		}

		inline constexpr std::string to_tooltip() const {
			const fluid_type_e fluid{ static_cast<fluid_type_e>(*this) };

			return std::format("The cell is {}.", fluid == fluid_type_e::None ? "dry" : std::format("spattered with {}", to_string(fluid)));
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_type_e>(zone[position])) }, position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_type_e>(zone[position])) }, position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_type_e>(zone[position])) }, position + offset, nudge);
		}

		struct hasher {
			static constexpr usize operator()(fluid_cell_t cell) noexcept { return hash_combine(std::bit_cast<u8>(cell)); }
		};
	};

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

	struct map_cell_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };
		bool smooth : 1 { false };
		bool protrudes : 1 { false };
		bool : 2;

		constexpr map_cell_t() noexcept = default;

		constexpr map_cell_t(cref<map_cell_t> other) noexcept = default;
		constexpr map_cell_t(rval<map_cell_t> other) noexcept = default;

		constexpr ref<map_cell_t> operator=(cref<map_cell_t> other) noexcept = default;
		constexpr ref<map_cell_t> operator=(rval<map_cell_t> other) noexcept = default;

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

		constexpr void set(cell_trait_t trait) noexcept {
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

		constexpr void unset(cell_trait_t trait) noexcept {
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

		template<typename... Traits>
			requires is_homogeneous<cell_trait_t, Traits...>::value && is_plurary<Traits...>::value
		constexpr map_cell_t(Traits... traits) {
			for (cref<cell_trait_t> trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline map_cell_t operator+(cell_trait_t trait) const noexcept {
			map_cell_t state{ *this };

			state.set(trait);

			return state;
		}

		constexpr inline map_cell_t operator-(cell_trait_t trait) const noexcept {
			map_cell_t state{ *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<map_cell_t> operator+=(cell_trait_t trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline ref<map_cell_t> operator-=(cell_trait_t trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr ref<map_cell_t> operator=(cell_trait_t trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr bool operator==(map_cell_t other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored && smooth == other.smooth && protrudes == other.protrudes;
		}

		constexpr bool operator!=(map_cell_t other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored || smooth != other.smooth || protrudes != other.protrudes;
		}

		constexpr bool operator==(cell_trait_t other) const noexcept { return contains(other); }

		constexpr bool operator!=(cell_trait_t other) const noexcept { return !contains(other); }

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
			static constexpr usize operator()(map_cell_t cell) noexcept { return hash_combine(std::bit_cast<u8>(cell)); }
		};
	};
} // namespace necrowarp

namespace bleak {
	template<> struct is_drawable<necrowarp::fluid_cell_t> {
		static bool constexpr value = true;
	};

	template<> struct is_drawable<necrowarp::map_cell_t> {
		static bool constexpr value = true;
	};
} // namespace bleak
