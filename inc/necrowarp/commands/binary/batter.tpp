#pragma once

#include <necrowarp/commands/binary/batter.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, batter_t>::process() const noexcept {
		ptr<battle_monk_t> maybe_monk{ entity_registry<MapType>.template at<battle_monk_t>(source_position) };

		if (maybe_monk == nullptr) {
			return;
		}

		const entity_e target{ determine_target<battle_monk_t>(entity_registry<MapType>.at(target_position)) };
		
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None) {
				using entity_type = to_entity_type<cval>::type;

				ptr<entity_type> maybe_target{ entity_registry<MapType>.template at<entity_type>(target_position) };

				if (maybe_target == nullptr) {
					return;
				}

				bool critical_strike{ false };

				if constexpr (is_concussable<entity_type>::value) {
					if (!stunned.contains(target_position)) {
						stunned.add(target_position);
					} else {
						critical_strike = true;
					}
				}

				const i8 damage{ static_cast<i8>(maybe_monk->get_damage(cval) * (critical_strike ? 2 : 1)) };

				if constexpr (!is_fodder<entity_type>::value) {
					if (maybe_target->can_survive(damage)) {
						maybe_target->receive_damage(damage);

						if constexpr (is_bleeder<entity_type>::value) {
							constexpr fluid_e fluid{ fluid_type<entity_type>::type };

							fluid_map<MapType>[maybe_target->position] += fluid;
						}

						return;
					}
				}

				maybe_target->template die<MapType>();

				if constexpr (is_npc_entity<entity_type>::value) {
					entity_registry<MapType>.template remove<entity_type>(target_position);
				}
			}
		}, target);
	}
} // namespace necrowarp
