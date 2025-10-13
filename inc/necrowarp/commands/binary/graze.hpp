#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<graze_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<graze_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::Graze> {
		using type = graze_t;
	};

	template<> struct to_command_enum<graze_t> {
		static constexpr command_e value = command_e::Graze;
	};

	template<Entity EntityType> struct entity_command_t<EntityType, graze_t> {
		using entity_type = EntityType;
		using command_type = graze_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
