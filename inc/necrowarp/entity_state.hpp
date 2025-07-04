#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<map_type_e MapType> struct entity_registry_t {
		template<PlayerEntity EntityType> inline void store() const noexcept;

		template<NonPlayerEntity EntityType> inline void store() const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void store() const noexcept;

		inline entity_group_e at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<PlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<NonPlayerEntity EntityType> inline usize count() const noexcept;

		template<PlayerEntity EntityType> inline usize count() const noexcept { return 1; }

		template<NullEntity EntityType> inline usize count() const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline usize count() const noexcept;

		inline usize count() const noexcept;

		template<NonPlayerEntity EntityType> inline bool empty() const noexcept;

		template<PlayerEntity EntityType> inline bool empty() const noexcept { return false; }

		template<NonPlayerEntity EntityType> inline bool empty(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool empty(offset_t position) const noexcept;

		inline bool empty(offset_t position) const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool empty() const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool empty(offset_t position) const noexcept;

		constexpr bool empty() const noexcept { return false; }

		inline bool contains(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool contains(offset_t position) const noexcept;

		template<NullEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity EntityType> inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity EntityType> inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<distance_function_e Distance, NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool add(offset_t position) noexcept;

		template<bool Force = false, NonPlayerEntity T> inline bool add(rval<T> entity) noexcept;

		template<NonPlayerEntity EntityType> inline bool remove(offset_t position) noexcept;

		template<PlayerEntity EntityType> inline void clear() noexcept;

		template<NonPlayerEntity EntityType> inline void clear() noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void clear() noexcept;

		inline void clear() noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, Args... args) noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count, u32 minimum_distance) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, u32 minimum_distance, Args... args) noexcept;

		template<NonPlayerEntity EntityType> inline bool update(offset_t current, offset_t target) noexcept;

		template<PlayerEntity EntityType> inline bool update(offset_t current, offset_t target) noexcept;

		template<NonNullEntity EntityType, Command CommandType> inline bool is_command_valid(cref<entity_command_t<EntityType, CommandType>> command) const noexcept;

		template<NPCEntity EntityType, NonNullCommand... CommandTypes>
			requires is_plurary<CommandTypes...>::value
		inline void process_commands() noexcept;

		template<NPCEntity EntityType, NonNullCommand CommandType> inline void process_commands() noexcept;

		template<NPCEntity EntityType> inline void process_commands() noexcept;

		template<PlayerEntity EntityType> inline void update() noexcept;

		template<NPCEntity EntityType> inline void update() noexcept;

		template<NPCEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void update() noexcept;

		inline void update() noexcept;

		template<AnimatedEntity EntityType> inline void advance() noexcept;

		template<AnimatedEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void advance() noexcept;

		inline void advance() noexcept;

		template<AnimatedEntity EntityType> inline void retreat() noexcept;

		template<AnimatedEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void retreat() noexcept;

		inline void retreat() noexcept;

		template<NonNullEntity EntityType> inline void recalculate_goal_map() noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void recalculate_goal_map() noexcept;

		inline void recalculate_goal_map() noexcept;

		inline void recalculate_good_goal_map() noexcept;

		inline void recalculate_evil_goal_map() noexcept;

		inline void recalculate_alignment_goal_maps() noexcept;

		inline void recalculate_ranger_goal_map() noexcept;

		inline void recalculate_skulker_goal_map() noexcept;

		inline void recalculate_specialist_goal_maps() noexcept;

		inline void recalculate_unique_goal_maps() noexcept;

		template<NonNullEntity EntityType> inline void reset_goal_map() noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void reset_goal_map() noexcept;

		inline void reset_goal_map() noexcept;

		inline void reset_good_goal_map() noexcept;

		inline void reset_evil_goal_map() noexcept;

		inline void reset_alignment_goal_maps() noexcept;

		inline void reset_ranger_goal_map() noexcept;

		inline void reset_skulker_goal_map() noexcept;

		inline void reset_specialist_goal_maps() noexcept;

		inline void reset_unique_goal_maps() noexcept;

		template<NonPlayerEntity EntityType> inline void draw() const noexcept;

		template<PlayerEntity EntityType> inline void draw() const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(cref<camera_t> camera) const noexcept;

		template<PlayerEntity EntityType> inline void draw(cref<camera_t> camera) const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;

		template<PlayerEntity EntityType> inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw() const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(cref<camera_t> camera) const noexcept;

		template<NonNullEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;

		inline void draw() const noexcept;

		inline void draw(cref<camera_t> camera) const noexcept;

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept;
	};
} // namespace necrowarp
