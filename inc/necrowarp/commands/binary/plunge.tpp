#pragma once

#include <necrowarp/commands/binary/plunge.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	static inline dimension_e determine_dimension(stability_e stability) noexcept {
		switch (stability) {
			case stability_e::Calm: {
				return dimension_e::Voitykras;
			} case stability_e::Vocal: {
				return dimension_e::Sudokras;
			} case stability_e::Turbulent: {
				return dimension_e::Tribulation;
			} case stability_e::Insightful: {
				return dimension_e::Yyndikras;
			} case stability_e::Collapsing: {
				return dimension_e::Underworld;
			} case stability_e::Yawning: {
				return dimension_e::Overworld;
			} case stability_e::Echoing: {
				return dimension_e::AncientVault;
			}
		}
	}

	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, plunge_t>::process() const noexcept {
		if (!game_map<MapType>.dependent within<zone_region_e::Interior>(target_position) || !object_registry<MapType>.dependent contains<portal_t>(target_position)) {
			return;
		}

		cptr<portal_t> portal{ object_registry<MapType>.dependent at<portal_t>(target_position) };

		if (portal == nullptr || portal->stability == current_dimension) {
			return;
		}

		++steam_stats::stats<steam_stat_e::PortalsTraversed, i32>;

		plunge_target = determine_dimension(portal->stability);
		plunge_flag = true;
	}
} // namespace necrowarp
