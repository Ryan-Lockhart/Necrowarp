#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType> struct object_buffer_t {
		inline object_group_e at(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline cptr<ObjectType> at(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline ptr<ObjectType> at(offset_t position) noexcept;

		template<NonNullObject ObjectType> inline usize count() const noexcept;

		template<NullObject ObjectType> inline usize count() const noexcept;

		template<Object... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline usize count() const noexcept;

		inline usize count() const noexcept;

		template<NonNullObject ObjectType> inline bool empty() const noexcept;

		template<NonNullObject ObjectType> inline bool empty(offset_t position) const noexcept;

		inline bool empty(offset_t position) const noexcept;

		template<Object... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool empty() const noexcept;

		template<Object... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool empty(offset_t position) const noexcept;

		constexpr bool empty() const noexcept { return false; }

		inline bool contains(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline bool contains(offset_t position) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool contains(offset_t position) const noexcept;

		template<NullObject ObjectType> inline bool contains(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline void draw() const noexcept;

		template<NonNullObject ObjectType> inline void draw(cref<camera_t> camera) const noexcept;

		template<NonNullObject ObjectType> inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw(cref<camera_t> camera) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;

		inline void draw() const noexcept;

		inline void draw(cref<camera_t> camera) const noexcept;

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;
	};
} // namespace necrowarp
