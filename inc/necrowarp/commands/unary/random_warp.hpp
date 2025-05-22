#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<random_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unary_command<random_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::RandomWarp> {
		using type = random_warp_t;
	};

	template<> struct to_command_enum<random_warp_t> {
		static constexpr command_e value = command_e::RandomWarp;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, random_warp_t> {
		using entity_type = EntityType;
		using command_type = random_warp_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		inline void process() const noexcept;
	};
} // namespace necrowarp
