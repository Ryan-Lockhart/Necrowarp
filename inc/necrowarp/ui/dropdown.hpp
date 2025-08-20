#pragma once

#include <bleak.hpp>

#include <array>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
    using namespace bleak;

    template<typename T> struct dropdown_option_t {
        T value;
        runes_t text;

        constexpr dropdown_option_t() noexcept : value{}, text{} {}

        constexpr dropdown_option_t(T value, cref<runes_t> text) noexcept : value{ value }, text{ text } {}
    };

    template<typename T, usize Count> struct embedded_dropdown_t {
      private:
        std::array<dropdown_option_t<T>, Count> options;
        
        usize index;

        bool expanded;

        bool previous_state;
        bool current_state;

        constexpr input_e get_state() const noexcept {
            if (previous_state) {
                return current_state ? input_e::Pressed : input_e::Up;
            } else {
                return current_state ? input_e::Down : input_e::Released;
            }
        }

      public:
        constexpr embedded_dropdown_t(cref<std::array<dropdown_option_t<T>, Count>> options) noexcept : options{ options }, index{ 0 }, expanded{ false }, previous_state{ false }, current_state{ false } {}

        constexpr bool is_expanded() const noexcept { return expanded; }

        constexpr T get_selected() const noexcept { return options[index].value; }

        constexpr bool set_selected(cref<T> value) noexcept {
            for (usize i{ 0 }; i < Count; ++i) {
                if (options[i].value == value) {
                    index = i;

                    return true;
                }
            }

            return false;
        }

        constexpr std::optional<T> operator[](usize index) const noexcept {
            if (index >= Count) {
                return std::nullopt;
            }

            return options[index].value;
        }

        constexpr bool is_hovered(offset_t position, extent_t size) const noexcept { 
            return mouse_s::is_inside(position * ui_atlas.get_glyph_size(), size * ui_atlas.get_glyph_size()); 
        }

        constexpr bool is_option_hovered(offset_t position, extent_t size, usize option_index) const noexcept {
            if (!expanded || option_index >= Count) {
                return false;
            }

            const offset_t option_pos{ position.x, position.y + size.h * (option_index + 1) };

            return mouse_s::is_inside(option_pos * ui_atlas.get_glyph_size(), size * ui_atlas.get_glyph_size());
        }

        constexpr void update(button_e button, offset_t position, extent_t size) noexcept {
            previous_state = current_state;
            current_state = mouse_s::is_button<input_e::Down>(button);

            if (!is_hovered(position, size) && !expanded) {
                return;
            }

            if (get_state() == input_e::Down) {
                if (is_hovered(position, size)) {
                    expanded = !expanded;
                } else if (expanded) {
                    for (usize i{ 0 }; i < Count; ++i) {
                        if (is_option_hovered(position, size, i)) {
                            index = i;
                            expanded = false;
                            return;
                        }
                    }

                    expanded = false;
                }
            }
        }

        constexpr void draw_header(ref<renderer_t> renderer, offset_t position, extent_t size, embedded_box_t box) const noexcept {
            const color_t header_bg{ expanded || is_hovered(position, size) ? box.background : box.background.dimmed(0.5f) };

            box.draw(renderer, position, size, header_bg);
            
            if (index < Count) {
                const extent_t text_size{ text::calculate_size(options[index].text) };

                const offset_t text_pos{
                    position.x + (size.w - text_size.w) / 2 + (text_size.w % 2 ? 1 : 0),
                    position.y + (size.h - text_size.h) / 2
                };

                ui_atlas.draw(options[index].text, text_pos);
            }
        }

        constexpr void draw_options(ref<renderer_t> renderer, offset_t position, extent_t size, embedded_box_t box) const noexcept {
            if (!expanded) {
                return;
            }

            for (usize i{ 0 }; i < Count; ++i) {
                const offset_t option_pos{ position.x, position.y + size.h * (i + 1) };

                const bool hovered{ is_option_hovered(position, size, i) };

                const color_t option_bg{ hovered ? box.background.dimmed(0.75f) : box.background.dimmed(0.5f) };
                
                box.draw(renderer, option_pos, size, option_bg);

                const extent_t text_size{ text::calculate_size(options[i].text) };

                const offset_t text_pos{
                    option_pos.x + (size.w - text_size.w) / 2 + (text_size.w % 2 ? 1 : 0),
                    option_pos.y + (size.h - text_size.h) / 2
                };

                ui_atlas.draw(options[i].text, text_pos);
            }
        }

        constexpr void draw(ref<renderer_t> renderer, offset_t position, extent_t size, embedded_box_t box) const noexcept {
            draw_header(renderer, position, size, box);
            draw_options(renderer, position, size, box);
        }
    };

    template<typename T, usize Count> class dropdown_t : public transform_t, public embedded_dropdown_t<T, Count>, public embedded_box_t {
      public:
        constexpr dropdown_t(transform_t transform, cref<std::array<dropdown_option_t<T>, Count>> options, embedded_box_t box) noexcept : transform_t{ transform }, embedded_dropdown_t<T, Count>{ options }, embedded_box_t{ box } {}
        
        constexpr bool is_hovered() const noexcept { 
            const offset_t pos{ get_offset() };

            if (embedded_dropdown_t<T, Count>::is_hovered(pos, size)) {
                return true;
            }

            if (embedded_dropdown_t<T, Count>::is_expanded()) {
                for (usize i{ 0 }; i < Count; ++i) {
                    if (embedded_dropdown_t<T, Count>::is_option_hovered(pos, size, i)) {
                        return true;
                    }
                }
            }

            return false;
        }

        constexpr extent_t full_size() const noexcept {
            return embedded_dropdown_t<T, Count>::is_expanded() ? extent_t{ size.w, size.h * (Count + 1) } : size;
        }

        constexpr void update(button_e button) noexcept { 
            embedded_dropdown_t<T, Count>::update(button, get_offset(), size); 
        }

        constexpr void draw(ref<renderer_t> renderer) const noexcept {
            embedded_dropdown_t<T, Count>::draw(renderer, get_offset(), size, *this);
        }
    };

    template<Enum E> class enum_dropdown_t : public dropdown_t<E, magic_enum::enum_count<E>()> {
      private:
        static constexpr usize count{ magic_enum::enum_count<E>() };

        using option_t = dropdown_option_t<E>;
        using self_t = dropdown_t<E, magic_enum::enum_count<E>()>;

        static constexpr std::array<option_t, count> generate_options() {
            std::array<option_t, count> options{};

            usize index{ 0 };

            magic_enum::enum_for_each<E>([&](auto val) {
                constexpr E cval{ val };

                options[index++] = dropdown_option_t<E>{ cval, to_colored_string(cval) };
            });

            return options;
        }

      public:
        constexpr enum_dropdown_t(transform_t transform, embedded_box_t box) noexcept : self_t{ transform, generate_options(), box } {}
    };
}
