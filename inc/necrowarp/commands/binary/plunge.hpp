#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	enum struct dimension_e : u8;
	enum struct stability_e : u8;

	using namespace bleak;

	template<> struct is_command<plunge_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<plunge_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::Plunge> {
		using type = plunge_t;
	};

	template<> struct to_command_enum<plunge_t> {
		static constexpr command_e value = command_e::Plunge;
	};

	static inline dimension_e determine_dimension(stability_e stability) noexcept;

	template<Entity EntityType> struct entity_command_t<EntityType, plunge_t> {
		using entity_type = EntityType;
		using command_type = plunge_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
