#pragma once

#include <necrowarp/commands/binary/devour.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, devour_t>::process() const noexcept {
		ptr<EntityType> maybe_devourer{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

		if (maybe_devourer == nullptr) {
			return;
		}

		ref<EntityType> devourer{ *maybe_devourer };

		const object_e target_object{ determine_target<EntityType>(object_registry<MapType>.at(target_position)) };
		const entity_e target_entity{ determine_target<EntityType>(entity_registry<MapType>.at(target_position)) };

		if (!devourer.can_devour(target_object) && !devourer.can_devour(target_entity)) {
			return;
		}

		if (target_object == object_e::Flesh) {
			object_registry<MapType>.dependent remove<flesh_t>(target_position);

			devourer.fatten();
		} else if (target_entity != entity_e::None) {
			magic_enum::enum_switch([&](auto val) {
				constexpr entity_e cval{ val };

				using entity_type = to_entity_type<cval>::type;

				if constexpr (is_devourable<entity_type>::value) {
					ptr<entity_type> maybe_target{ entity_registry<MapType>.dependent at<entity_type>(target_position) };

					if (maybe_target == nullptr) {
						return;
					}

					ref<entity_type> target{ *maybe_target };

					if constexpr (is_player<entity_type>::value) {
						if (player.no_hit_enabled() || player.has_ascended()) {
							return;
						}

						const death_info_t<death_e::Devoured> info{ target.dependent die<MapType, death_e::Devoured>() };

						if (!info.devoured) {
							return;
						}

						devourer.fatten(info.protein);
					} else {
						const death_info_t<death_e::Devoured> info{ target.dependent die<MapType, death_e::Devoured>(target_position) };

						if (!info.devoured) {
							return;
						}

						entity_registry<MapType>.dependent remove<entity_type>(target_position);

						devourer.fatten(info.protein);
					}
				}
			}, target_entity);
		}
	}
} // namespace necrowarp
