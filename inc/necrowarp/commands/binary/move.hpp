#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<move_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<move_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::Move> {
		using type = move_t;
	};

	template<> struct to_command_enum<move_t> {
		static constexpr command_e value = command_e::Move;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, move_t> {
		using entity_type = EntityType;
		using command_type = move_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};
} // namespace necrowarp
