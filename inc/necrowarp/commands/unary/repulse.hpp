#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<repulse_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unary_command<repulse_t> {
		static constexpr bool value = true;
	};

	template<> struct is_grimoire<repulse_t> {
		static constexpr bool value = true;
	};

	template<> struct to_grimoire_enum<repulse_t> {
		static constexpr grimoire_e value = grimoire_e::Repulse;
	};

	template<> struct to_command_type<command_e::Repulse> {
		using type = repulse_t;
	};

	template<> struct to_command_enum<repulse_t> {
		static constexpr command_e value = command_e::Repulse;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, repulse_t> {
		using entity_type = EntityType;
		using command_type = repulse_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
