#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	using namespace bleak;

	struct labeled_button_t : public anchor_t, public embedded_button_t, public embedded_label_t {
		constexpr labeled_button_t() noexcept : anchor_t{}, embedded_button_t{}, embedded_label_t{} {}

		constexpr labeled_button_t(anchor_t anchor, embedded_label_t label) noexcept : anchor_t{ anchor }, embedded_button_t{}, embedded_label_t{ label } {}

		constexpr bool is_hovered() const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			return embedded_button_t::is_hovered(current_pos, current_size);
		}

		constexpr void update(mouse_s::button_t button) noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			embedded_button_t::update(button, current_pos, current_size);
		}

		constexpr color_t current_background() const noexcept { return is_active() ? box.background : is_hovered() ? box.background.dimmed(0.75f) : box.background.dimmed(0.5f); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = get_offset(current_size);

			embedded_label_t::draw(renderer, current_pos, current_background());
		}
	};

	struct labeled_toggle_t : public anchor_t, public embedded_toggle_t, public embedded_label_t {
		constexpr labeled_toggle_t() noexcept : anchor_t{}, embedded_toggle_t{}, embedded_label_t{} {}

		constexpr labeled_toggle_t(anchor_t anchor, embedded_label_t label) noexcept : anchor_t{ anchor }, embedded_toggle_t{}, embedded_label_t{ label } {}

		constexpr labeled_toggle_t(bool toggled) noexcept : anchor_t{}, embedded_toggle_t{ toggled }, embedded_label_t{} {}

		constexpr labeled_toggle_t(anchor_t anchor, embedded_label_t label, bool toggled) noexcept : anchor_t{ anchor }, embedded_toggle_t{ toggled }, embedded_label_t{ label } {}

		constexpr bool is_hovered() const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			return embedded_toggle_t::is_hovered(current_pos, current_size);
		}

		constexpr void update(mouse_s::button_t button) noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			embedded_toggle_t::update(button, current_pos, current_size);
		}

		template<typename... Linked>
			requires is_homogeneous<labeled_toggle_t, Linked...>::value
		constexpr void update(mouse_s::button_t button, ptr<Linked>... linked) noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			embedded_toggle_t::update(button, current_pos, current_size, linked...);
		}

		constexpr color_t current_background() const noexcept { return is_active() ? box.background : is_hovered() ? box.background.dimmed(0.75f) : box.background.dimmed(0.5f); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = get_offset(current_size);

			embedded_label_t::draw(renderer, current_pos, current_background());
		}
	};

	template<typename... Linked>
		requires is_homogeneous<labeled_toggle_t, Linked...>::value
	constexpr void embedded_toggle_t::update(mouse_s::button_t button, offset_t position, extent_t size, ptr<Linked>... linked) noexcept {
		previous_state = current_state;
		current_state = mouse_s::is_button_down(button) && is_hovered(position, size);

		if (get_state() == input_e::Down) {
			toggled = !toggled;

			if (!toggled) {
				return;
			}

			for (ptr<labeled_toggle_t> linked_toggle : std::array<ptr<labeled_toggle_t>, sizeof...(Linked)>{ linked... }) {
				linked_toggle->toggle_off();
			}
		}
	}
}
