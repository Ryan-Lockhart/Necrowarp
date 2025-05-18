#pragma once

#include <necrowarp/ui/base.hpp>

#include <necrowarp/ui/label.hpp>
#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct labeled_button_t;
	struct labeled_toggle_t;

	struct embedded_button_t {
		bool previous_state;
		bool current_state;

		constexpr embedded_button_t() noexcept : previous_state{ false }, current_state{ false } {}

		constexpr input_state_t get_state() const noexcept {
			if (previous_state) {
				return current_state ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state ? input_state_t::Down : input_state_t::Released;
			}
		}

		constexpr bool is_active() const noexcept { return current_state; }

		constexpr bool is_hovered(offset_t position, extent_t size) const noexcept { return Mouse::is_inside(position * ui_atlas.get_glyph_size(), size * ui_atlas.get_glyph_size()); }

		constexpr void update(Mouse::button_t button, offset_t position, extent_t size) noexcept {
			previous_state = current_state;
			current_state = Mouse::is_button_down(button) && is_hovered(position, size);
		}
	};

	struct button_t : public transform_t, public embedded_button_t, public embedded_box_t {
		constexpr button_t() noexcept : transform_t{}, embedded_button_t{}, embedded_box_t{} {}

		constexpr button_t(transform_t transform, embedded_box_t box) noexcept : transform_t{ transform }, embedded_button_t{}, embedded_box_t{ box } {}

		constexpr bool is_hovered() const noexcept { return embedded_box_t::is_hovered(get_offset(), size); }

		constexpr color_t current_background() const noexcept { return is_active() ? background : is_hovered() ? background.dimmed(0.75f) : background.dimmed(0.5f); }

		constexpr void update(Mouse::button_t button) noexcept { embedded_button_t::update(button, get_offset(), size); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_box_t::draw(renderer, get_offset(), size, current_background());
		}
	};

	struct embedded_toggle_t {
	  private:
		bool previous_state;
		bool current_state;

		bool toggled;

		constexpr input_state_t get_state() const noexcept {
			if (previous_state) {
				return current_state ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state ? input_state_t::Down : input_state_t::Released;
			}
		}

	  public:
		constexpr bool is_toggled() const noexcept { return toggled; }

		constexpr void toggle_on() noexcept { toggled = true; }
		constexpr void toggle_off() noexcept { toggled = false; }

		constexpr embedded_toggle_t() noexcept : previous_state{ false }, current_state{ false }, toggled{ false } {}

		constexpr embedded_toggle_t(bool toggled) noexcept : previous_state{ false }, current_state{ false }, toggled{ toggled } {}

		constexpr bool is_active() const noexcept { return current_state || toggled; }

		constexpr bool is_hovered(offset_t position, extent_t size) const noexcept { return Mouse::is_inside(position * ui_atlas.get_glyph_size(), size * ui_atlas.get_glyph_size()); }

		constexpr void update(Mouse::button_t button, offset_t position, extent_t size) noexcept {
			previous_state = current_state;
			current_state = Mouse::is_button_down(button) && is_hovered(position, size);

			if (get_state() == input_state_t::Down) {
				toggled = !toggled;
			}
		}
		
		template<typename... Linked>
			requires is_homogeneous<embedded_toggle_t, Linked...>::value
		constexpr void update(Mouse::button_t button, offset_t position, extent_t size, ptr<Linked>... linked) noexcept {
			previous_state = current_state;
			current_state = Mouse::is_button_down(button) && is_hovered(position, size);

			if (get_state() == input_state_t::Down) {
				toggled = !toggled;

				if (!toggled) {
					return;
				}

				for (ptr<embedded_toggle_t> linked_toggle : std::array<ptr<embedded_toggle_t>, sizeof...(Linked)>{ linked... }) {
					linked_toggle->toggle_off();
				}
			}
		}
		
		template<typename... Linked>
			requires is_homogeneous<labeled_toggle_t, Linked...>::value
		constexpr void update(Mouse::button_t button, offset_t position, extent_t size, ptr<Linked>... linked) noexcept;
	};

	struct toggle_t : public transform_t, public embedded_toggle_t, public embedded_box_t {
		constexpr toggle_t() noexcept : transform_t{}, embedded_toggle_t{}, embedded_box_t{} {}

		constexpr toggle_t(transform_t transform, embedded_box_t box) noexcept : transform_t{ transform }, embedded_toggle_t{}, embedded_box_t{ box } {}

		constexpr toggle_t(bool toggled) noexcept : transform_t{}, embedded_toggle_t{ toggled }, embedded_box_t{} {}

		constexpr toggle_t(transform_t transform, embedded_box_t box, bool toggled) noexcept : transform_t{ transform }, embedded_toggle_t{ toggled }, embedded_box_t{ box } {}

		constexpr bool is_hovered() const noexcept { return embedded_box_t::is_hovered(get_offset(), size); }

		constexpr color_t current_background() const noexcept { return is_active() ? background : is_hovered() ? background.dimmed(0.75f) : background.dimmed(0.5f); }

		constexpr void update(Mouse::button_t button) noexcept { embedded_toggle_t::update(button, get_offset(), size); }

		template<typename... Linked>
			requires is_homogeneous<embedded_toggle_t, Linked...>::value
		constexpr void update(Mouse::button_t button, Linked... linked) noexcept { embedded_toggle_t::update(button, get_offset(), size, linked...); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_box_t::draw(renderer, get_offset(), size, current_background());
		}
	};
} // namespace necrowarp
