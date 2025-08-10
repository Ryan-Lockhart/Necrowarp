#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<necromantic_ascendance_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unary_command<necromantic_ascendance_t> {
		static constexpr bool value = true;
	};

	template<> struct is_grimoire<necromantic_ascendance_t> {
		static constexpr bool value = true;
	};

	template<> struct to_grimoire_enum<necromantic_ascendance_t> {
		static constexpr grimoire_e value = grimoire_e::NecromanticAscendance;
	};

	template<> struct to_command_type<command_e::NecromanticAscendance> {
		using type = necromantic_ascendance_t;
	};

	template<> struct to_command_enum<necromantic_ascendance_t> {
		static constexpr command_e value = command_e::NecromanticAscendance;
	};

	template<Entity EntityType> struct entity_command_t<EntityType, necromantic_ascendance_t> {
		using entity_type = EntityType;
		using command_type = necromantic_ascendance_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
