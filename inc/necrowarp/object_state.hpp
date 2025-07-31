#pragma once

#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>
#include <necrowarp/object_buffer.hpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType> struct object_registry_t {
		template<NonNullObject ObjectType> inline void store() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void store() const noexcept;

		inline object_group_e at(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline cptr<ObjectType> at(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline ptr<ObjectType> at(offset_t position) noexcept;

		template<NonNullObject ObjectType> inline usize count() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline usize count() const noexcept;

		inline usize count() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline usize count(offset_t position) const noexcept;

		inline usize count(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline bool empty() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool empty() const noexcept;

		inline bool empty() const noexcept;

		template<NonNullObject ObjectType> inline bool empty(offset_t position) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool empty(offset_t position) const noexcept;

		inline bool empty(offset_t position) const noexcept;

		template<NonNullObject ObjectType> inline bool contains(offset_t position) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool contains(offset_t position) const noexcept;

		inline bool contains(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullObject ObjectType> inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullObject ObjectType> inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<NonNullObject ObjectType, bool Force = false> inline bool add(offset_t position) noexcept;

		template<bool Force = false, NonNullObject ObjectType> inline bool add(offset_t position, rval<ObjectType> object) noexcept;

		template<NonNullObject ObjectType> inline bool spill(offset_t position, rval<ObjectType> object) noexcept;

		template<NonNullObject ObjectType> inline bool spill(offset_t position, rval<ObjectType> object, usize amount) noexcept;

		template<NonNullObject ObjectType> inline bool remove(offset_t position) noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void remove(offset_t position) noexcept;

		inline void remove(offset_t position) noexcept;

		template<NonNullObject ObjectType> inline void clear() noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void clear() noexcept;

		inline void clear() noexcept;

		template<NonNullObject ObjectType> inline bool spawn(usize count) noexcept;

		template<NonNullObject ObjectType, typename... Args> inline bool spawn(usize count, Args... args) noexcept;

		template<NonNullObject ObjectType> inline bool spawn(usize count, u32 minimum_distance) noexcept;

		template<NonNullObject ObjectType, typename... Args> inline bool spawn(usize count, u32 minimum_distance, Args... args) noexcept;

		template<NonNullObject ObjectType> inline bool update(offset_t current, offset_t target) noexcept;

		template<AnimatedObject ObjectType> inline void advance() noexcept;

		template<AnimatedObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void advance() noexcept;
		
		inline void advance() noexcept;

		template<AnimatedObject ObjectType> inline void retreat() noexcept;

		template<AnimatedObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void retreat() noexcept;

		inline void retreat() noexcept;

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

		template<NonNullObject ObjectType> inline void draw(offset_t offset) const noexcept;

		template<NonNullObject ObjectType> inline void draw(offset_t offset, offset_t nudge) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw() const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw(offset_t offset) const noexcept;

		template<NonNullObject... ObjectTypes>
			requires is_plurary<ObjectTypes...>::value
		inline void draw(offset_t offset, offset_t nudge) const noexcept;

		inline void draw() const noexcept;

		inline void draw(offset_t offset) const noexcept;

		inline void draw(offset_t offset, offset_t nudge) const noexcept;
	};
} // namespace necrowarp
