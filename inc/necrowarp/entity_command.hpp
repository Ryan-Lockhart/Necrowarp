#pragma once

#include <bleak.hpp>

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

namespace necrowarp {
	using namespace bleak;

	template<AnimateEntity EntityType, NonNullCommand Command> struct is_entity_command_valid {
		static constexpr bool value = false;
	};

	template<AnimateEntity EntityType> struct is_entity_command_valid<EntityType, move_t> {
		static constexpr bool value = true;
	};

	template<CombatantEntity EntityType> struct is_entity_command_valid<EntityType, clash_t> {
		static constexpr bool value = true;
	};

	template<AnimateEntity EntityType> static inline entity_e determine_target(entity_group_e group) noexcept {
		crauto target_priorities{ EntityType::TargetPriorities };

		for (const entity_e entity_type : target_priorities) {
			if (group != entity_type) {
				continue;
			}

			return entity_type;
		}

		return entity_e::None;
	}

	template<AnimateEntity EntityType, Command CommandType> struct entity_command_t;

	template<AnimateEntity EntityType, UnaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;

		inline void process() const noexcept;
	};

	template<AnimateEntity EntityType, BinaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};

	template<AnimateEntity EntityType, TernaryCommand CommandType> struct entity_command_t<EntityType, CommandType> {
		using entity_type = EntityType;
		using command_type = CommandType;

		static constexpr entity_e entity_enum{ to_entity_enum<entity_type>::value };
		static constexpr command_e command_enum{ to_command_enum<command_type>::value };

		const offset_t source_position;
		const offset_t intermediate_position;
		const offset_t target_position;

		inline void process() const noexcept;
	};
} // namespace necrowarp
