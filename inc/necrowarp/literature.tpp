#pragma once

#include <necrowarp/literature.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/object_state.tpp>

namespace necrowarp {
    template<map_type_e MapType> static inline bool literature::is_softlocked() noexcept {
        for (crauto [position, ladder] : object_registry_storage<ladder_t>) {
            if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || ladder.is_up_ladder() || !ladder.has_shackle()) {
                continue;
            }

            const bool can_break{
                magic_enum::enum_switch([&](auto val) -> bool {
                    constexpr shackle_e cval{ val };

                    return literature::is_acquired(shackle_breaker<cval>);
                }, ladder.shackle)
            };

            if (can_break) {
                return false;
            }
        }

        return true;
    }
} // namespace necrowarp
