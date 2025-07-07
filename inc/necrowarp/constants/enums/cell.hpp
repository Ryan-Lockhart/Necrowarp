#pragma once

#include <necrowarp/constants/enums/base.hpp>

namespace necrowarp {
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
} // namespace necrowarp
