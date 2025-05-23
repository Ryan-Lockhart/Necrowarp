#pragma once

#include <bleak.hpp>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct status_t {
		static constexpr u8 left_segment{ 1 };
		static constexpr u8 center_segment{ 2 };
		static constexpr u8 right_segment{ 3 };
		
		runes_t label;
		
		color_t active_color;
		color_t inactive_color;
		
		u8 max_value{ 16 };
		u8 current_value;

		inline u8 get_segment(u8 value) const noexcept {
			return value != 0 ? value == max_value - 1 ? status_t::right_segment : status_t::center_segment : status_t::left_segment;
		}

		constexpr status_t(cref<runes_t> label, u8 max_value, color_t active_color, color_t inactive_color) noexcept : label{ label }, active_color{ active_color }, inactive_color{ inactive_color }, max_value{ max_value }, current_value{} {}

		constexpr status_t(cref<runes_t> label, u8 max_value, u8 current_value, color_t active_color, color_t inactive_color) noexcept : label{ label }, active_color{ active_color }, inactive_color{ inactive_color }, max_value{ max_value }, current_value{} {}
	};

	template<usize Statuses> struct embedded_status_bar_t {
		std::array<status_t, Statuses> statuses;

		embedded_box_t box;

		extent_t padding;

		constexpr embedded_status_bar_t(std::array<status_t, Statuses> statuses) noexcept : statuses{ statuses } {}

		constexpr embedded_status_bar_t(std::array<status_t, Statuses> statuses, embedded_box_t box, extent_t padding) noexcept : statuses{ statuses }, box{ box }, padding{ padding } {}

		constexpr cref<status_t> operator[](usize index) const noexcept { return statuses[index]; }
		constexpr ref<status_t> operator[](usize index) noexcept { return statuses[index]; }

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr usize count() const noexcept { return Statuses; }

		constexpr u8 max_length() const noexcept {
			u8 max_length{};

			for (u8 i{ 0 }; i < Statuses; ++i) {
				max_length = std::max(max_length, static_cast<u8>(text::calculate_size(statuses[i].label).w + statuses[i].max_value));
			}

			return max_length;
		}

		constexpr extent_t internal_size() const noexcept { return extent_t{ max_length(), Statuses }; }

		constexpr extent_t external_size() const noexcept { return extent_t{ max_length(), Statuses } + padding * 2 + extent_t{ 0, Statuses - 1 }; }

		constexpr bool is_hovered(offset_t position) const noexcept { return box.is_hovered(apply_padding(position), external_size()); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), external_size());

			runes_t runes{};

			for (usize i{ 0 }; i < Statuses; ++i) {
				runes.concatenate(statuses[i].label);

				for (usize j{ 0 }; j < statuses[i].max_value; ++j) {
					runes += glyph_t{ statuses[i].get_segment(j), statuses[i].current_value > j ? statuses[i].active_color : statuses[i].inactive_color };
				}

				if (i < Statuses - 1) {
					runes.concatenate(runes_t{ "\n\n", colors::White });
				}
			}

			ui_atlas.draw(runes, position);
		}
	};
	
	template<usize Statuses> struct status_bar_t : public anchor_t, public embedded_status_bar_t<Statuses> {
		constexpr status_bar_t(std::array<status_t, Statuses> statuses) noexcept : anchor_t{}, embedded_status_bar_t<Statuses>{ statuses } {}

		constexpr status_bar_t(anchor_t anchor, std::array<status_t, Statuses> statuses, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_status_bar_t<Statuses>{ statuses, box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_status_bar_t<Statuses>::is_hovered(anchor_t::get_offset(embedded_status_bar_t<Statuses>::external_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept { embedded_status_bar_t<Statuses>::draw(renderer, get_offset(embedded_status_bar_t<Statuses>::external_size())); }
	};
}
