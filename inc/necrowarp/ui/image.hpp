#pragma once

#include <necrowarp/ui/base.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct embedded_image_t {
		ref<texture_t> texture;

		constexpr embedded_image_t(ref<texture_t> texture) noexcept : texture{ texture } {}

		constexpr bool is_hovered(offset_t position) const noexcept { return mouse_s::is_inside(position, texture.info.size); }

		constexpr void draw(offset_t position) const noexcept {
			texture.draw(position);
		}

		constexpr void draw(offset_t position, color_t color) const noexcept {
			texture.draw(position, color);
		}
	};

	class image_t : public anchor_t, public embedded_image_t {
	  public:
		constexpr image_t(embedded_image_t image) noexcept : anchor_t{}, embedded_image_t{ image } {}

		constexpr image_t(anchor_t anchor, embedded_image_t image) noexcept : anchor_t{ anchor }, embedded_image_t{ image } {}

		constexpr bool is_hovered() const noexcept { return embedded_image_t::is_hovered(get_offset(texture.info.size)); }

		constexpr void draw() const noexcept {
			embedded_image_t::draw(anchor_t::get_offset(texture.info.size));
		}
	};
}
