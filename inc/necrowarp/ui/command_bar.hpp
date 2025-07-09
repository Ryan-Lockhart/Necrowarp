#pragma once

#include <bleak.hpp>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct embedded_command_icon_t {
		embedded_box_t box;
		extent_t padding;
		glyph_t glyph;

		constexpr embedded_command_icon_t(glyph_t glyph) noexcept : box{}, padding{ 1, 1 }, glyph{ glyph } {}

		constexpr embedded_command_icon_t(glyph_t glyph, embedded_box_t box, extent_t padding) noexcept : box{ box }, padding{ padding }, glyph{ glyph } {}

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr extent_t calculate_size() const noexcept { return globals::cell_size<grid_type_e::Icon> / globals::cell_size<grid_type_e::UI> + padding * 2; }

		constexpr bool is_hovered(offset_t position) const noexcept { return box.is_hovered(apply_padding(position), calculate_size()); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size());
			icon_atlas.draw(glyph, position);
		}

		constexpr void draw(ref<renderer_t> renderer, offset_t position, color_t background) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size(), background);
			icon_atlas.draw(glyph, position);
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size());
			atlas.draw(glyph, position);
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, offset_t position, color_t background) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size(), background);
			atlas.draw(glyph, position);
		}
	};

	class command_icon_t : public anchor_t, public embedded_command_icon_t {
	  public:
		constexpr command_icon_t(glyph_t glyph) noexcept : anchor_t{}, embedded_command_icon_t{ glyph } {}

		constexpr command_icon_t(anchor_t anchor, glyph_t glyph, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_command_icon_t{ glyph, box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_command_icon_t::is_hovered(anchor_t::get_offset(embedded_command_icon_t::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_command_icon_t::draw(renderer, anchor_t::get_offset(embedded_command_icon_t::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer, color_t background) const noexcept {
			embedded_command_icon_t::draw(renderer, anchor_t::get_offset(embedded_command_icon_t::calculate_size()), background);
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas) const noexcept {
			embedded_command_icon_t::draw(renderer, atlas, anchor_t::get_offset(embedded_command_icon_t::calculate_size()));
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, color_t background) const noexcept {
			embedded_command_icon_t::draw(renderer, atlas, anchor_t::get_offset(embedded_command_icon_t::calculate_size()), background);
		}
	};

	template<usize Commands> struct embedded_command_bar_t {
		std::array<embedded_command_icon_t, Commands> commands;

		embedded_box_t box;
		extent_t padding;

		constexpr embedded_command_bar_t(cref<std::array<embedded_command_icon_t, Commands>> commands) noexcept : commands{ commands }, box{}, padding{ 1, 1 } {}

		constexpr embedded_command_bar_t(cref<std::array<embedded_command_icon_t, Commands>> commands, embedded_box_t box, extent_t padding) noexcept : commands{ commands }, box{ box }, padding{ padding } {}

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr extent_t calculate_size() const noexcept {
			extent_t size{};

			for (usize i{ 0 }; i < Commands; ++i) {
				size += commands[i].calculate_size();
			}

			return size + padding * 2;
		}

		constexpr bool is_hovered(offset_t position) const noexcept { return box.is_hovered(apply_padding(position), calculate_size()); }

		constexpr offset_t get_position(offset_t position, usize index) const noexcept {
			for (usize i{ 0 }; i < index; ++i) {
				position.y += commands[i].calculate_size().h;
			}

			return position;
		}

		constexpr bool any_hovered(offset_t position) const noexcept {
			for (usize i{ 0 }; i < Commands; ++i) {
				if (commands[i].is_hovered(position)) {
					return true;
				}

				position.y += commands[i].calculate_size().h;
			}

			return false;
		}

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size());

			for (usize i{ 0 }; i < Commands; ++i) {
				commands[i].draw(renderer, position);

				position.y += commands[i].calculate_size().h;
			}
		}

		constexpr void draw(ref<renderer_t> renderer, offset_t position, color_t background) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size(), background);

			for (usize i{ 0 }; i < Commands; ++i) {
				commands[i].draw(renderer, position);

				position.y += commands[i].calculate_size().h;
			}
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size());

			for (usize i{ 0 }; i < Commands; ++i) {
				commands[i].draw(renderer, position);

				position.y += commands[i].calculate_size().h;
			}
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, offset_t position, color_t background) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size(), background);

			for (usize i{ 0 }; i < Commands; ++i) {
				commands[i].draw(renderer, position);

				position.y += commands[i].calculate_size().h;
			}
		}
	};

	template<usize Commands> class command_bar_t : public anchor_t, public embedded_command_bar_t<Commands> {
	  public:
		constexpr command_bar_t(cref<std::array<embedded_command_icon_t, Commands>> commands) noexcept : anchor_t{}, embedded_command_bar_t<Commands>{ commands } {}

		constexpr command_bar_t(anchor_t anchor, cref<std::array<embedded_command_icon_t, Commands>> commands, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_command_bar_t<Commands>{ commands, box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_command_bar_t<Commands>::is_hovered(anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()));
		}

		constexpr bool any_hovered() const noexcept {
			return embedded_command_bar_t<Commands>::any_hovered(anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_command_bar_t<Commands>::draw(renderer, anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer, color_t background) const noexcept {
			embedded_command_bar_t<Commands>::draw(renderer, anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()), background);
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas) const noexcept {
			embedded_command_bar_t<Commands>::draw(renderer, atlas, anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()));
		}

		template<extent_t AtlasSize> constexpr void draw(ref<renderer_t> renderer, ref<atlas_t<AtlasSize>> atlas, color_t background) const noexcept {
			embedded_command_bar_t<Commands>::draw(renderer, atlas, anchor_t::get_offset(embedded_command_bar_t<Commands>::calculate_size()), background);
		}
	};
}
