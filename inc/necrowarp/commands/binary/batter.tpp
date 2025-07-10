#pragma once

#include <necrowarp/commands/binary/batter.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, batter_t>::process() const noexcept {
		ptr<battle_monk_t> maybe_monk{ entity_registry<MapType>.dependent at<battle_monk_t>(source_position) };

		if (maybe_monk == nullptr) {
			return;
		}

		ref<battle_monk_t> initiator{ *maybe_monk };

		const entity_e target{ determine_target<battle_monk_t>(entity_registry<MapType>.at(target_position)) };
		
		magic_enum::enum_switch([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if constexpr (cval != entity_e::None) {
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

				bool critical_strike{ false };

				if constexpr (is_concussable<entity_type>::value) {
					if (!entity_registry<MapType>.is_concussed(target_position)) {
						concussed.add(target_position);
					} else {
						critical_strike = true;
					}
				}

				const i8 damage{ static_cast<i8>(initiator.get_damage(cval) * (critical_strike ? 2 : 1)) };

				if constexpr (!is_fodder<entity_type>::value) {
					if (target.can_survive(damage)) {
						target.receive_damage(damage);
						
						try_bleed();

						return;
					}
				}
						
				try_bleed();

				if constexpr (is_player<entity_type>::value) {
					target.dependent die<MapType>();
				} else {
					target.dependent die<MapType>(target_position);
				}

				if constexpr (is_npc_entity<entity_type>::value) {
					entity_registry<MapType>.dependent remove<entity_type>(target_position);
				}
			}
		}, target);
	}
} // namespace necrowarp
