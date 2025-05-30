#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	constexpr bool operator==(entity_group_e lhs, entity_e rhs) noexcept {
		const entity_group_e flag{
			magic_enum::enum_switch([](auto val) -> entity_group_e {
				constexpr entity_e cval{ val };

				return to_entity_group<cval>::value;
			}, rhs)
		};

		return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) & static_cast<entity_group_t>(flag)) == flag;
	}

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator==(entity_group_e lhs, EntityTypes... rhs) noexcept {
		return ((lhs == rhs) || ...);
	}

	constexpr bool operator!=(entity_group_e lhs, entity_e rhs) noexcept {
		const entity_group_e flag{
			magic_enum::enum_switch([](auto val) -> entity_group_e {
				constexpr entity_e cval{ val };

				return to_entity_group<cval>::value;
			}, rhs)
		};

		return static_cast<entity_group_e>(static_cast<entity_group_t>(lhs) & static_cast<entity_group_t>(flag)) == entity_group_e::None;
	}

	template<typename... EntityTypes>
		requires is_plurary<EntityTypes...>::value && is_homogeneous<entity_e, EntityTypes...>::value
	constexpr bool operator!=(entity_group_e lhs, EntityTypes... rhs) noexcept {
		return ((lhs != rhs) && ...);
	}

	static constexpr std::string to_string(entity_group_e group) noexcept {
		std::string string{ "" };

		magic_enum::enum_for_each<entity_e>([&](auto val) {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (string != "" && cval != entity_e::Priest) {
				string.append(", ");
			}

			string.append(to_string(cval));
		});

		return string;
	}

	static constexpr runes_t to_colored_string(entity_group_e group) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<entity_e>([&](auto val) {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (!runes.empty() && cval != entity_e::Priest) {
				runes.concatenate(runes_t{ ", " });
			}

			runes.concatenate(to_colored_string(cval));
		});

		return runes;
	}

	static constexpr runes_t to_colored_string(entity_group_e group, offset_t position) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<entity_e>([&](auto val) {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (!runes.empty() && cval != entity_e::Priest) {
				runes.concatenate(runes_t{ ", " });
			}

			using entity_type = to_entity_type<cval>::type;

			if constexpr (globals::has_unique_descriptor<entity_type>::value) {
				cauto entity_ptr{ entity_registry.at<entity_type>(position) };

				if (entity_ptr != nullptr) {
					runes.concatenate(entity_ptr->to_colored_string());

					return;
				}
			}

			runes.concatenate(to_colored_string(cval));
		});

		return runes;
	}
} // namespace necrowarp
