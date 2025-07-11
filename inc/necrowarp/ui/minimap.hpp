#pragma once

#include <bleak.hpp>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType, extent_t PixelSize = extent_t{ 1, 1 }> struct embedded_minimap_t {
		embedded_box_t box;
		extent_t padding;

		constexpr embedded_minimap_t() noexcept : box{}, padding{} {}

		constexpr embedded_minimap_t(embedded_box_t box, extent_t padding) noexcept : box{ box }, padding{ padding } {}

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr extent_t internal_size() const noexcept { return globals::MapSize<MapType> * PixelSize; }

		constexpr extent_t external_size() const noexcept { return internal_size() + padding * 2; }

		constexpr bool is_hovered(offset_t position) const noexcept { return mouse_s::is_inside(apply_padding(position), external_size()); }

		template<typename Entities, typename Objects> constexpr void draw(ref<renderer_t> renderer, cref<Entities> entities, cref<Objects> objects, offset_t position) const noexcept {
			renderer.draw_composite_rect(rect_t{ apply_padding(position), external_size() }, box.background, box.border.color, box.border.thickness * globals::cell_size<grid_type_e::UI>.w);
			
			for (extent_t::scalar_t y{ 0 }; y < globals::MapSize<MapType>.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < globals::MapSize<MapType>.w; ++x) {
					const offset_t pos{ x, y };
					const offset_t pixel_pos{ position + (pos * PixelSize) };

					const color_t pixel_color{
						[&]() -> color_t {
							if (entities.dependent contains<player_t>(pos)) {
								return colors::Magenta;
							} else if (entities.dependent contains<ALL_GOOD_NPCS>(pos)) {
								return colors::Red;
							} else if (entities.dependent contains<ALL_NEUTRAL_NPCS>(pos)) {
								return colors::Orange;
							} else if (entities.dependent contains<ALL_EVIL_NPCS>(pos)) {
								return colors::Green;
							} else if (objects.contains(pos)) {
								return colors::Blue;
							} else if (game_map<MapType>[pos].solid) {
								return colors::light::Charcoal;
							} else if (!fluid_map<MapType>[pos].contains(fluid_e::None)) {
								return to_color(static_cast<fluid_e>(fluid_map<MapType>[pos]));
							} else {
								return colors::dark::Marble;
							}
						}()
					};

					if constexpr (PixelSize == extent_t{ 1, 1 }) {
						renderer.draw_point(pixel_pos, pixel_color);
					} else {
						renderer.draw_fill_rect(rect_t{ pixel_pos, PixelSize }, pixel_color);
					}
				}
			}

			renderer.draw_outline_rect(rect_t{ position + camera<MapType>.get_position() * PixelSize, camera<MapType>.get_size() * PixelSize }, colors::metals::Gold);
		}
	};
	
	template<map_type_e MapType, extent_t PixelSize = extent_t{ 1, 1 }> class minimap_t : public anchor_t, public embedded_minimap_t<MapType, PixelSize> {
	  public:
		constexpr minimap_t() noexcept : anchor_t{}, embedded_minimap_t<MapType, PixelSize>{} {}

		constexpr minimap_t(anchor_t anchor, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_minimap_t<MapType, PixelSize>{ box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_minimap_t<MapType, PixelSize>::is_hovered(anchor_t::get_offset(embedded_minimap_t<MapType, PixelSize>::external_size()));
		}

		template<typename Entities, typename Objects> constexpr void draw(ref<renderer_t> renderer, cref<Entities> entities, cref<Objects> objects) const noexcept {
			embedded_minimap_t<MapType, PixelSize>::draw(renderer, entities, objects, get_offset(embedded_minimap_t<MapType, PixelSize>::external_size()));
		}
	};
}
