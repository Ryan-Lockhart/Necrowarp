#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

namespace necrowarp {
	constexpr bool operator==(object_group_e lhs, object_e rhs) noexcept {
		const object_group_e flag{
			magic_enum::enum_switch([](auto val) -> object_group_e {
				constexpr object_e cval{ val };

				return to_object_group<cval>::value;
			}, rhs)
		};

		return static_cast<object_group_e>(static_cast<object_group_t>(lhs) & static_cast<object_group_t>(flag)) == flag;
	}

	template<typename... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value && is_homogeneous<object_e, ObjectTypes...>::value
	constexpr bool operator==(object_group_e lhs, ObjectTypes... rhs) noexcept {
		return ((lhs == rhs) || ...);
	}

	constexpr bool operator!=(object_group_e lhs, object_e rhs) noexcept {
		const object_group_e flag{
			magic_enum::enum_switch([](auto val) -> object_group_e {
				constexpr object_e cval{ val };

				return to_object_group<cval>::value;
			}, rhs)
		};

		return static_cast<object_group_e>(static_cast<object_group_t>(lhs) & static_cast<object_group_t>(flag)) == object_group_e::None;
	}

	template<typename... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value && is_homogeneous<object_e, ObjectTypes...>::value
	constexpr bool operator!=(object_group_e lhs, ObjectTypes... rhs) noexcept {
		return ((lhs != rhs) && ...);
	}

	static constexpr std::string to_string(object_group_e group) noexcept {
		std::string string{ "" };

		magic_enum::enum_for_each<object_e>([&](auto val) -> void {
			constexpr object_e cval{ val };

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

	static constexpr runes_t to_colored_string(object_group_e group) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<object_e>([&](auto val) -> void {
			constexpr object_e cval{ val };

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

	constexpr object_e at(object_group_e group, usize index) noexcept {
		if (index < 0 || index > ObjectTypeCount) {
			return object_e::None;
		}

		usize i{ 0 };

		for (usize j{ 0 }; j < ObjectTypeCount; ++j) {
			const object_e as_object{ static_cast<object_e>(j + 1) };

			if (group == as_object) {
				if (i == index) {
					return as_object;
				}

				++i;
			}
		}

		return object_e::None;
	}	

	template<map_type_e MapType> static constexpr runes_t to_colored_string(object_group_e group, offset_t position) noexcept {
		runes_t runes{};

		magic_enum::enum_for_each<object_e>([&](auto val) -> void {
			constexpr object_e cval{ val };

			if (group != cval) {
				return;
			}

			if (!runes.empty()) {
				runes.concatenate(runes_t{ ", " });
			}

			using object_type = to_object_type<cval>::type;

			if constexpr (has_unique_descriptor<object_type>::value) {
				cauto object_ptr{ object_registry<MapType>.dependent at<object_type>(position) };

				if (object_ptr != nullptr) {
					runes.concatenate(object_ptr->to_colored_string());

					return;
				}
			}

			runes.concatenate(to_colored_string(cval));
		});

		return runes;
	}
} // namespace necrowarp
