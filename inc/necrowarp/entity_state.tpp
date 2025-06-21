#pragma once

#include <necrowarp/entity_state.hpp>

#include <necrowarp/entities.hpp>
#include <necrowarp/commands.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

#include <atomic>
#include <optional>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;
	
	template<map_type_e MapType> extern grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor;

	extern bool draw_cursor;
	extern bool draw_warp_cursor;

	static inline player_t player{};

	template<patron_e Patron> constexpr runes_t to_colored_string() noexcept {
		runes_t colored_string{};

		magic_enum::enum_for_each<discount_e>([&](auto val) -> void {
			constexpr discount_e cval{ val };

			colored_string.concatenate({ std::format("{}:{}", to_string(cval), std::string(padding_size(cval), ' ')) });

			colored_string.concatenate(to_colored_string<Patron>(cval));

			if constexpr (cval == discount_e::TargetWarp || cval == discount_e::GalvanicInvocation) {
				colored_string.concatenate(runes_t{ "\n" });
			} if (cval == discount_e::NecromanticAscendance) {
				return;
			} else {
				colored_string.concatenate(runes_t{ "\n\n" });
			}
		});

		return colored_string;
	}

	template<map_type_e MapType> static inline bool update_camera() noexcept {
		bool force_width{ globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w };
		bool force_height{ globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h };

		bool moved{ false };

		if (force_width || force_height) {
			moved = camera<MapType>.center_on(
				force_width, force_width ? globals::MapCenter<MapType>.x : player.position.x,
				force_height, force_height ? globals::MapCenter<MapType>.y : player.position.y
			);

			return moved;
		}

		if (camera_locked) {
			moved = camera<MapType>.center_on(player.position);

			return moved;
		}

		return false;
	}

	template<map_type_e MapType> static inline entity_registry_t<MapType> entity_registry{};

	template<NonPlayerEntity EntityType> static inline sparse_t<EntityType> entity_storage{};

	template<NonPlayerEntity EntityType, NonNullCommand CommandType> static inline std::queue<entity_command_t<EntityType, CommandType>> entity_commands{};

	static inline sparse_t<sparseling_t<bool>> newborns{};
	static inline sparse_t<sparseling_t<bool>> stunned{};

	template<map_type_e MapType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> good_goal_map{};
	template<map_type_e MapType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> evil_goal_map{};

	template<map_type_e MapType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> ranger_goal_map{};
	template<map_type_e MapType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> skulker_goal_map{};

	template<map_type_e MapType, NonNullEntity EntityType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> entity_goal_map{};

	static inline volatile std::atomic<bool> descent_flag{ false };

	static inline volatile std::atomic<bool> plunge_flag{ false };

	static inline volatile std::atomic<dimension_e> plunge_target{ dimension_e::Abyss };

	static inline volatile std::atomic<bool> player_turn_invalidated{ false };

	static inline volatile std::atomic<bool> freshly_divine{ false };

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return entity_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return player.position == position; }

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) || ...);
	}

	template<map_type_e MapType> template<NullEntity EntityType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return !contains<ALL_ENTITIES>(position); }

	template<map_type_e MapType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return contains<ALL_ENTITIES>(position); }

	template<map_type_e MapType> template<distance_function_e Distance, NonNullEntity EntityType> inline bool entity_registry_t<MapType>::nearby(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<Distance>) {
			cauto current_pos{ position + offset };

			if (contains<EntityType>(current_pos)) {
				return true;
			}
		}

		return false;
	}

	template<map_type_e MapType>
	template<distance_function_e Distance, NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::nearby(offset_t position) const noexcept {
		return (nearby<Distance, EntityTypes>(position) || ...);
	}

	template<map_type_e MapType> template<distance_function_e Distance, NonNullEntity EntityType> inline std::optional<offset_t> entity_registry_t<MapType>::nearest(offset_t position) const noexcept {
		if (empty<EntityType>()) {
			return std::nullopt;
		}

		if (contains<EntityType>(position)) {
			return position;
		}

		std::queue<creeper_t<offset_t::float_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		if (frontier.empty()) {
			return std::nullopt;
		}

		while (!frontier.empty()) {
			const creeper_t<offset_t::float_t> current{ frontier.front() };
			frontier.pop();

			if (contains<EntityType>(current.position)) {
				return current.position;
			}

			visited.insert(current.position);

			for (cauto offset : neighbourhood_offsets<Distance>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !contains<EntityType>(offset_position)) {
					continue;
				}

				frontier.emplace(offset_position, offset_t::float_t{ current.distance + 1 });
			}
		}

		return std::nullopt;
	}

	template<map_type_e MapType>
	template<distance_function_e Distance, NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline std::optional<offset_t> entity_registry_t<MapType>::nearest(offset_t position) const noexcept {
		if (empty<EntityTypes...>()) {
			return std::nullopt;
		}

		if (contains<EntityTypes...>(position)) {
			return position;
		}

		std::queue<creeper_t<offset_t::float_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		if (frontier.empty()) {
			return std::nullopt;
		}

		while (!frontier.empty()) {
			const creeper_t<offset_t::float_t> current{ frontier.front() };
			frontier.pop();

			if (contains<EntityTypes...>(current.position)) {
				return current.position;
			}

			visited.insert(current.position);

			for (cauto offset : neighbourhood_offsets<Distance>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !contains<EntityTypes...>(offset_position)) {
					continue;
				}

				frontier.emplace(offset_position, offset_t::float_t{ current.distance + 1 });
			}
		}

		return std::nullopt;
	}

	template<map_type_e MapType> inline entity_group_e entity_registry_t<MapType>::at(offset_t position) const noexcept {
		entity_group_e entities{ entity_group_e::None };

		magic_enum::enum_for_each<entity_e>([&](auto val) -> void {
			constexpr entity_e cval{ val };

			using entity_type = to_entity_type<cval>::type;

			if (contains<entity_type>(position)) {
				entities += to_entity_group<cval>::value;
			}
		});
		
		return entities;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline cptr<EntityType> entity_registry_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_storage<EntityType>[position];
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline cptr<EntityType> entity_registry_t<MapType>::at(offset_t position) const noexcept {
		if (player.position != position) {
			return nullptr;
		}

		return &player;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline ptr<EntityType> entity_registry_t<MapType>::at(offset_t position) noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_storage<EntityType>[position];
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline ptr<EntityType> entity_registry_t<MapType>::at(offset_t position) noexcept {
		if (player.position != position) {
			return nullptr;
		}

		return &player;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline usize entity_registry_t<MapType>::count() const noexcept {
		return entity_storage<EntityType>.size();
	}

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_registry_t<MapType>::count() const noexcept {
		return (count<EntityTypes>() + ...);
	}

	template<map_type_e MapType> inline usize entity_registry_t<MapType>::count() const noexcept { return count<ALL_ENTITIES>(); }
	
	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty() const noexcept {
		return entity_storage<EntityType>.empty();
	}
	
	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::empty() const noexcept {
		return (empty<EntityTypes>() && ...);
	}
	
	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<EntityTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return !entity_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return player.position != position; }

	template<map_type_e MapType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_ENTITIES>(position); }

	template<map_type_e MapType> template<bool Force, NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::add(rval<EntityType> entity) noexcept {
		if constexpr (!Force) {
			if (contains(entity.position)) {
				return false;
			}
		}
		
		const offset_t position{ entity.position };
		const bool inserted{ entity_storage<EntityType>.add(std::move(entity)) };

		if (inserted) {
			if constexpr (is_evil_entity<EntityType>::value) {
				good_goal_map<MapType>.add(position);
			} else if constexpr (is_good_entity<EntityType>::value) {
				evil_goal_map<MapType>.add(position);
			}

			entity_goal_map<MapType, EntityType>.add(position);

			newborns.add(sparseling_t<bool>{ position });
		}

		return inserted;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::remove(offset_t position) noexcept {
		if (empty(position)) {
			return false;
		}
		
		if (!entity_storage<EntityType>.remove(position)) {
			return false;
		}
		
		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map<MapType>.remove(position);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map<MapType>.remove(position);
		}

		entity_goal_map<MapType, EntityType>.remove(position);

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::clear() noexcept {
		entity_storage<EntityType>.clear();
	}

	template<map_type_e MapType>
	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::clear() noexcept {
		(clear<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::clear() noexcept {
		clear<ALL_NON_PLAYER>();

		player = player_t{};
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::reset() noexcept {
		entity_storage<EntityType>.clear();
		reset_goal_map<EntityType>();
	}

	template<map_type_e MapType>
	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::reset() noexcept {
		(clear<EntityTypes>(), ...);
		(reset_goal_map<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset() noexcept {
		clear();

		reset_goal_map();
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(EntityType{ maybe_position.value() });
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t<MapType>::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(EntityType{ maybe_position.value(), args... });
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::spawn(usize count, u32 minimum_distance) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<MapType, EntityType>.template find_random<zone_region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(EntityType{ maybe_position.value() });

			entity_goal_map<MapType, EntityType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t<MapType>::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		entity_goal_map<MapType, EntityType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<MapType, EntityType>.template find_random<zone_region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(EntityType{ maybe_position.value(), args... });

			entity_goal_map<MapType, EntityType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.template within<zone_region_e::Interior>(current) || contains(target) || !game_map<MapType>.template within<zone_region_e::Interior>(target)) {
			return false;
		}
		
		if (!entity_storage<EntityType>.move(current, target)) {
			return false;
		}

		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map<MapType>.update(current, target);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map<MapType>.update(current, target);
		}

		entity_goal_map<MapType, EntityType>.update(current, target);

		if (stunned.contains(current)) {
			stunned.move(current, target);
		}

		return true;
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.template within<zone_region_e::Interior>(current) || contains(target) || !game_map<MapType>.template within<zone_region_e::Interior>(target)) {
			return false;
		}

		player.position = target;
		good_goal_map<MapType>.update(current, target);

		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map<MapType>.update(current, target);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map<MapType>.update(current, target);
		}

		entity_goal_map<MapType, EntityType>.update(current, target);

		return true;
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline void entity_registry_t<MapType>::update() noexcept {
		const command_pack_t pack{ player.command };

		magic_enum::enum_switch([&](auto val) -> void {
			constexpr command_e cval{ val };

			using command_type = to_command_type<cval>::type;			

			if constexpr (is_null_command<command_type>::value) {
				return;
			} else {
				if constexpr (is_entity_command_valid<EntityType, command_type>::value) {
					if constexpr (is_unary_command<command_type>::value) {
						if (!pack.source_position.has_value()) {
							player_turn_invalidated = true;
							return;
						}

						const entity_command_t<EntityType, command_type> command{ pack.source_position.value() };
						
						if (!is_command_valid<EntityType, command_type>(command)) {
							player_turn_invalidated = true;
							return;
						}
						
						command.template process<MapType>();
					} else if constexpr (is_binary_command<command_type>::value) {
						if (!pack.source_position.has_value() || !pack.target_position.has_value()) {
							player_turn_invalidated = true;
							return;
						}

						const entity_command_t<EntityType, command_type> command{ pack.source_position.value(), pack.target_position.value() };
						
						if (!is_command_valid<EntityType, command_type>(command)) {
							player_turn_invalidated = true;
							return;
						}
						
						command.template process<MapType>();
					} else if constexpr (is_ternary_command<command_type>::value) {
						if (!pack.source_position.has_value() || !pack.intermediate_position.has_value() || !pack.target_position.has_value()) {
							player_turn_invalidated = true;
							return;
						}

						const entity_command_t<EntityType, command_type> command{
							pack.source_position.value(),
							pack.intermediate_position.value(),
							pack.target_position.value()
						};
						
						if (!is_command_valid<EntityType, command_type>(command)) {
							player_turn_invalidated = true;
							return;
						}
						
						command.template process<MapType>();
					} else {
						return;
					}
				} else {
					player_turn_invalidated = true;
					return;
				}
			}
		}, pack.type);
	}

	template<map_type_e MapType> template<NPCEntity EntityType> inline void entity_registry_t<MapType>::update() noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			if (newborns.contains(entity.position)) {
				newborns.remove(entity.position);
				continue;
			}

			if (stunned.contains(entity.position)) {
				stunned.remove(entity.position);
				continue;
			}

			const command_pack_t pack{ entity.template think<MapType>() };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr command_e cval{ val };

				using command_type = to_command_type<cval>::type;

				if constexpr (is_null_command<command_type>::value) {
					return;
				} else {
					if constexpr (is_entity_command_valid<EntityType, command_type>::value) {
						if constexpr (is_unary_command<command_type>::value) {
							if (!pack.source_position.has_value()) {
								return;
							}

							const entity_command_t<EntityType, command_type> command{ pack.source_position.value() };
							
							if (!is_command_valid<EntityType, command_type>(command)) {
								return;
							}

							entity_commands<EntityType, command_type>.push(command);
						} else if constexpr (is_binary_command<command_type>::value) {
							if (!pack.source_position.has_value() || !pack.target_position.has_value()) {
								return;
							}

							const entity_command_t<EntityType, command_type> command{ pack.source_position.value(), pack.target_position.value() };
							
							if (!is_command_valid<EntityType, command_type>(command)) {
								return;
							}

							entity_commands<EntityType, command_type>.push(command);
						} else if constexpr (is_ternary_command<command_type>::value) {
							if (!pack.source_position.has_value() || !pack.intermediate_position.has_value() || !pack.target_position.has_value()) {
								return;
							}

							const entity_command_t<EntityType, command_type> command{
								pack.source_position.value(),
								pack.intermediate_position.value(),
								pack.target_position.value()
							};
							
							if (!is_command_valid<EntityType, command_type>(command)) {
								return;
							}

							entity_commands<EntityType, command_type>.push(command);
						} else {
							return;
						}
					}
				}
			}, pack.type);
		}

		process_commands<EntityType>();
	}

	template<map_type_e MapType>
	template<NPCEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::update() noexcept {
		(update<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::update() noexcept {
		update<player_t>();

		if (player_turn_invalidated) {
			player_turn_invalidated = false;

			return;
		}

		if (!freshly_divine && player.has_ascended()) {
			player.erode_divinity();
		}

		freshly_divine = false;

		if (camera_locked) {
			update_camera<MapType>();
		}

		if (descent_flag || plunge_flag) {
			return;
		}

		update<ALL_NON_PLAYER>();

		recalculate_goal_map();

		steam_stats::store();
	}

	template<map_type_e MapType> template<AnimatedEntity EntityType> inline void entity_registry_t<MapType>::advance() noexcept {
		for (crauto entity : entity_storage<EntityType>) { entity.idle_animation.advance(); }
	}

	template<map_type_e MapType> 
	template<AnimatedEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::advance() noexcept {
		(advance<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::advance() noexcept { advance<ALL_ANIMATED_ENTITIES>(); }

	template<map_type_e MapType> template<AnimatedEntity EntityType> inline void entity_registry_t<MapType>::retreat() noexcept {
		for (crauto entity : entity_storage<EntityType>) { entity.idle_animation.retreat(); }
	}

	template<map_type_e MapType> 
	template<AnimatedEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::retreat() noexcept {
		(retreat<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::retreat() noexcept { retreat<ALL_ANIMATED_ENTITIES>(); }

	template<map_type_e MapType> template<NPCEntity EntityType, NonNullCommand CommandType> inline void entity_registry_t<MapType>::process_commands() noexcept {
		if constexpr (is_entity_command_valid<EntityType, CommandType>::value) {
			while (!entity_commands<EntityType, CommandType>.empty()) {
				cauto command{ entity_commands<EntityType, CommandType>.front() };

				entity_commands<EntityType, CommandType>.pop();

				command.template process<MapType>();
			}
		}
	}

	template<map_type_e MapType>
	template<NPCEntity EntityType, NonNullCommand... CommandTypes>
		requires is_plurary<CommandTypes...>::value
	inline void entity_registry_t<MapType>::process_commands() noexcept {
		(process_commands<EntityType, CommandTypes>(), ...);
	}

	template<map_type_e MapType> template<NPCEntity EntityType> inline void entity_registry_t<MapType>::process_commands() noexcept {
		process_commands<EntityType, ALL_NPC_COMMANDS>();
	}

	template<map_type_e MapType> template<NonNullEntity EntityType> inline void entity_registry_t<MapType>::recalculate_goal_map() noexcept {
		entity_goal_map<MapType, EntityType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::recalculate_goal_map() noexcept {
		(recalculate_goal_map<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_good_goal_map() noexcept {
		good_goal_map<MapType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_evil_goal_map() noexcept {
		evil_goal_map<MapType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_alignment_goal_maps() noexcept {
		recalculate_good_goal_map();
		recalculate_evil_goal_map();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_ranger_goal_map() noexcept {
		ranger_goal_map<MapType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_skulker_goal_map() noexcept {
		skulker_goal_map<MapType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_specialist_goal_maps() noexcept {
		recalculate_ranger_goal_map();
		recalculate_skulker_goal_map();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_goal_map() noexcept {
		recalculate_goal_map<ALL_ENTITIES>();

		recalculate_alignment_goal_maps();
		recalculate_specialist_goal_maps();
	}

	template<map_type_e MapType> template<NonNullEntity EntityType> inline void entity_registry_t<MapType>::reset_goal_map() noexcept {
		entity_goal_map<MapType, EntityType>.reset();
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::reset_goal_map() noexcept {
		(reset_goal_map<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_good_goal_map() noexcept {
		good_goal_map<MapType>.reset();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_evil_goal_map() noexcept {
		evil_goal_map<MapType>.reset();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_alignment_goal_maps() noexcept {
		reset_good_goal_map();
		reset_evil_goal_map();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_goal_map() noexcept {
		reset_goal_map<ALL_ENTITIES>();

		reset_alignment_goal_maps();
	}

	template<map_type_e MapType> template<NonNullEntity EntityType, Command CommandType> inline bool entity_registry_t<MapType>::is_command_valid(cref<entity_command_t<EntityType, CommandType>> command) const noexcept {
		static constexpr entity_e entity_enum{ to_entity_enum<EntityType>::value };
		static constexpr command_e command_enum{ to_command_enum<CommandType>::value };

		if constexpr (is_null_command<CommandType>::value) {
			return false;
		}

		if (!game_map<MapType>.template within<zone_region_e::Interior>(command.source_position) || !contains<EntityType>(command.source_position)) {
			return false;
		}

		if constexpr (is_npc_entity<EntityType>::value) {
			switch (command_enum) {
				case command_e::Descend:
				case command_e::Plunge:
				case command_e::Consume:
				case command_e::RandomWarp:
				case command_e::TargetWarp:
				case command_e::ConsumeWarp:
				case command_e::CalciticInvocation:
				case command_e::SpectralInvocation:
				case command_e::SanguineInvocation:
				case command_e::GalvanicInvocation:
				case command_e::NecromanticAscendance: {
					return false;
				} default: {
					break;
				}
			}
		}

		if constexpr (entity_enum != entity_e::Bloodhound) {
			switch (command_enum) {
				case command_e::Lunge: {
					return false;
				} default: {
					break;
				}
			}
		}

		if constexpr (entity_enum != entity_e::Wraith) {
			switch (command_enum) {
				case command_e::Eviscerate: {
					return false;
				} default: {
					break;
				}
			}
		}

		if constexpr (entity_enum != entity_e::Ranger) {
			switch (command_enum) {
				case command_e::Knock:
				case command_e::Retrieve:
				case command_e::Loose: {
					return false;
				} default: {
					break;
				}
			}
		}

		if constexpr (entity_enum != entity_e::BattleMonk) {
			switch (command_enum) {
				case command_e::Meditate:
				case command_e::Batter: {
					return false;
				} default: {
					break;
				}
			}
		}
		
		if constexpr (is_binary_command<CommandType>::value) {
			if (!game_map<MapType>.template within<zone_region_e::Interior>(command.target_position)) {
				return false;
			}

			switch (command_enum) {
				case command_e::Move:
				case command_e::TargetWarp: {
					if (game_map<MapType>[command.target_position].solid || contains(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Clash:
				  case command_e::Lunge:
				  case command_e::Loose:
				  case command_e::Batter: {
					if (empty(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Descend: {
					if (object_registry<MapType>.empty(command.target_position) || !object_registry<MapType>.template contains<ladder_t>(command.target_position)) {
						return false;
					}

					cptr<ladder_t> ladder{ object_registry<MapType>.template at<ladder_t>(command.target_position) };

					if (ladder == nullptr || ladder->is_up_ladder() || ladder->has_shackle()) {
						return false;
					}

					break;
				} case command_e::Plunge: {
					if (object_registry<MapType>.empty(command.target_position) || !object_registry<MapType>.template contains<portal_t>(command.target_position)) {
						return false;
					}

					cptr<portal_t> portal{ object_registry<MapType>.template at<portal_t>(command.target_position) };

					if (portal == nullptr || portal->stability == current_dimension) {
						return false;
					}

					break;
				} case command_e::Consume:
				  case command_e::ConsumeWarp: {
					if (empty(command.target_position) && object_registry<MapType>.empty(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Retrieve: {
					if (object_registry<MapType>.empty(command.target_position) || !object_registry<MapType>.template contains<arrow_t>(command.target_position)) {
						return false;
					}

					break;
				} default: {
					break;
				}
			}

			if constexpr (is_ternary_command<CommandType>::value) {
				if (!game_map<MapType>.template within<zone_region_e::Interior>(command.intermediate_position)) {
					return false;
				}

				const entity_group_e intermediate_types{ at(command.intermediate_position) };

				switch (command_enum) {
					case command_e::Lunge: {
						if (!empty(command.intermediate_position) || intermediate_types != entity_e::None) {
							return false;
						}
					} default: {
						return false;
					}
				}
			}
		}		

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw() const noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			entity.draw();
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto entity : entity_storage<EntityType>) {
			if (!viewport.within(entity.position)) {
				continue;
			}

			entity.draw(camera);
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto entity : entity_storage<EntityType>) {
			if (!viewport.within(entity.position)) {
				continue;
			}

			entity.draw(camera, offset);
		}
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::draw() const noexcept {
		(draw<EntityTypes>(), ...);
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		(draw<EntityTypes>(camera), ...);
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		(draw<EntityTypes>(camera, offset), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw() const noexcept {
		draw<ALL_NON_PLAYER>();

		player.draw();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		draw<ALL_NON_PLAYER>(camera);

		player.draw(camera);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		draw<ALL_NON_PLAYER>(camera, offset);

		player.draw(camera, offset);
	}
} // namespace necrowarp

#include <necrowarp/entities.tpp>
#include <necrowarp/commands.tpp>
