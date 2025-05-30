#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct object_registry_t {
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

		template<bool Force = false, NonNullObject T> inline bool add(rval<T> object) noexcept;

		template<NonNullObject ObjectType> inline bool remove(offset_t position) noexcept;

		template<NonNullObject ObjectType> inline void clear() noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void clear() noexcept;

		inline void clear() noexcept;

		template<NonNullObject ObjectType> inline void reset() noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void reset() noexcept;

		inline void reset() noexcept;

		template<NonNullObject ObjectType> inline bool spawn(usize count) noexcept;

		template<NonNullObject ObjectType, typename... Args> inline bool spawn(usize count, Args... args) noexcept;

		template<NonNullObject ObjectType> inline bool spawn(usize count, u32 minimum_distance) noexcept;

		template<NonNullObject ObjectType, typename... Args> inline bool spawn(usize count, u32 minimum_distance, Args... args) noexcept;

		template<NonNullObject ObjectType> inline bool update(offset_t current, offset_t target) noexcept;

		template<NonNullObject ObjectType> inline void recalculate_goal_map() noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void recalculate_goal_map() noexcept;

		inline void recalculate_goal_map() noexcept;

		template<NonNullObject ObjectType> inline void reset_goal_map() noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void reset_goal_map() noexcept;

		inline void reset_goal_map() noexcept;

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
	} static inline object_registry{};
} // namespace necrowarp
