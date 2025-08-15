#pragma once

#include <necrowarp/constants/enums/steam/achievements.hpp>

#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	inline constexpr cstr to_string(achievement_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr achievement_e cval{ val };

			return achievements::display_name<cval>;
		}, type);
	}

	inline constexpr cstr to_api_key(achievement_e type) noexcept {
		return magic_enum::enum_switch([&](auto val) -> cstr {
			constexpr achievement_e cval{ val };

			return achievements::api_key<cval>;
		}, type);
	}
} // namespace necrowarp
