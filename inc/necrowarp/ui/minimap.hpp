#pragma once

#include <bleak.hpp>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<extent_t ZoneSize, extent_t BorderSize, extent_t PixelSize = extent_t{ 1, 1 }> struct embedded_minimap_t {
		embedded_box_t box;
		extent_t padding;

		constexpr embedded_minimap_t() noexcept : box{}, padding{} {}

		constexpr embedded_minimap_t(embedded_box_t box, extent_t padding) noexcept : box{ box }, padding{ padding } {}

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr extent_t internal_size() const noexcept { return ZoneSize * PixelSize; }

		constexpr extent_t external_size() const noexcept { return internal_size() + padding * 2; }

		constexpr bool is_hovered(offset_t position) const noexcept { return Mouse::is_inside(apply_padding(position), external_size()); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			renderer.draw_composite_rect(rect_t{ apply_padding(position), external_size() }, box.background, box.border.color, box.border.thickness * globals::cell_size<grid_type_e::UI>.w);
			
			for (extent_t::scalar_t y{ 0 }; y < ZoneSize.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < ZoneSize.w; ++x) {
					const offset_t pos{ x, y };
					const offset_t pixel_pos{ position + (pos * PixelSize) };

					const color_t pixel_color{ [&]() -> color_t {
						if (entity_registry.contains<player_t>(pos)) {
							return colors::Magenta;
						} else if (entity_registry.contains<ALL_EVIL_NPCS>(pos)) {
							return colors::Green;
						} else if (entity_registry.contains<ALL_GOOD_NPCS>(pos)) {
							return colors::Red;
						} else if (object_registry.contains(pos)) {
							return colors::Blue;
						} else if (game_map[pos].solid) {
							return colors::Charcoal;
						} else if (!fluid_map[pos].contains(fluid_type_e::None)) {
							return fluid_color(static_cast<fluid_type_e>(fluid_map[pos]));
						} else {
							return colors::Marble;
						}
					}()};

					if constexpr (PixelSize == extent_t{ 1, 1 }) {
						renderer.draw_point(pixel_pos, pixel_color);
					} else {
						renderer.draw_fill_rect(rect_t{ pixel_pos, PixelSize }, pixel_color);
					}
				}
			}

			renderer.draw_outline_rect(rect_t{ position + camera.get_position() * PixelSize, camera.get_size() * PixelSize }, colors::metals::Gold);
		}
	};
	
	template<extent_t ZoneSize, extent_t BorderSize, extent_t PixelSize = extent_t{ 1, 1 }> struct minimap_t : public anchor_t, public embedded_minimap_t<ZoneSize, BorderSize, PixelSize> {
		constexpr minimap_t() noexcept : anchor_t{}, embedded_minimap_t<ZoneSize, BorderSize, PixelSize>{} {}

		constexpr minimap_t(anchor_t anchor, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_minimap_t<ZoneSize, BorderSize, PixelSize>{ box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_minimap_t<ZoneSize, BorderSize, PixelSize>::is_hovered(anchor_t::get_offset(embedded_minimap_t<ZoneSize, BorderSize, PixelSize>::external_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept { embedded_minimap_t<ZoneSize, BorderSize, PixelSize>::draw(renderer, get_offset(embedded_minimap_t<ZoneSize, BorderSize, PixelSize>::external_size())); }
	};
}
