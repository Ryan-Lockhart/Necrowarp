#pragma once

#include <bleak.hpp>

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/objects/object.hpp>

namespace necrowarp {
	using namespace bleak;

	template<Entity EntityType, NonNullCommand Command> struct is_entity_command_valid {
		static constexpr bool value = false;
	};

	template<Entity EntityType> struct is_entity_command_valid<EntityType, wander_t> {
		static constexpr bool value = true;
	};

	template<Entity EntityType> struct is_entity_command_valid<EntityType, move_t> {
		static constexpr bool value = true;
	};

	template<CombatantEntity EntityType> struct is_entity_command_valid<EntityType, clash_t> {
		static constexpr bool value = true;
	};

	template<GoodEntity EntityType> struct is_entity_command_valid<EntityType, depart_t> {
		static constexpr bool value = true;
	};

	template<Entity EntityType, entity_e Entity> static constexpr bool is_interactable() noexcept {
		return std::any_of(EntityType::EntityPriorities.begin(), EntityType::EntityPriorities.end(), [&](auto val) -> bool { return Entity == val; });
	}

	template<Entity EntityType, object_e Object> static constexpr bool is_interactable() noexcept {
		return std::any_of(EntityType::ObjectPriorities.begin(), EntityType::ObjectPriorities.end(), [&](auto val) -> bool { return Object == val; });
	}

	template<Entity EntityType> static inline bool is_interactable(entity_e entity) noexcept {
		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr entity_e cval{ val };

			return is_interactable<EntityType, cval>();
		}, entity);
	}

	template<Entity EntityType> static inline bool is_interactable(object_e object) noexcept {
		return magic_enum::enum_switch([&](auto val) -> bool {
			constexpr object_e cval{ val };

			return is_interactable<EntityType, cval>();
		}, object);
	}

	template<Entity EntityType> static inline object_e determine_target(object_group_e group) noexcept {
		crauto priorities{ EntityType::ObjectPriorities };

		for (const object_e type : priorities) {
			if (group != type) {
				continue;
			}

			return type;
		}

		return object_e::None;
	}

	template<Entity EntityType, Command CommandType> struct entity_command_t;

	template<Entity EntityType, UnaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		inline void process() const noexcept;
	};

	template<Entity EntityType, BinaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};

	template<Entity EntityType, TernaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t intermediate_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};
} // namespace necrowarp
