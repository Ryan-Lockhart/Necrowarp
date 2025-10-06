#pragma once

#include <necrowarp/game_state.hpp>
#include <necrowarp/objects/catalyst/bones.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

namespace necrowarp {
		template<map_type_e MapType> inline bool bones_t::decay(offset_t position) noexcept {
            if (lifespan > 0) {
                --lifespan;

                return false;
            }

            if (!is<decay_e::Rotted>()) {
                state = necrowarp::decay(state);

                lifespan = determine_lifespan(state);

                return false;
            }

            spill_fluid<MapType>(position, fluid_e::Ichor);

            return true;
        }
} // namespace necrowarp
