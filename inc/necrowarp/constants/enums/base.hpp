#pragma once

#include <bleak/typedef.hpp>

#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct plurality_e : u8 {
		Singular,
		Multiple
	};
} // namespace necrowarp
