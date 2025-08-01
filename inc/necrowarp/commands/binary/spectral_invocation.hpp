#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<spectral_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct is_binary_command<spectral_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct is_grimoire<spectral_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct to_grimoire_enum<spectral_invocation_t> {
		static constexpr grimoire_e value = grimoire_e::SpectralInvocation;
	};

	template<> struct to_command_type<command_e::SpectralInvocation> {
		using type = spectral_invocation_t;
	};

	template<> struct to_command_enum<spectral_invocation_t> {
		static constexpr command_e value = command_e::SpectralInvocation;
	};

	template<NonNullEntity EntityType> struct entity_command_t<EntityType, spectral_invocation_t> {
		using entity_type = EntityType;
		using command_type = spectral_invocation_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
