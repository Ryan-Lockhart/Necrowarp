#pragma once

#include <necrowarp/commands/binary/loose.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <optional>

namespace necrowarp {
	template<map_type_e MapType> inline bool ranger_t::loose(offset_t source_position, offset_t target_position) noexcept {
		if (!can_loose()) {
			return false;
		}

		nocked = false;

		const bool haunted{ entity_goal_map<MapType, furtive_horror_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(source_position) <= furtive_horror_t::EffectRadius };

		const bool missed{ ranger_t::fumble(random_engine) || (haunted && furtive_horror_t::fumble(random_engine)) };
		const bool snapped{ missed && arrow_t::snap(random_engine) };

		if (!snapped) {
			if (object_registry<MapType>.dependent contains<arrow_t>(target_position)) {
				ptr<arrow_t> maybe_arrow{ object_registry<MapType>.dependent at<arrow_t>(target_position) };

				if (maybe_arrow != nullptr && !maybe_arrow->is_full()) {
					++(*maybe_arrow);
				}

				return missed;
			}

			object_registry<MapType>.spill(target_position, arrow_t{});
		}

		return missed;
	}

	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, loose_t>::process() const noexcept {
		ptr<ranger_t> maybe_ranger{ entity_registry<MapType>.dependent at<ranger_t>(source_position) };

		if (maybe_ranger == nullptr || !std::ceil(non_good_goal_map<MapType>.average(target_position)) || !maybe_ranger->can_loose()) {
			return;
		}

		ref<ranger_t> ranger{ *maybe_ranger };

		const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(target_position) };

		if (!maybe_entity.has_value() || !is_valid_target<ranger_t>(maybe_entity.value())) {
			return;
		}

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			using entity_type = to_entity_type<cval>::type;

			ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(target_position) };

			if (maybe_target == nullptr) {
				return;
			}

			ref<entity_type> target{ *maybe_target };
			
			cauto try_bleed = [&] {
				if constexpr (is_bleeder_v<entity_type>) {
					if constexpr (is_bleeder<entity_type>::conditional) {
						if (!target.can_bleed()) {
							return;
						}
					}

					constexpr fluid_e fluid{ is_bleeder<entity_type>::type };
					
					spill_fluid<MapType>(target_position, fluid);
				}
			};

			if (!ranger.loose<MapType>(source_position, target_position)) {
				return;
			}

			const i8 damage{ ranger.get_damage(cval) };

			if constexpr (!is_fodder<entity_type>::value) {
				if constexpr (is_cleaver<EntityType>::value && is_armored<entity_type>::value) {
					if constexpr (is_volumetric<entity_type>::value) {
						const f16 fluid_damage{ fluid_pool_volume(damage) };

						if (target.dependent can_survive<EntityType>(fluid_damage)) {
							if (target.dependent receive_damage<EntityType>(fluid_damage)) {	
								try_bleed();
							}

							return;
						}
					} else {
						if (target.dependent can_survive<EntityType>(damage)) {
							if (target.dependent receive_damage<EntityType>(damage)) {	
								try_bleed();
							}

							return;
						}
					}
				} else {
					if constexpr (is_volumetric<entity_type>::value) {
						const f16 fluid_damage{ fluid_pool_volume(damage) };

						if (target.can_survive(fluid_damage)) {
							if (target.receive_damage(fluid_damage)) {	
								try_bleed();
							}

							return;
						}
					} else {
						if (target.can_survive(damage)) {
							if (target.receive_damage(damage)) {	
								try_bleed();
							}

							return;
						}
					}
				}
			}

			try_bleed();

			if constexpr (is_player<entity_type>::value) {
				target.dependent die<MapType, death_e::Killed>();
			} else {
				target.dependent die<MapType, death_e::Killed>(target_position);
			}

			if constexpr (is_npc_entity<entity_type>::value) {
				entity_registry<MapType>.dependent remove<entity_type>(target_position);
			}
		}, maybe_entity.value());
	}
} // namespace necrowarp
