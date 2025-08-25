#pragma once

#include <bleak.hpp>

#include <cstddef>

#include <necrowarp/cell.hpp>

#include <magic_enum/magic_enum_utility.hpp>
#include <magic_enum/magic_enum_switch.hpp>

namespace necrowarp {
	using namespace bleak;

	struct bones_t;
	struct flesh_t;
	struct metal_t;
	struct cerebra_t;

	struct crevice_t;
	struct ladder_t;
	struct portal_t;
	struct gateway_t;

	struct pedestal_t;

	struct arrow_t;

	#define ALL_CATALYST_OBJECTS \
		bones_t, \
		flesh_t, \
		metal_t, \
		cerebra_t

	#define ALL_LOCOMOTION_OBJECTS \
		crevice_t, \
		ladder_t, \
		portal_t, \
		gateway_t

	#define ALL_CONSTRUCT_OBJECTS \
		pedestal_t

	#define ALL_DEBRIS_OBJECTS \
		arrow_t

	#define ALL_ANIMATED_OBJECTS \
		metal_t, \
		ladder_t, \
		portal_t, \
		gateway_t

	#define ALL_OBJECTS \
		ALL_CATALYST_OBJECTS, \
		ALL_LOCOMOTION_OBJECTS, \
		ALL_CONSTRUCT_OBJECTS, \
		ALL_DEBRIS_OBJECTS

	enum struct object_e : u8 {
		None = 0,

		Bones,
		Flesh,
		Metal,
		Cerebra,

		Crevice,
		Ladder,
		Portal,
		Gateway,

		Pedestal,

		Arrow,
	};

	template<plurality_e Plurality = plurality_e::Singular> static constexpr cstr to_string(object_e object) noexcept;

	template<> constexpr cstr to_string<plurality_e::Singular>(object_e object) noexcept {
		switch (object) {
			case object_e::None: {
				return "none";
			} case object_e::Bones: {
				return "bone";
			} case object_e::Flesh: {
				return "flesh";
			} case object_e::Metal: {
				return "metal";
			} case object_e::Cerebra: {
				return "cerebrum";
			} case object_e::Crevice: {
				return "crevice";
			} case object_e::Ladder: {
				return "ladder";
			} case object_e::Portal: {
				return "portal";
			} case object_e::Gateway: {
				return "gateway";
			} case object_e::Pedestal: {
				return "pedestal";
			} case object_e::Arrow: {
				return "arrow";
			}
		}
	}

	template<> constexpr cstr to_string<plurality_e::Multiple>(object_e object) noexcept {
		switch (object) {
			case object_e::None: {
				return "none";
			} case object_e::Bones: {
				return "bones";
			} case object_e::Flesh: {
				return "flesh";
			} case object_e::Metal: {
				return "metal";
			} case object_e::Cerebra: {
				return "cerebra";
			} case object_e::Crevice: {
				return "crevices";
			} case object_e::Ladder: {
				return "ladders";
			} case object_e::Portal: {
				return "portals";
			} case object_e::Gateway: {
				return "gateways";
			} case object_e::Pedestal: {
				return "pedestals";
			} case object_e::Arrow: {
				return "arrows";
			}
		}
	}

	static constexpr color_t to_color(object_e object) noexcept {
		switch (object) {
			case object_e::None: {
				return colors::Grey;
			} case object_e::Bones: {
				return colors::White;
			} case object_e::Flesh: {
				return colors::Orange;
			} case object_e::Cerebra: {
				return mix(colors::Red, colors::Magenta);
			} case object_e::Metal: {
				return colors::metals::Iron;
			} case object_e::Crevice: {
				return mix(colors::materials::Oak, colors::dark::Grey);
			} case object_e::Ladder: {
				return colors::materials::Oak;
			} case object_e::Gateway: {
				return mix(colors::Blue, colors::Green);
			} case object_e::Portal: {
				return colors::light::Green;
			} case object_e::Pedestal: {
				return mix(colors::dark::Blue, colors::Grey);
			} case object_e::Arrow: {
				return colors::materials::Willow;
			}
		}
	}

	template<plurality_e Plurality = plurality_e::Singular> static constexpr runes_t to_colored_string(object_e object) noexcept {
		return runes_t{ to_string<Plurality>(object), to_color(object) };
	}

	constexpr usize ObjectTypeCount{ static_cast<usize>(object_e::Arrow) };

	using object_group_t = u16;

	enum struct object_group_e : object_group_t {
		None = 0,

		Bones = 1 << 0,
		Flesh = Bones << 1,
		Metal = Flesh << 1,
		Cerebra = Metal << 1,

		Crevice = Cerebra << 1,
		Ladder = Crevice << 1,
		Portal = Ladder << 1,
		Gateway = Portal << 1,

		Pedestal = Gateway << 1,

		Arrow = Pedestal << 1,
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

	constexpr object_e at(object_group_e group, usize index) noexcept;

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

	template<usize Index> using from_object_index = to_object_type<static_cast<object_e>(Index)>::type;
	
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

	template<typename T> struct is_blocking {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_blocking_v = is_blocking<T>::value;

	template<typename T> concept BlockingObject = NonNullObject<T> && is_blocking<T>::value;

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

	template<NonNullObject ObjectType>
		requires (!globals::has_animation<ObjectType>::value && !globals::has_variants<ObjectType>::value)
	inline constexpr glyph_t object_glyphs;
} // namespace necrowarp
