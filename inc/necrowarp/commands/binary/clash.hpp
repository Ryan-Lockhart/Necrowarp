#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<clash_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<clash_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_enum<clash_t> {
		static constexpr command_e value = command_e::Clash;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, clash_t> {
		using entity_type = EntityType;
		using command_type = clash_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() noexcept;
	};
} // namespace necrowarp
