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

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void store() const noexcept;

		inline void store() const noexcept;

		inline std::optional<entity_e> at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline cptr<EntityType> at(offset_t position) const noexcept;

		template<NonPlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<PlayerEntity EntityType> inline ptr<EntityType> at(offset_t position) noexcept;

		template<NonPlayerEntity EntityType> inline usize count() const noexcept;

		template<PlayerEntity EntityType> inline usize count() const noexcept { return 1; }

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

		template<NonPlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool contains(offset_t position) const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool contains(offset_t position) const noexcept;

		inline bool contains(offset_t position) const noexcept;

		template<distance_function_e Distance, Entity... EntityTypes> inline bool nearby(offset_t position) const noexcept;

		template<distance_function_e Distance, Entity... EntityTypes> inline std::optional<offset_t> nearest(offset_t position) const noexcept;

		template<PlayerEntity EntityType> inline bool add(offset_t position) noexcept;

		template<NonPlayerEntity T, bool Force = false> inline bool add(offset_t position) noexcept;

		template<bool Force = false, NonPlayerEntity T> inline bool add(offset_t position, rval<T> entity) noexcept;

		template<NonPlayerEntity EntityType> inline bool remove(offset_t position) noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void remove(offset_t position) noexcept;

		inline void remove(offset_t position) noexcept;

		template<NonPlayerEntity EntityType, NonNullCommand CommandType> inline void clear() noexcept;

		template<NonPlayerEntity EntityType, NonNullCommand... CommandTypes>
			requires is_plurary<CommandTypes...>::value
		inline void clear() noexcept;

		template<PlayerEntity EntityType> inline void clear() noexcept;

		template<NonPlayerEntity EntityType> inline void clear() noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void clear() noexcept;

		inline void clear() noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, Args... args) noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count, u32 minimum_distance) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, u32 minimum_distance, Args... args) noexcept;

		template<NonPlayerEntity EntityType> inline bool update(offset_t current, offset_t target) noexcept;

		template<PlayerEntity EntityType> inline bool update(offset_t current, offset_t target) noexcept;

		template<Entity EntityType, Command CommandType> inline bool is_command_valid(cref<entity_command_t<EntityType, CommandType>> command) const noexcept;

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

		template<PlayerEntity EntityType, typename Function>
			requires std::is_invocable<Function, cref<EntityType>>::value
		inline void execute(rval<Function> func) const noexcept;

		template<PlayerEntity EntityType, typename Function>
			requires std::is_invocable<Function, ref<EntityType>>::value
		inline void execute(rval<Function> func) noexcept;

		template<NonPlayerEntity EntityType, typename Function>
			requires std::is_invocable<Function, offset_t, cref<EntityType>>::value
		inline void execute(rval<Function> func) const noexcept;

		template<NonPlayerEntity EntityType, typename Function>
			requires std::is_invocable<Function, offset_t, ref<EntityType>>::value
		inline void execute(rval<Function> func) noexcept;

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

		inline void rescan_medicus_goals() noexcept;

		template<Entity EntityType> inline void recalculate_goal_map() noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void recalculate_goal_map() noexcept;

		inline void recalculate_goal_map() noexcept;

		inline void recalculate_alignment_goal_maps() noexcept;

		inline void recalculate_medicus_goal_map() noexcept;

		inline void recalculate_skulker_goal_map() noexcept;

		inline void recalculate_specialist_goal_maps() noexcept;

		inline void recalculate_unique_goal_maps() noexcept;

		template<Entity EntityType> inline void reset_goal_map() noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void reset_goal_map() noexcept;

		inline void reset_goal_map() noexcept;

		inline void reset_alignment_goal_maps() noexcept;

		inline void reset_specialist_goal_maps() noexcept;

		inline void reset_unique_goal_maps() noexcept;

		template<GoodEntity EntityType> inline bool depart() noexcept;

		template<GoodEntity EntityType> inline bool depart(usize epoch) noexcept;

		template<GoodEntity EntityType> inline void clear_offmap() noexcept;

		template<GoodEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void clear_offmap() noexcept;

		inline void clear_offmap() noexcept;

		template<GoodEntity EntityType> inline void tick_offmap() noexcept;

		template<GoodEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void tick_offmap() noexcept;

		inline void tick_offmap() noexcept;

		template<GoodEntity EntityType> inline bool any_offmap() const noexcept;

		template<GoodEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool any_offmap() const noexcept;

		inline bool any_offmap() const noexcept;

		template<GoodEntity EntityType> inline usize offmap_count() const noexcept;

		template<GoodEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline usize offmap_count() const noexcept;

		inline usize offmap_count() const noexcept;

		template<NonPlayerEntity EntityType> inline void draw() const noexcept;

		template<PlayerEntity EntityType> inline void draw() const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(offset_t offset) const noexcept;

		template<PlayerEntity EntityType> inline void draw(offset_t offset) const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(offset_t offset, offset_t nudge) const noexcept;

		template<PlayerEntity EntityType> inline void draw(offset_t offset, offset_t nudge) const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw() const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(offset_t offset) const noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(offset_t offset, offset_t nudge) const noexcept;

		inline void draw() const noexcept;

		inline void draw(offset_t offset) const noexcept;

		inline void draw(offset_t offset, offset_t nudge) const noexcept;
	};
} // namespace necrowarp
