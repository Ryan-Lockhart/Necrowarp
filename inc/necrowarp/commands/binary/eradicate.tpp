#pragma once

#include <necrowarp/commands/unary/eviscerate.hpp>

#include <necrowarp/commands/binary/eradicate.hpp>
#include <necrowarp/commands/binary/clash.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, eradicate_t>::process() const noexcept {
		entity_command_t<EntityType, move_t>{ source_position, target_position }.dependent process<MapType>();

		ptr<EntityType> maybe_isoscel{ entity_registry<MapType>.dependent at<EntityType>(target_position) };

		if (maybe_isoscel == nullptr) {
			return;
		}

		entity_command_t<EntityType, eviscerate_t>{ target_position }.dependent process<MapType>();

		if (!maybe_isoscel->destabilize()) {
			return;
		}

		const death_info_t<death_e::Killed> info{ maybe_isoscel->dependent die<MapType, death_e::Killed>(target_position) };

		if (info.perished) {
			entity_registry<MapType>.dependent remove<EntityType>(target_position);
		}
	}
} // namespace necrowarp
