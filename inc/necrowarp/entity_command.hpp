#pragma once

#include <bleak.hpp>

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<NonNullEntity EntityType, Command CommandType> struct entity_command_t;

	template<NonNullEntity EntityType, UnaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		inline void process() noexcept;
	};

	template<NonNullEntity EntityType, BinaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() noexcept;
	};

	template<NonNullEntity EntityType, TernaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t intermediate_position;
		const offset_t target_position;

		inline void process() noexcept;
	};
} // namespace necrowarp
