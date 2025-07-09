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

	template<> struct to_command_type<command_e::Clash> {
		using type = clash_t;
	};

	template<> struct to_command_enum<clash_t> {
		static constexpr command_e value = command_e::Clash;
	};

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<InitiatorType>::value && !std::is_same<InitiatorType, VictimType>::value)
	static inline bool instigate(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept;

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<VictimType>::value && !std::is_same<InitiatorType, VictimType>::value)
	static inline bool retaliate(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept;

	template<map_type_e MapType, CombatantEntity InitiatorType, CombatantEntity VictimType>
		requires (!is_docile<InitiatorType>::value && !std::is_same<InitiatorType, VictimType>::value)
	static inline bool reflect(offset_t source_position, offset_t target_position, ref<InitiatorType> initiator, ref<VictimType> victim) noexcept;

	template<CombatantEntity EntityType> struct entity_command_t<EntityType, clash_t> {
		using entity_type = EntityType;
		using command_type = clash_t;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		template<map_type_e MapType> inline void process() const noexcept;
	};
} // namespace necrowarp
