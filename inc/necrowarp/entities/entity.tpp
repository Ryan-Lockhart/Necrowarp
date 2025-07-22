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

		magic_enum::enum_for_each<entity_e>([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (string != "") {
				string.append(", ");
			}

			string.append(to_string(cval));
		});

		return string;
	}

	static constexpr runes_t to_colored_string(entity_group_e group) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<entity_e>([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (!runes.empty()) {
				runes.concatenate(runes_t{ ", " });
			}

			runes.concatenate(to_colored_string(cval));
		});

		return runes;
	}

	constexpr entity_e at(entity_group_e group, usize index) noexcept {
		if (index < 0 || index > EntityTypeCount) {
			return entity_e::None;
		}

		usize i{ 0 };

		for (usize j{ 0 }; j < EntityTypeCount; ++j) {
			const entity_e as_entity{ static_cast<entity_e>(j + 1) };

			if (group == as_entity) {
				if (i == index) {
					return as_entity;
				}

				++i;
			}
		}

		return entity_e::None;
	}

	template<map_type_e MapType> static constexpr runes_t to_colored_string(entity_group_e group, offset_t position) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<entity_e>([&](auto val) -> void {
			constexpr entity_e cval{ val };

			if (group != cval) {
				return;
			}

			if (!runes.empty()) {
				runes.concatenate(runes_t{ ", " });
			}

			using entity_type = to_entity_type<cval>::type;

			if constexpr (globals::has_unique_descriptor<entity_type>::value) {
				cauto entity_ptr{ entity_registry<MapType>.dependent at<entity_type>(position) };

				if (entity_ptr != nullptr) {
					runes.concatenate(entity_ptr->to_colored_string());

					return;
				}
			}

			runes.concatenate(to_colored_string(cval));
		});

		return runes;
	}

	namespace globals {
		static constexpr entity_e OopsAllEnum{ entity_e::Berserker };

		template<> inline constexpr bool OopsAll<OopsAllEnum>{ false };
		
		template<NonNullEntity... Entities> static constexpr usize OopsAllCount{ (OopsAll<to_entity_enum<Entities>::value> + ...) };

		static_assert(OopsAllCount<ALL_NON_EVIL_NPCS> <= 1, "cannot force multiple enemy types!");

		static constexpr bool OopsAllEnabled{ OopsAllCount<ALL_NON_EVIL_NPCS> == 1 };
	}
} // namespace necrowarp
