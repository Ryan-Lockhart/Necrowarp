#pragma once

#include <necrowarp/entities/entity.hpp>

namespace necrowarp {
	constexpr bool operator==(entity_group_e lhs, entity_e rhs) noexcept {
		const entity_group_e flag{
			magic_enum::enum_switch([](auto val) -> entity_group_e {
				constexpr entity_e cval{ val };

				return to_entity_group<cval>::value;
			}, rhs)
		};

		return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) & static_cast<entity_group_t>(flag)) == flag;
	}

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator==(entity_group_e lhs, EntityTypes... rhs) noexcept {
		return ((lhs == rhs) || ...);
	}

	constexpr bool operator!=(entity_group_e lhs, entity_e rhs) noexcept {
		const entity_group_e flag{
			magic_enum::enum_switch([](auto val) -> entity_group_e {
				constexpr entity_e cval{ val };

				return to_entity_group<cval>::value;
			}, rhs)
		};

		return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) & static_cast<entity_group_t>(flag)) == entity_group_e::None;
	}

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator!=(entity_group_e lhs, EntityTypes... rhs) noexcept {
		return ((lhs != rhs) && ...);
	}
} // namespace necrowarp
