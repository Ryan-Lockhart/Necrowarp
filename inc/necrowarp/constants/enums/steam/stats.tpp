#pragma once

#include <necrowarp/constants/enums/steam/stats.hpp>

#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	inline constexpr cstr to_string(stat_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr stat_e cval{ val };

			return stats::display_name<cval>;
		}, type);
	}

	inline constexpr cstr to_api_key(stat_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr stat_e cval{ val };

			return stats::api_key<cval>;
		}, type);
	}
} // namespace necrowarp
