#pragma once

#include <bleak.hpp>

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>
#include <necrowarp/objects/object.hpp>

namespace necrowarp {
	using namespace bleak;

	template<NonNullEntity EntityType, NonNullCommand Command> struct is_entity_command_valid {
		static constexpr bool value = false;
	};

	template<NonNullEntity EntityType> struct is_entity_command_valid<EntityType, wander_t> {
		static constexpr bool value = true;
	};

	template<NonNullEntity EntityType> struct is_entity_command_valid<EntityType, move_t> {
		static constexpr bool value = true;
	};

	template<CombatantEntity EntityType> struct is_entity_command_valid<EntityType, clash_t> {
		static constexpr bool value = true;
	};

	template<NonNullEntity EntityType> static inline entity_e determine_target(entity_group_e group) noexcept {
		crauto priorities{ EntityType::EntityPriorities };

		for (const entity_e type : priorities) {
			if (group != type) {
				continue;
			}

			return type;
		}

		return entity_e::None;
	}

	template<NonNullEntity EntityType> static inline object_e determine_target(object_group_e group) noexcept {
		crauto priorities{ EntityType::ObjectPriorities };

		for (const object_e type : priorities) {
			if (group != type) {
				continue;
			}

			return type;
		}

		return object_e::None;
	}

	template<NonNullEntity EntityType, Command CommandType> struct entity_command_t;

	template<NonNullEntity EntityType, UnaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		inline void process() const noexcept;
	};

	template<NonNullEntity EntityType, BinaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};

	template<NonNullEntity EntityType, TernaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
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
