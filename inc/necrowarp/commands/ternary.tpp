#pragma once

// IWYU pragma: begin_exports
#include <necrowarp/commands/ternary/lunge.tpp>
// IWYU pragma: end_exports

namespace necrowarp {
	template struct entity_command_t<bloodhound_t, lunge_t>;
	template void entity_command_t<bloodhound_t, lunge_t>::process<map_type_e::Pocket>() const noexcept;
} // namespace necrowarp
