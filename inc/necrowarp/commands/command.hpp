#pragma once

#include <bleak.hpp>

#include <optional>

#include <necrowarp/entities/entity.hpp>

namespace necrowarp {
	using namespace bleak;

	struct none_t;

	struct random_warp_t;
	struct necromantic_ascendance_t;
	struct eviscerate_t;

	struct move_t;
	struct descend_t;
	struct plunge_t;
	struct consume_t;
	struct clash_t;
	struct target_warp_t;
	struct consume_warp_t;
	struct calcitic_invocation_t;
	struct spectral_invocation_t;
	struct sanguine_invocation_t;
	struct galvanic_invocation_t;

	struct lunge_t;

	#define ALL_NULL_COMMANDS \
		none_t

	#define ALL_UNARY_COMMANDS \
		random_warp_t, \
		necromantic_ascendance_t

	#define ALL_BINARY_COMMANDS \
		move_t, \
		descend_t, \
		plunge_t, \
		consume_t, \
		clash_t, \
		target_warp_t, \
		consume_warp_t, \
		calcitic_invocation_t, \
		spectral_invocation_t, \
		sanguine_invocation_t, \
		galvanic_invocation_t

	#define ALL_TERNARY_COMMANDS \
		lunge_t

	#define ALL_NON_NULL_COMMANDS \
		ALL_UNARY_COMMANDS, \
		ALL_BINARY_COMMANDS, \
		ALL_TERNARY_COMMANDS

	#define ALL_COMMANDS \
		ALL_NULL_COMMANDS, \
		ALL_NON_NULL_COMMANDS
	
	#define ALL_BASIC_COMMANDS \
		move_t, \
		clash_t
	
	#define ALL_NECRO_COMMANDS \
		descend_t, \
		plunge_t, \
		consume_t, \
		random_warp_t, \
		target_warp_t, \
		consume_warp_t, \
		calcitic_invocation_t, \
		spectral_invocation_t, \
		sanguine_invocation_t, \
		galvanic_invocation_t, \
		necromantic_ascendance_t
	
	#define ALL_BLOODHOUND_COMMANDS \
		lunge_t
	
	#define ALL_WRAITH_COMMANDS \
		eviscerate_t
	
	#define ALL_NPC_COMMANDS \
		ALL_BASIC_COMMANDS, \
		ALL_BLOODHOUND_COMMANDS, \
		ALL_WRAITH_COMMANDS
	
	#define ALL_PLAYER_COMMANDS \
		ALL_BASIC_COMMANDS, \
		ALL_NECRO_COMMANDS

	enum struct command_e : u8 {
		None = 0,
		Move,
		Descend,
		Plunge,
		Consume,
		Clash,
		Lunge,
		Eviscerate,
		RandomWarp,
		TargetWarp,
		ConsumeWarp,
		CalciticInvocation,
		SpectralInvocation,
		SanguineInvocation,
		GalvanicInvocation,
		NecromanticAscendance,
	};

	struct command_pack_t {
		command_e type;

		std::optional<offset_t> source_position;
		std::optional<offset_t> intermediate_position;
		std::optional<offset_t> target_position;

		constexpr command_pack_t() : type{ command_e::None }, source_position{}, intermediate_position{}, target_position{} {}

		constexpr command_pack_t(command_e command) : type{ command }, source_position{}, intermediate_position{}, target_position{} {}

		constexpr command_pack_t(command_e command, offset_t source) : type{ command }, source_position{ source }, intermediate_position{}, target_position{} {}

		constexpr command_pack_t(command_e command, offset_t source, offset_t target) : type{ command },  source_position{ source }, intermediate_position{}, target_position{ target } {}

		constexpr command_pack_t(command_e command, offset_t source, offset_t intermediate, offset_t target) : type{ command },  source_position{ source }, intermediate_position{ intermediate }, target_position{ target } {}
	};

	static constexpr cstr to_string(command_e command) noexcept {
		switch (command) {
			case command_e::None: {
				return "none";
			} case command_e::Move: {
				return "move";
			} case command_e::Descend: {
				return "descend";
			} case command_e::Plunge: {
				return "plunge";
			} case command_e::Consume: {
				return "consume";
			} case command_e::Clash: {
				return "clash";
			} case command_e::Lunge: {
				return "lunge";
			} case command_e::Eviscerate: {
				return "eviscerate";
			} case command_e::RandomWarp: {
				return "random warp";
			} case command_e::TargetWarp: {
				return "target warp";
			} case command_e::ConsumeWarp: {
				return "consume warp";
			} case command_e::CalciticInvocation: {
				return "calcitic invocation";
			} case command_e::SpectralInvocation: {
				return "spectral invocation";
			} case command_e::SanguineInvocation: {
				return "sanguine invocation";
			} case command_e::GalvanicInvocation: {
				return "galvanic invocation";
			} case command_e::NecromanticAscendance: {
				return "necromantic ascendance";
			} default: {
				return "unknown";
			}
		}
	}

	template<typename T> struct is_command {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_command_v = is_command<T>::value;

	template<typename T> concept Command = is_command<T>::value;

	template<Command T> struct to_command_enum {
		static constexpr command_e value = command_e::None;
	};

	template<Command T> constexpr command_e to_command_enum_v = to_command_enum<T>::value;
	
	template<command_e CommandType> struct to_command_type;

	template<command_e CommandType> using to_command_type_t = typename to_command_type<CommandType>::type;
	
	template<> struct to_command_type<command_e::None> {
		using type = std::nullptr_t;
	};
	
	template<typename T> struct is_null_command {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_null_command_v = is_null_command<T>::value;

	template<> struct is_null_command<std::nullptr_t> {
		static constexpr bool value = true;
	};

	template<typename T> concept NullCommand = Command<T> && is_null_command<T>::value;

	template<typename T> concept NonNullCommand = Command<T> && !is_null_command<T>::value;

	template<typename T> struct is_unary_command {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_unary_command_v = is_unary_command<T>::value;

	template<typename T> concept UnaryCommand = Command<T> && is_unary_command<T>::value;

	template<typename T> struct is_binary_command {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_binary_command_v = is_binary_command<T>::value;

	template<typename T> concept BinaryCommand = Command<T> && is_binary_command<T>::value;

	template<typename T> struct is_ternary_command {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_ternary_command_v = is_ternary_command<T>::value;

	template<typename T> concept TernaryCommand = Command<T> && is_ternary_command<T>::value;

	template<Command T, command_e CommandType> struct is_command_type {
		static constexpr bool value = to_command_enum<T>::value == CommandType;
	};

	template<command_e> inline constexpr glyph_t command_icons;
} // namespace necrowarp
