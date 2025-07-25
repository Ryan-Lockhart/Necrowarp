#pragma once

#include <bleak.hpp>

#include <bit>
#include <format>
#include <string>

#include <necrowarp/globals.hpp>

#include <necrowarp/constants/enums/fluid.tpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType> struct unified_map_renderer_s;

	struct fluid_cell_t {
		bool blood : 1 { false };
		bool ichor : 1 { false };
		bool filth : 1 { false };
		bool ectoplasm : 1 { false };

		u8 index : 4 { 0 };

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
				} case fluid_e::Filth: {
					return filth;
				} case fluid_e::BloodyFilth: {
					return blood && filth;
				} case fluid_e::IchorousFilth: {
					return ichor && filth;
				} case fluid_e::BloodyIchorousFilth: {
					return blood && ichor && filth;
				} case fluid_e::Ectoplasm: {
					return ectoplasm;
				} case fluid_e::BloodyEctoplasm: {
					return blood && ectoplasm;
				} case fluid_e::IchorousEctoplasm: {
					return ichor && ectoplasm;
				} case fluid_e::FilthyEctoplasm: {
					return filth && ectoplasm;
				} case fluid_e::BloodyIchorousEctoplasm: {
					return blood && ichor && ectoplasm;
				} case fluid_e::BloodyFilthyEctoplasm: {
					return blood && filth && ectoplasm;
				} case fluid_e::IchorousFilthyEctoplasm: {
					return ichor && filth && ectoplasm;
				} case fluid_e::BloodyIchorousFilthyEctoplasm: {
					return blood && ichor && filth && ectoplasm;
				}
			}
		}

		inline void set(fluid_e fluid) noexcept {
			switch (fluid) {
				case fluid_e::None: {
					blood = false;
					ichor = false;
					filth = false;
					ectoplasm = false;

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
				} case fluid_e::Filth: {
					filth = true;

					break;
				} case fluid_e::BloodyFilth: {
					blood = true;
					filth = true;

					break;
				} case fluid_e::IchorousFilth: {
					ichor = true;
					filth = true;

					break;
				} case fluid_e::BloodyIchorousFilth: {
					blood = true;
					ichor = true;
					filth = true;

					break;
				} case fluid_e::Ectoplasm: {
					ectoplasm = true;

					break;
				} case fluid_e::BloodyEctoplasm: {
					blood = true;
					ectoplasm = true;

					break;
				} case fluid_e::IchorousEctoplasm: {
					ichor = true;
					ectoplasm = true;

					break;
				} case fluid_e::FilthyEctoplasm: {
					filth = true;
					ectoplasm = true;

					break;
				} case fluid_e::BloodyIchorousEctoplasm: {
					blood = true;
					ichor = true;
					ectoplasm = true;

					break;
				} case fluid_e::BloodyFilthyEctoplasm: {
					blood = true;
					filth = true;
					ectoplasm = true;

					break;
				} case fluid_e::IchorousFilthyEctoplasm: {
					ichor = true;
					filth = true;
					ectoplasm = true;

					break;
				} case fluid_e::BloodyIchorousFilthyEctoplasm: {
					blood = true;
					ichor = true;
					filth = true;
					ectoplasm = true;

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
				} case fluid_e::Filth: {
					filth = false;

					break;
				} case fluid_e::BloodyFilth: {
					blood = false;
					filth = false;

					break;
				} case fluid_e::IchorousFilth: {
					ichor = false;
					filth = false;

					break;
				} case fluid_e::BloodyIchorousFilth: {
					blood = false;
					ichor = false;
					filth = false;

					break;
				} case fluid_e::Ectoplasm: {
					ectoplasm = false;

					break;
				} case fluid_e::BloodyEctoplasm: {
					blood = false;
					ectoplasm = false;

					break;
				} case fluid_e::IchorousEctoplasm: {
					ichor = false;
					ectoplasm = false;

					break;
				} case fluid_e::FilthyEctoplasm: {
					filth = false;
					ectoplasm = false;

					break;
				} case fluid_e::BloodyIchorousEctoplasm: {
					blood = false;
					ichor = false;
					ectoplasm = false;

					break;
				} case fluid_e::BloodyFilthyEctoplasm: {
					blood = false;
					filth = false;
					ectoplasm = false;

					break;
				} case fluid_e::IchorousFilthyEctoplasm: {
					ichor = false;
					filth = false;
					ectoplasm = false;

					break;
				} case fluid_e::BloodyIchorousFilthyEctoplasm: {
					blood = false;
					ichor = false;
					filth = false;
					ectoplasm = false;

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

		constexpr bool operator==(fluid_cell_t other) const noexcept {
			return blood == other.blood && ichor == other.ichor && filth == other.filth && ectoplasm == other.ectoplasm;
		}

		constexpr bool operator!=(fluid_cell_t other) const noexcept {
			return blood != other.blood || ichor != other.ichor || filth != other.filth || ectoplasm != other.ectoplasm;
		}

		constexpr bool operator==(fluid_e fluid) const noexcept { return contains(fluid); }

		constexpr bool operator!=(fluid_e fluid) const noexcept { return !contains(fluid); }

		constexpr explicit operator fluid_e() const noexcept {
			u8 fluid{ 0 };

			if (blood) { fluid |= static_cast<u8>(fluid_e::Blood); }
			if (ichor) { fluid |= static_cast<u8>(fluid_e::Ichor); }
			if (filth) { fluid |= static_cast<u8>(fluid_e::Filth); }
			if (ectoplasm) { fluid |= static_cast<u8>(fluid_e::Ectoplasm); }

			return static_cast<fluid_e>(fluid);
		}

		inline constexpr std::string to_tooltip() const {
			const fluid_e fluid{ static_cast<fluid_e>(*this) };

			return std::format("The cell is {}.", fluid == fluid_e::None ? "dry" : std::format("spattered with {}", to_string(fluid)));
		}

		inline constexpr operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}]",
				blood ? "Blood" : "Bloodless",
				ichor ? "Ichor" : "Ichorless",
				filth ? "Filth" : "Filthless",
				ectoplasm ? "Ectoplasm" : "Ectoplasmless"
			);
		}

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void recalculate_index(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, fluid_e fluid) noexcept {
			index = zone.dependent calculate_index<solver_e::MarchingSquares>(position, fluid);
		}

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder, typename Predicate>
			requires std::is_invocable<Predicate, T>::value
		inline constexpr void recalculate_index(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, rval<Predicate> predicate) noexcept {
			index = zone.dependent calculate_index<solver_e::MarchingSquares>(position, std::forward<Predicate>(predicate));
		}

		inline constexpr u8 pool_index() const noexcept { return static_cast<u8>(characters::FluidTileOrigin + index); }

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr glyph_t pool_glyph(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			const cardinal_t edge_state{ zone.edge_state(position) };

			fluid_e constituents{ static_cast<fluid_e>(zone[position]) };

			if (!edge_state.east) {
				constituents += static_cast<fluid_e>(zone[position + offset_t::East]);
			}

			if (!edge_state.south) {
				constituents += static_cast<fluid_e>(zone[position + offset_t::South]);
			}

			if (!edge_state.south && !edge_state.east) {
				constituents += static_cast<fluid_e>(zone[position + offset_t::Southeast]);
			}

			return glyph_t{ pool_index(), to_color(constituents) };
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_pool(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			atlas.draw(pool_glyph(zone, position), position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			draw_pool(atlas, zone, position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_pool(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			atlas.draw(pool_glyph(zone, position), position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			draw_pool(atlas, zone, position, offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_pool(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			atlas.draw(pool_glyph(zone, position), position + offset, nudge);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			draw_pool(atlas, zone, position, offset, nudge);
		}

		struct hasher {
			static constexpr usize operator()(fluid_cell_t cell) noexcept { return hash_combine(std::bit_cast<u8>(cell)); }
		};

		template<map_type_e MapType> friend struct unified_map_renderer_s;
	};

	static constexpr bool operator==(fluid_e fluid, fluid_cell_t cell) noexcept { return cell.contains(fluid); }

	static constexpr bool operator!=(fluid_e fluid, fluid_cell_t cell) noexcept { return !cell.contains(fluid); }

	struct map_cell_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };

		u8 index : 4 { 0 };

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

		constexpr bool operator==(cell_e trait) const noexcept { return contains(trait); }

		constexpr bool operator!=(cell_e trait) const noexcept { return !contains(trait); }

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
		inline constexpr void recalculate_index(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, cell_e trait) noexcept {
			index = zone.dependent calculate_index<solver_e::MarchingSquares>(position, trait);
		}

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder, typename Predicate>
			requires std::is_invocable<Predicate, T>::value
		inline constexpr void recalculate_index(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, rval<Predicate> predicate) noexcept {
			index = zone.dependent calculate_index<solver_e::MarchingSquares>(position, std::forward<Predicate>(predicate));
		}

		inline glyph_t floor_glyph() const noexcept { return glyph_t{ characters::Floor, color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		inline glyph_t wall_glyph() const noexcept { return glyph_t{ static_cast<u8>(characters::CavernTileOrigin + index), color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		inline glyph_t patch_glyph(u8 idx) const noexcept { return glyph_t{ idx, color_t{ 0xFF, seen ? u8{ 0xFF } : u8{ 0x80 } } }; }

		template<typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr u8 determine_patch(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (zone.on_y_edge(position)) {
				return 0;
			}

			bool is_left_patch{};

			switch (index) {
				case 4:
				case 5:
				case 6:
				case 7: {
					is_left_patch = true;

					break;
				} case 8:
				  case 9:
				  case 10:
				  case 11: {
					is_left_patch = false;

					break;
				} default: {
					return 0;
				}
			}

			const u8 north_index{ zone[position + offset_t::North].index };

			switch (north_index) {
				case  1:
				case  2:
				case  5:
				case  6:
				case  9:
				case 10: {
					return 0;
				} default: {
					break;
				}
			}

			const bool is_alt_patch{ index == 6 || index == 9 };
			const bool is_cropped_patch{ index == 7 || index == 11 };

			if (!is_left_patch) {
				return !is_cropped_patch ? !is_alt_patch ? characters::RightPatch : characters::AltRightPatch : characters::CroppedRightPatch;
			} else {
				return !is_cropped_patch ? !is_alt_patch ? characters::LeftPatch : characters::AltLeftPatch : characters::CroppedLeftPatch;
			}

			return 0;
		}

		template<extent_t AtlasSize> inline constexpr void draw_floor(cref<atlas_t<AtlasSize>> atlas, offset_t position) const noexcept {
			atlas.draw(floor_glyph(), position);
		}

		template<extent_t AtlasSize> inline constexpr void draw_wall(cref<atlas_t<AtlasSize>> atlas, offset_t position) const noexcept {
			if (index != 0) {
				atlas.draw(wall_glyph(), position);
			}
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			const u8 patch_character{ determine_patch(zone, position) };
			
			if (!patch_character) {
				return;
			}

			atlas.draw(patch_glyph(patch_character), position);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position) const noexcept {
			if (!explored) {
				return;
			}

			draw_floor(atlas, position);

			draw_wall(atlas, position);

			draw_patch(atlas, zone, position);
		}

		template<extent_t AtlasSize> inline constexpr void draw_floor(cref<atlas_t<AtlasSize>> atlas, offset_t position, offset_t offset) const noexcept {
			atlas.draw(floor_glyph(), position + offset);
		}

		template<extent_t AtlasSize> inline constexpr void draw_wall(cref<atlas_t<AtlasSize>> atlas, offset_t position, offset_t offset) const noexcept {
			if (index != 0) {
				atlas.draw(wall_glyph(), position + offset);
			}
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			const u8 patch_character{ determine_patch(zone, position) };
			
			if (!patch_character) {
				return;
			}

			atlas.draw(patch_glyph(patch_character), position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset) const noexcept {
			if (!explored) {
				return;
			}

			draw_floor(atlas, position, offset);

			draw_wall(atlas, position, offset);

			draw_patch(atlas, zone, position, offset);
		}

		template<extent_t AtlasSize> inline constexpr void draw_floor(cref<atlas_t<AtlasSize>> atlas, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			atlas.draw(floor_glyph(), position + offset, nudge);
		}

		template<extent_t AtlasSize> inline constexpr void draw_wall(cref<atlas_t<AtlasSize>> atlas, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (index != 0) {
				atlas.draw(wall_glyph(), position + offset, nudge);
			}
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw_patch(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
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
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, offset_t position, offset_t offset, offset_t nudge) const noexcept {
			if (!explored) {
				return;
			}

			draw_floor(atlas, position, offset, nudge);

			draw_wall(atlas, position, offset, nudge);

			draw_patch(atlas, zone, position, offset, nudge);
		}

		struct hasher {
			static constexpr usize operator()(map_cell_t cell) noexcept { return hash_combine(std::bit_cast<u8>(cell)); }
		};

		template<map_type_e MapType> friend struct unified_map_renderer_s;
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
