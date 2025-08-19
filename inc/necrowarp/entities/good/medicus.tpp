#pragma once

#include <necrowarp/entities/good/medicus.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/scorekeeper.hpp>

namespace necrowarp {
	template<map_type_e MapType> inline std::optional<entity_e> medicus_t::can_resuscitate(offset_t position) const noexcept {
		if (!has_satchels()) {
			return std::nullopt;
		}

		if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position)) {
			return std::nullopt;
		}

		if (object_registry<MapType>.dependent empty<cerebra_t>(position) && object_registry<MapType>.dependent empty<flesh_t>(position) && object_registry<MapType>.dependent empty<bones_t>(position)) {
			return std::nullopt;
		}

		if (cptr<bones_t> maybe_bones{ object_registry<MapType>.dependent at<bones_t>(position) }; maybe_bones == nullptr || !maybe_bones->is<decay_e::Fresh>()) {
			return std::nullopt;
		}

		cptr<cerebra_t> maybe_cerebra{ object_registry<MapType>.dependent at<cerebra_t>(position) };

		if (maybe_cerebra == nullptr || !maybe_cerebra->healthy) {
			return std::nullopt;
		}

		cref<cerebra_t> cerebra{ *maybe_cerebra };

		switch (cerebra.entity) {
			case entity_e::Mercenary:
			case entity_e::Paladin: {
				if (object_registry<MapType>.dependent empty<metal_t>(position)) {
					return std::nullopt;
				}

				break;
			} default: {
				return cerebra.entity;
			}
		}

		cptr<metal_t> maybe_metal{ object_registry<MapType>.dependent at<metal_t>(position) };

		if (maybe_metal == nullptr) {
			return std::nullopt;
		}

		cref<metal_t> metal{ *maybe_metal };

		switch (cerebra.entity) {
			case entity_e::Mercenary: {
				if (metal.state != galvanisation_e::Twisted) {
					return std::nullopt;
				}

				break;
			} case entity_e::Paladin: {
				if (metal.state != galvanisation_e::Shimmering) {
					return std::nullopt;
				}

				break;
			} default: {
				return std::nullopt;
			}
		}

		return cerebra.entity;
	}

	template<map_type_e MapType> inline command_pack_t medicus_t::think(offset_t position) const noexcept {
		if (!has_satchels()) {
			if (object_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, ladder_t>(position)) {
				for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
					cauto current_position{ position + offset };
					
					if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || object_registry<MapType>.dependent empty<ladder_t>(current_position)) {
						continue;
					}

					cptr<ladder_t> maybe_ladder{ object_registry<MapType>.dependent at<ladder_t>(current_position) };

					if (maybe_ladder == nullptr || maybe_ladder->is_down_ladder() || maybe_ladder->has_shackle()) {
						continue;
					}

					return command_pack_t{ command_e::Depart, position, current_position };
				}
			}

			if (cauto departure_pos{ departure_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; departure_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, departure_pos.value() };
			}
		} else {
			if (medicus_goal_map<MapType>.dependent average<region_e::Interior, distance_function_e::Octile>(position) < 1.5f) {
				for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
					cauto current_position{ position + offset };

					if (!game_map<MapType>.dependent within<region_e::Interior>(current_position) || !can_resuscitate<MapType>(current_position).has_value()) {
						continue;
					}

					return command_pack_t{ command_e::Resuscitate, position, current_position };
				}
			} 

			if (cauto corpse_pos{ medicus_goal_map<MapType>.dependent descend<region_e::Interior>(position, entity_registry<MapType>) }; corpse_pos.has_value()) {
				return command_pack_t{ command_e::Move, position, corpse_pos.value() };
			}
		}

		cauto flee_pos{ non_good_goal_map<MapType>.dependent ascend<region_e::Interior>(position, entity_registry<MapType>) };

		if (!flee_pos.has_value()) {
			return command_pack_t{ command_e::Wander, position };
		}

		return command_pack_t{ command_e::Move, position, flee_pos.value() };
	}

	template<map_type_e MapType> inline bool medicus_t::resuscitate(offset_t position) noexcept {
		cauto maybe_entity{ can_resuscitate<MapType>(position) };

		if (!maybe_entity.has_value()) {
			return false;
		}

		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr entity_e cval{ val };

			using entity_type = typename to_entity_type<cval>::type;

			if constexpr (is_good<entity_type>::value) {
				if constexpr (cval == entity_e::Mercenary || cval == entity_e::Paladin) {
					object_registry<MapType>.dependent remove<metal_t>(position);
				}

				object_registry<MapType>.dependent remove<bones_t>(position);
				object_registry<MapType>.dependent remove<flesh_t>(position);
				object_registry<MapType>.dependent remove<cerebra_t>(position);

				if constexpr (is_stockable<entity_type>::value) {
					entity_registry<MapType>.add(position, entity_type{ 0 });
				} else {
					entity_registry<MapType>.add(position, entity_type{});
				}

				set_satchels(satchels - 1);

				return true;
			} else {
				return false;
			}
		}, maybe_entity.value());
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> medicus_t::die(offset_t position) noexcept {
		++steam_stats::stats<stat_e::MediciiSlain>;

		if constexpr (Death == death_e::Killed) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, flesh_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Medicus });
		} else if constexpr (Death == death_e::Devoured) {
			object_registry<MapType>.spill(position, bones_t{});
			object_registry<MapType>.spill(position, cerebra_t{ entity_e::Medicus });
		}

		player.receive_death_boon<medicus_t>();

		scorekeeper.add(entity_e::Medicus);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
