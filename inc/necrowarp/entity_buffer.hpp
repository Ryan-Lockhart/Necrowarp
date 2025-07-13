#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType> struct entity_buffer_t {
		inline entity_group_e at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<PlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<NonPlayerEntity EntityType> inline usize count() const noexcept;

		template<PlayerEntity EntityType> inline usize count() const noexcept { return 1; }

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline usize count() const noexcept;

		inline usize count() const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline usize count(offset_t position) const noexcept;

		inline usize count(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline bool empty() const noexcept;

		template<PlayerEntity EntityType> inline bool empty() const noexcept { return false; }

		template<NonPlayerEntity EntityType> inline bool empty(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool empty(offset_t position) const noexcept;

		inline bool empty(offset_t position) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool empty() const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool empty(offset_t position) const noexcept;

		constexpr bool empty() const noexcept { return false; }

		template<NonPlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool contains(offset_t position) const noexcept;

		inline bool contains(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity EntityType> inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity EntityType> inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline void draw() const noexcept;

		template<PlayerEntity EntityType> inline void draw() const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(offset_t offset) const noexcept;

		template<PlayerEntity EntityType> inline void draw(offset_t offset) const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(offset_t offset, offset_t nudge) const noexcept;

		template<PlayerEntity EntityType> inline void draw(offset_t offset, offset_t nudge) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw() const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(offset_t offset) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(offset_t offset, offset_t nudge) const noexcept;

		inline void draw() const noexcept;

		inline void draw(offset_t offset) const noexcept;

		inline void draw(offset_t offset, offset_t nudge) const noexcept;
	};
} // namespace necrowarp
