#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<devour_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<devour_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::Devour> {
		using type = devour_t;
	};

	template<> struct to_command_enum<devour_t> {
		static constexpr command_e value = command_e::Devour;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, devour_t> {
		using entity_type = EntityType;
		using command_type = devour_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
