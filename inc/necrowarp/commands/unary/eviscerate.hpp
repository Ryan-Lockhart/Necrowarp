#pragma once

#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_command<eviscerate_t> {
		static constexpr bool value = true;
	};

	template<> struct is_unary_command<eviscerate_t> {
		static constexpr bool value = true;
	};

	template<> struct to_command_type<command_e::Eviscerate> {
		using type = eviscerate_t;
	};

	template<> struct to_command_enum<eviscerate_t> {
		static constexpr command_e value = command_e::Eviscerate;
	};

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!std::is_same<InitiatorType, VictimType>::value)
	static inline bool brutalize(offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim, ref<i8> damage) noexcept;

	template<CombatantEntity EntityType> struct entity_command_t<EntityType, eviscerate_t> {
		using entity_type = EntityType;
		using command_type = eviscerate_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
