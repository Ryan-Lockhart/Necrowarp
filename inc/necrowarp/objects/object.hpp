#pragma once

#include <bleak.hpp>

#include <cstddef>

#include <necrowarp/cell.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct skull_t;
	struct flesh_t;
	struct metal_t;

	struct ladder_t;
	struct portal_t;

	struct arrow_t;

	#define ALL_CATALYST_OBJECTS \
		skull_t, \
		flesh_t, \
		metal_t

	#define ALL_LOCOMOTION_OBJECTS \
		ladder_t, \
		portal_t

	#define ALL_DEBRIS_OBJECTS \
		arrow_t

	#define ALL_ANIMATED_OBJECTS \
		metal_t, \
		ladder_t, \
		portal_t

	#define ALL_OBJECTS \
		ALL_CATALYST_OBJECTS, \
		ALL_LOCOMOTION_OBJECTS, \
		ALL_DEBRIS_OBJECTS

	enum struct object_e : u8 {
		None = 0,
		Skull,
		Flesh,
		Metal,
		Ladder,
		Portal,
		Arrow,
	};

	static constexpr cstr to_string(object_e type) noexcept {
		switch (type) {
			case object_e::None: {
				return "none";
			} case object_e::Skull: {
				return "skull";
			} case object_e::Flesh: {
				return "flesh";
			} case object_e::Metal: {
				return "metal";
			} case object_e::Ladder: {
				return "ladder";
			} case object_e::Portal: {
				return "portal";
			} case object_e::Arrow: {
				return "arrow";
			}
		}
	}

	static constexpr runes_t to_colored_string(object_e type) noexcept {
		const cstr string{ to_string(type) };

		switch (type) {
			case object_e::None: {
				return runes_t{ string, colors::Grey };
			} case object_e::Skull: {
				return runes_t{ string, colors::White };
			} case object_e::Flesh: {
				return runes_t{ string, colors::Orange };
			} case object_e::Metal: {
				return runes_t{ string, colors::metals::Iron };
			} case object_e::Ladder: {
				return runes_t{ string, colors::materials::Oak };
			} case object_e::Portal: {
				return runes_t{ string, colors::light::Green };
			} case object_e::Arrow: {
				return runes_t{ string, colors::materials::Oak };
			}
		}
	}

	constexpr usize ObjectTypeCount{ static_cast<usize>(object_e::Arrow) + 1 };

	using object_group_t = u16;

	enum struct object_group_e : object_group_t {
		None = 0,
		Skull = 1 << 0,
		Flesh = Skull << 1,
		Metal = Flesh << 1,
		Ladder = Metal << 1,
		Portal = Ladder << 1,
		Arrow = Portal << 1,
	};

	constexpr object_group_e operator+(object_group_e lhs, object_group_e rhs) noexcept { return static_cast<object_group_e>(static_cast<object_group_t>(lhs) | static_cast<object_group_t>(rhs)); }

	constexpr ref<object_group_e> operator+=(ref<object_group_e> lhs, object_group_e rhs) noexcept { return lhs = lhs + rhs, lhs; }

	constexpr object_group_e operator-(object_group_e lhs, object_group_e rhs) noexcept { return static_cast<object_group_e>(static_cast<object_group_t>(lhs) & (~static_cast<object_group_t>(rhs))); }

	constexpr ref<object_group_e> operator-=(ref<object_group_e> lhs, object_group_e rhs) noexcept { return lhs = lhs - rhs, lhs; }

	template<object_e ObjectType> struct to_object_group {
		static constexpr object_group_e value{ object_group_e::None };
	};

	constexpr bool operator==(object_group_e lhs, object_e rhs) noexcept;

	template<typename... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value && is_homogeneous<object_e, ObjectTypes...>::value
	constexpr bool operator==(object_group_e lhs, ObjectTypes... rhs) noexcept;

	constexpr bool operator!=(object_group_e lhs, object_e rhs) noexcept;

	template<typename... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value && is_homogeneous<object_e, ObjectTypes...>::value
	constexpr bool operator!=(object_group_e lhs, ObjectTypes... rhs) noexcept;

	static constexpr std::string to_string(object_group_e group) noexcept;

	static constexpr runes_t to_colored_string(object_group_e group) noexcept;

	enum struct decay_e : i8 {
		Rotted = -1,
		Animate = 0,
		Fresh = 1,
	};

	constexpr decay_e decay(decay_e state) noexcept {
		switch (state) {
			case decay_e::Fresh: {
				return decay_e::Animate;
			} case decay_e::Animate: {
				return decay_e::Rotted;
			} case decay_e::Rotted: {
				return decay_e::Rotted;
			}
		}
	}

	constexpr cstr to_string(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return "rotted";
			} case decay_e::Animate: {
				return "animate";
			} case decay_e::Fresh: {
				return "fresh";
			}
		}
	}

	constexpr runes_t to_colored_string(decay_e decay) noexcept {
		switch (decay) {
			case decay_e::Rotted: {
				return runes_t{ to_string(decay_e::Rotted), colors::light::Red };
			} case decay_e::Animate: {
				return runes_t{ to_string(decay_e::Animate), colors::light::Magenta };
			} case decay_e::Fresh: {
				return runes_t{ to_string(decay_e::Fresh), colors::light::Green };
			}
		}
	}

	enum struct galvanisation_e : u8 {
		Twisted = 0,
		Shimmering = Twisted + 1,
		Wriggling = Shimmering + 1,
		Writhing = Wriggling + 1,
	};

	constexpr galvanisation_e galvanise(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return galvanisation_e::Shimmering;
			} case galvanisation_e::Shimmering: {
				return galvanisation_e::Wriggling;
			} case galvanisation_e::Wriggling: {
				return galvanisation_e::Writhing;
			} case galvanisation_e::Writhing: {
				return galvanisation_e::Writhing;
			}
		}
	}

	constexpr cstr to_string(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return "twisted";
			} case galvanisation_e::Shimmering: {
				return "shimmering";
			} case galvanisation_e::Wriggling: {
				return "wriggling";
			} case galvanisation_e::Writhing: {
				return "writhing";
			}
		}
	}

	constexpr runes_t to_colored_string(galvanisation_e state) noexcept {
		switch (state) {
			case galvanisation_e::Twisted: {
				return runes_t{ to_string(galvanisation_e::Twisted), colors::metals::Tin };
			} case galvanisation_e::Shimmering: {
				return runes_t{ to_string(galvanisation_e::Shimmering), colors::light::Magenta };
			} case galvanisation_e::Wriggling: {
				return runes_t{ to_string(galvanisation_e::Wriggling), colors::light::Orange };
			} case galvanisation_e::Writhing: {
				return runes_t{ to_string(galvanisation_e::Writhing), colors::light::Green };
			}
		}
	}

	template<typename T> struct is_object {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_object_v = is_object<T>::value;

	template<typename T> concept Object = is_object<T>::value;

	template<Object T> struct to_object_enum {
		static constexpr object_e value = object_e::None;
	};

	template<Object T> constexpr object_e to_object_enum_v = to_object_enum<T>::value;

	template<> struct is_object<std::nullptr_t> {
		static constexpr bool value = true;
	};
	
	template<object_e ObjectType> struct to_object_type;
	
	template<> struct to_object_type<object_e::None> {
		using type = std::nullptr_t;
	};

	template<object_e ObjectType> using to_object_type_t = typename to_object_type<ObjectType>::type;

	template<typename T> struct is_null_object {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_null_object_v = is_null_object<T>::value;

	template<typename T> concept NullObject = Object<T> && is_null_object<T>::value;

	template<typename T> concept NonNullObject = Object<T> && !is_null_object<T>::value;

	template<> struct is_null_object<std::nullptr_t> {
		static constexpr bool value = true;
	};

	template<typename T> concept AnimatedObject = NonNullObject<T> && globals::has_animation<T>::value;

	template<typename T, object_e ObjectType> struct is_object_type {
		static constexpr bool value = false;
	};

	template<> struct is_object_type<std::nullptr_t, object_e::None> {
		static constexpr bool value = true;
	};

	template<typename T, object_e ObjectType> constexpr bool is_object_type_v = is_object_type<T, ObjectType>::value;

	template<Object ObjectType>
		requires (!globals::has_animation<ObjectType>::value)
	inline constexpr glyph_t object_glyphs;

	template<> inline constexpr glyph_t object_glyphs<std::nullptr_t>{ 0x40, colors::White };
} // namespace necrowarp
