#pragma once

#include <bleak.hpp>

#include <bit>
#include <format>
#include <string>

#include <necrowarp/globals.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct fluid_e : u8 {
		None = 0,

		Blood = 1 << 0,
		Ichor = Blood << 1,

		BloodyIchor = Blood | Ichor,
	};

	constexpr cstr to_string(fluid_e fluid) noexcept {
		switch (fluid) {
			case fluid_e::None: {
				return "none";
			} case fluid_e::Blood: {
				return "blood";
			} case fluid_e::Ichor: {
				return "ichor";
			} case fluid_e::BloodyIchor: {
				return "bloody ichor";
			}
		}
	}

	static inline color_t fluid_color(fluid_e fluid) noexcept {
			switch (fluid) {
				case fluid_e::None: {
					return colors::Transparent;
				} case fluid_e::Blood: {
					return colors::materials::Blood;
				} case fluid_e::Ichor: {
					return colors::materials::Ichor;
				} case fluid_e::BloodyIchor: {
					return colors::materials::BloodyIchor;
				}
			}
		}

	constexpr runes_t to_colored_string(fluid_e fluid) noexcept {
		const cstr string{ to_string(fluid) };

		switch (fluid) {
			case fluid_e::None: {
				return runes_t{ string, colors::White };
			} case fluid_e::Blood: {
				return runes_t{ string, colors::materials::Blood };
			} case fluid_e::Ichor: {
				// using magenta because I can't read the colored string for ichor
				return runes_t{ string, colors::dark::Magenta };
			} case fluid_e::BloodyIchor: {
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

		inline bool contains(fluid_e fluid) const noexcept {
			switch (fluid) {
				case fluid_e::None: {
					return !blood && !ichor;
				} case fluid_e::Blood: {
					return blood;
				} case fluid_e::Ichor: {
					return ichor;
				} case fluid_e::BloodyIchor: {
					return blood && ichor;
				}
			}
		}

		inline void set(fluid_e fluid) noexcept {
			switch (fluid) {
				case fluid_e::None: {
					blood = false;
					ichor = false;
					break;
				} case fluid_e::Blood: {
					blood = true;
					break;
				} case fluid_e::Ichor: {
					ichor = true;
					break;
				} case fluid_e::BloodyIchor: {
					blood = true;
					ichor = true;
					break;
				}
			}
		}

		inline void unset(fluid_e fluid) noexcept {
			switch (fluid) {
				case fluid_e::None: {
					break;
				} case fluid_e::Blood: {
					blood = false;
					break;
				} case fluid_e::Ichor: {
					ichor = false;
					break;
				} case fluid_e::BloodyIchor: {
					blood = false;
					ichor = false;
					break;
				}
			}
		}

		template<typename... Fluids>
			requires is_homogeneous<fluid_e, Fluids...>::value && is_plurary<Fluids...>::value
		constexpr fluid_cell_t(Fluids... fluids) {
			for (fluid_e fluid : { fluids... }) {
				set(fluid);
			}
		}

		constexpr inline fluid_cell_t operator+(fluid_e fluid) const noexcept {
			fluid_cell_t state{ *this };

			state.set(fluid);

			return state;
		}

		constexpr inline fluid_cell_t operator-(fluid_e fluid) const noexcept {
			fluid_cell_t cell{ *this };

			cell.unset(fluid);

			return cell;
		}

		constexpr inline ref<fluid_cell_t> operator+=(fluid_e fluid) noexcept {
			set(fluid);

			return *this;
		}

		constexpr inline ref<fluid_cell_t> operator-=(fluid_e fluid) noexcept {
			unset(fluid);

			return *this;
		}

		constexpr ref<fluid_cell_t> operator=(fluid_e fluid) noexcept {
			set(fluid);

			return *this;
		}

		constexpr bool operator==(fluid_cell_t other) const noexcept { return blood == other.blood && ichor == other.ichor; }

		constexpr bool operator!=(fluid_cell_t other) const noexcept { return blood != other.blood || ichor != other.ichor; }

		constexpr bool operator==(fluid_e fluid) const noexcept { return contains(fluid); }

		constexpr bool operator!=(fluid_e fluid) const noexcept { return !contains(fluid); }

		constexpr explicit operator fluid_e() const noexcept {
			u8 fluid{ 0 };

			if (blood) { fluid |= static_cast<u8>(fluid_e::Blood); }
			if (ichor) { fluid |= static_cast<u8>(fluid_e::Ichor); }

			return static_cast<fluid_e>(fluid);
		}

		inline constexpr std::string to_tooltip() const {
			const fluid_e fluid{ static_cast<fluid_e>(*this) };

			return std::format("The cell is {}.", fluid == fluid_e::None ? "dry" : std::format("spattered with {}", to_string(fluid)));
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_e>(zone[position])) }, position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_e>(zone[position])) }, position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			atlas.draw(glyph_t{ characters::Floor, fluid_color(static_cast<fluid_e>(zone[position])) }, position + offset, nudge);
		}

		struct hasher {
			static constexpr usize operator()(fluid_cell_t cell) noexcept { return hash_combine(std::bit_cast<u8>(cell)); }
		};
	};

	enum struct cell_e : u8 {
		Open,
		Solid,
		Transperant,
		Opaque,
		Unseen,
		Seen,
		Unexplored,
		Explored
	};

	constexpr cstr to_string(cell_e trait) noexcept {
		switch (trait) {
			case cell_e::Open: {
				return "open";
			} case cell_e::Solid: {
				return "solid";
			} case cell_e::Transperant: {
				return "transperant";
			} case cell_e::Opaque: {
				return "opaque";
			} case cell_e::Unseen: {
				return "unseen";
			} case cell_e::Seen: {
				return "seen";
			} case cell_e::Unexplored: {
				return "unexplored";
			} case cell_e::Explored: {
				return "explored";
			}
		}
	}

	struct map_cell_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };

		u8 index: 4 { 0 };

		constexpr map_cell_t() noexcept = default;

		constexpr map_cell_t(cref<map_cell_t> other) noexcept = default;
		constexpr map_cell_t(rval<map_cell_t> other) noexcept = default;

		constexpr ref<map_cell_t> operator=(cref<map_cell_t> other) noexcept = default;
		constexpr ref<map_cell_t> operator=(rval<map_cell_t> other) noexcept = default;

		constexpr bool contains(cell_e trait) const noexcept {
			switch (trait) {
			case cell_e::Solid:
				return solid;
			case cell_e::Open:
				return !solid;
			case cell_e::Opaque:
				return opaque;
			case cell_e::Transperant:
				return !opaque;
			case cell_e::Seen:
				return seen;
			case cell_e::Unseen:
				return !seen;
			case cell_e::Explored:
				return explored;
			case cell_e::Unexplored:
				return !explored;
			default:
				return false;
			}
		}

		constexpr void set(cell_e trait) noexcept {
			switch (trait) {
			case cell_e::Open:
				solid = false;
				break;
			case cell_e::Solid:
				solid = true;
				break;
			case cell_e::Transperant:
				opaque = false;
				break;
			case cell_e::Opaque:
				opaque = true;
				break;
			case cell_e::Unseen:
				seen = false;
				break;
			case cell_e::Seen:
				seen = true;
				break;
			case cell_e::Unexplored:
				explored = false;
				break;
			case cell_e::Explored:
				explored = true;
				break;
			default:
				break;
			}
		}

		constexpr void unset(cell_e trait) noexcept {
			switch (trait) {
			case cell_e::Open:
				solid = true;
				break;
			case cell_e::Solid:
				solid = false;
				break;
			case cell_e::Transperant:
				opaque = true;
				break;
			case cell_e::Opaque:
				opaque = false;
				break;
			case cell_e::Unseen:
				seen = true;
				break;
			case cell_e::Seen:
				seen = false;
				break;
			case cell_e::Unexplored:
				explored = true;
				break;
			case cell_e::Explored:
				explored = false;
				break;
			default:
				break;
			}
		}

		template<typename... Traits>
			requires is_homogeneous<cell_e, Traits...>::value && is_plurary<Traits...>::value
		constexpr map_cell_t(Traits... traits) {
			for (cref<cell_e> trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline map_cell_t operator+(cell_e trait) const noexcept {
			map_cell_t state{ *this };

			state.set(trait);

			return state;
		}

		constexpr inline map_cell_t operator-(cell_e trait) const noexcept {
			map_cell_t state{ *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<map_cell_t> operator+=(cell_e trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline ref<map_cell_t> operator-=(cell_e trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr ref<map_cell_t> operator=(cell_e trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr bool operator==(map_cell_t other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored;
		}

		constexpr bool operator!=(map_cell_t other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored;
		}

		constexpr bool operator==(cell_e other) const noexcept { return contains(other); }

		constexpr bool operator!=(cell_e other) const noexcept { return !contains(other); }

		inline constexpr std::string to_tooltip() const {
			if (explored) {
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
				"[{}, {}, {}, {}]",
				solid ? "Solid" : "Open",
				opaque ? "Opaque" : "Transperant",
				seen ? "Seen" : "Unseen",
				explored ? "Explored" : "Unexplored"
			);
		}

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void recalculate_index(cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, cell_e trait) noexcept {
			index = zone.template calculate_index<solver_e::Melded>(position, trait);
		}

		inline glyph_t floor_glyph() const noexcept { return glyph_t{ characters::Floor, color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		inline glyph_t wall_glyph() const noexcept { return glyph_t{ index, color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		inline glyph_t patch_glyph(u8 idx) const noexcept { return glyph_t{ idx, color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr u8 determine_patch(cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (zone.on_y_edge(position)) {
				return 0;
			}

			bool is_left_patch{};

			switch (index) {
				case 4:
				case 6: {
					is_left_patch = true;

					break;
				} case 8:
				  case 9: {
					is_left_patch = false;

					break;
				} default: {
					return 0;
				}
			}

			const u8 north_index{ zone[position + offset_t::North].index };

			if (north_index == 1 || north_index == 2 || north_index == 6 || north_index == 9) {
				return 0;
			}

			const bool is_alt_patch{ index == 6 || index == 9 };

			if (!is_left_patch) {
				return !is_alt_patch ? characters::RightPatch : characters::AltRightPatch;
			} else {
				return !is_alt_patch ? characters::LeftPatch : characters::AltLeftPatch;
			}

			return 0;
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (!explored) {
				return;
			}

			const u8 patch_character{ determine_patch(zone, position) };
			
			if (!patch_character) {
				return;
			}

			atlas.draw(patch_glyph(patch_character), position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (!explored) {
				return;
			}

			atlas.draw(floor_glyph(), position);

			if (!solid) {
				return;
			}

			atlas.draw(wall_glyph(), position);

			draw_patch(atlas, zone, position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			if (!explored) {
				return;
			}

			const u8 patch_character{ determine_patch(zone, position) };
			
			if (!patch_character) {
				return;
			}

			atlas.draw(patch_glyph(patch_character), position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			if (!explored) {
				return;
			}

			atlas.draw(floor_glyph(), position + offset);

			if (!solid) {
				return;
			}

			atlas.draw(wall_glyph(), position + offset);

			draw_patch(atlas, zone, position, offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (!explored) {
				return;
			}

			const u8 patch_character{ determine_patch(zone, position) };
			
			if (!patch_character) {
				return;
			}

			atlas.draw(patch_glyph(patch_character), position + offset, nudge);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref< zone_ct_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (!explored) {
				return;
			}

			atlas.draw(floor_glyph(), position + offset, nudge);

			if (!solid) {
				return;
			}

			atlas.draw(wall_glyph(), position + offset, nudge);

			draw_patch(atlas, zone, position, offset, nudge);
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
