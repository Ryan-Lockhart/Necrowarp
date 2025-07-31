#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<chaotic_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unary_command<chaotic_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_grimoire<chaotic_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct to_grimoire_enum<chaotic_warp_t> {
		static constexpr grimoire_e value = grimoire_e::ChaoticWarp;
	};

	template<> struct to_command_type<command_e::ChaoticWarp> {
		using type = chaotic_warp_t;
	};

	template<> struct to_command_enum<chaotic_warp_t> {
		static constexpr command_e value = command_e::ChaoticWarp;
	};

	struct chaotic_warp_t {
		template<map_type_e MapType> static inline bool execute(offset_t position, bool free = false) noexcept;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, chaotic_warp_t> {
		using entity_type = EntityType;
		using command_type = chaotic_warp_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
