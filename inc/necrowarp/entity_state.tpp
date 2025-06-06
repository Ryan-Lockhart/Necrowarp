#pragma once

#include <necrowarp/entity_state.hpp>

#include <necrowarp/entities.hpp>
#include <necrowarp/commands.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

#include <optional>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;
	
	extern grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor;

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

	static inline bool update_camera() noexcept {
		bool force_width{ globals::MapSize.w <= globals::grid_size<grid_type_e::Game>().w };
		bool force_height{ globals::MapSize.h <= globals::grid_size<grid_type_e::Game>().h };

		bool moved{ false };

		if (force_width || force_height) {
			moved = camera.center_on(
				force_width, force_width ? globals::MapCenter.x : player.position.x,
				force_height, force_height ? globals::MapCenter.y : player.position.y
			);

			return moved;
		}

		if (camera_locked) {
			moved = camera.center_on(player.position);

			return moved;
		}

		return false;
	}

	template<NonPlayerEntity EntityType> static inline sparse_t<EntityType> entity_storage{};

	template<NonPlayerEntity EntityType, NonNullCommand CommandType> static inline std::queue<entity_command_t<EntityType, CommandType>> entity_commands{};

	static inline sparse_t<sparseling_t<bool>> newborns{};

	static inline field_t<float, globals::DistanceFunction, globals::MapSize, globals::BorderSize> good_goal_map{};
	static inline field_t<float, globals::DistanceFunction, globals::MapSize, globals::BorderSize> evil_goal_map{};

	template<NonNullEntity EntityType> static inline field_t<float, globals::DistanceFunction, globals::MapSize, globals::BorderSize> entity_goal_map{};

	static inline bool descent_flag{ false };

	static inline bool player_turn_invalidated{ false };

	static inline bool freshly_divine{ false };

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::contains(offset_t position) const noexcept { return entity_storage<EntityType>.contains(position); }

	template<PlayerEntity EntityType> inline bool entity_registry_t::contains(offset_t position) const noexcept { return player.position == position; }

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t::contains(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) || ...);
	}

	template<NullEntity EntityType> inline bool entity_registry_t::contains(offset_t position) const noexcept { return !contains<ALL_ENTITIES>(position); }

	inline bool entity_registry_t::contains(offset_t position) const noexcept { return contains<ALL_ENTITIES>(position); }

	inline entity_group_e entity_registry_t::at(offset_t position) const noexcept {
		entity_group_e entities{ entity_group_e::None };

		magic_enum::enum_for_each<entity_e>([this, position, &entities](auto val) -> void {
			constexpr entity_e cval{ val };

			using entity_type = to_entity_type<cval>::type;

			if (contains<entity_type>(position)) {
				entities += to_entity_group<cval>::value;
			}
		});
		
		return entities;
	}

	template<NonPlayerEntity EntityType> inline cptr<EntityType> entity_registry_t::at(offset_t position) const noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_storage<EntityType>[position];
	}

	template<PlayerEntity EntityType> inline cptr<EntityType> entity_registry_t::at(offset_t position) const noexcept {
		if (player.position != position) {
			return nullptr;
		}

		return &player;
	}

	template<NonPlayerEntity EntityType> inline ptr<EntityType> entity_registry_t::at(offset_t position) noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_storage<EntityType>[position];
	}

	template<PlayerEntity EntityType> inline ptr<EntityType> entity_registry_t::at(offset_t position) noexcept {
		if (player.position != position) {
			return nullptr;
		}

		return &player;
	}

	template<NonPlayerEntity EntityTypes> inline usize entity_registry_t::count() const noexcept {
		return entity_storage<EntityTypes>.size();
	}

	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_registry_t::count() const noexcept {
		return (count<EntityTypes>() + ...);
	}

	inline usize entity_registry_t::count() const noexcept { return count<ALL_ENTITIES>(); }
	
	template<NonPlayerEntity EntityType> inline bool entity_registry_t::empty() const noexcept {
		return entity_storage<EntityType>.empty();
	}
	
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t::empty() const noexcept {
		return (empty<EntityTypes>() && ...);
	}
	
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t::empty(offset_t position) const noexcept {
		return (empty<EntityTypes>(position) && ...);
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::empty(offset_t position) const noexcept { return !entity_storage<EntityType>.contains(position); }

	template<PlayerEntity EntityType> inline bool entity_registry_t::empty(offset_t position) const noexcept { return player.position != position; }

	inline bool entity_registry_t::empty(offset_t position) const noexcept { return empty<ALL_ENTITIES>(position); }

	template<bool Force, NonPlayerEntity EntityType> inline bool entity_registry_t::add(rval<EntityType> entity) noexcept {
		if constexpr (!Force) {
			if (entity_registry.contains(entity.position)) {
				return false;
			}
		}
		
		const offset_t position{ entity.position };
		const bool inserted{ entity_storage<EntityType>.add(std::move(entity)) };

		if (inserted) {
			if constexpr (is_evil_entity<EntityType>::value) {
				good_goal_map.add(position);
			} else if constexpr (is_good_entity<EntityType>::value) {
				evil_goal_map.add(position);
			}

			entity_goal_map<EntityType>.add(position);

			newborns.add(sparseling_t<bool>{ position });
		}

		return inserted;
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::remove(offset_t position) noexcept {
		if (entity_registry.empty(position)) {
			return false;
		}
		
		if (!entity_storage<EntityType>.remove(position)) {
			return false;
		}
		
		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map.remove(position);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map.remove(position);
		}

		entity_goal_map<EntityType>.remove(position);

		return true;
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::clear() noexcept {
		entity_storage<EntityType>.clear();
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::clear() noexcept {
		(clear<EntityTypes>(), ...);
	}

	inline void entity_registry_t::clear() noexcept {
		clear<ALL_NON_PLAYER>();

		player = player_t{};
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::reset() noexcept {
		entity_storage<EntityType>.clear();
		reset_goal_map<EntityType>();
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::reset() noexcept {
		(clear<EntityTypes>(), ...);
		(reset_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::reset() noexcept {
		clear();

		reset_goal_map();
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_e::Interior>(random_engine, cell_e::Open, entity_registry, object_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value() });
		}

		return true;
	}

	template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_e::Interior>(random_engine, cell_e::Open, entity_registry, object_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value(), args... });
		}

		return true;
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::spawn(usize count, u32 minimum_distance) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<EntityType>.template find_random<zone_region_e::Interior>(game_map, random_engine, cell_e::Open, entity_registry, object_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value() });

			entity_goal_map<EntityType>.template recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
		}

		return true;
	}

	template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		entity_goal_map<EntityType>.template recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<EntityType>.template find_random<zone_region_e::Interior>(game_map, random_engine, cell_e::Open, entity_registry, object_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value(), args... });

			entity_goal_map<EntityType>.template recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
		}

		return true;
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::update(offset_t current, offset_t target) noexcept {
		if (entity_registry.empty(current) || !game_map.within<zone_region_e::Interior>(current) || entity_registry.contains(target) || !game_map.within<zone_region_e::Interior>(target)) {
			return false;
		}
		
		if (!entity_storage<EntityType>.move(current, target)) {
			return false;
		}

		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map.update(current, target);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map.update(current, target);
		}

		entity_goal_map<EntityType>.update(current, target);

		return true;
	}

	template<PlayerEntity EntityType> inline bool entity_registry_t::update(offset_t current, offset_t target) noexcept {
		if (entity_registry.empty(current) || !game_map.within<zone_region_e::Interior>(current) || entity_registry.contains(target) || !game_map.within<zone_region_e::Interior>(target)) {
			return false;
		}

		player.position = target;
		good_goal_map.update(current, target);

		if constexpr (is_evil_entity<EntityType>::value) {
			good_goal_map.update(current, target);
		} else if constexpr (is_good_entity<EntityType>::value) {
			evil_goal_map.update(current, target);
		}

		entity_goal_map<EntityType>.update(current, target);

		return true;
	}

	template<PlayerEntity EntityType> inline void entity_registry_t::update() noexcept {
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
						
						command.process();
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
						
						command.process();
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
						
						command.process();
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

	template<NPCEntity EntityType> inline void entity_registry_t::update() noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			if (newborns.contains(entity.position)) {
				newborns.remove(entity.position);
				continue;
			}

			const command_pack_t pack{ entity.think() };

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

		entity_registry.process_commands<EntityType>();
	}

	template<NPCEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::update() noexcept {
		(update<EntityTypes>(), ...);
	}

	inline void entity_registry_t::update() noexcept {
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
			update_camera();
		}

		if (descent_flag) {
			return;
		}

		update<ALL_NON_PLAYER>();

		recalculate_goal_map();

		steam_stats::store();
	}

	template<NPCEntity EntityType, NonNullCommand CommandType> inline void entity_registry_t::process_commands() noexcept {
		if constexpr (is_entity_command_valid<EntityType, CommandType>::value) {
			while (!entity_commands<EntityType, CommandType>.empty()) {
				cauto command{ entity_commands<EntityType, CommandType>.front() };

				entity_commands<EntityType, CommandType>.pop();

				command.process();
			}
		}
	}

	template<NPCEntity EntityType, NonNullCommand... CommandTypes>
		requires is_plurary<CommandTypes...>::value
	inline void entity_registry_t::process_commands() noexcept {
		(process_commands<EntityType, CommandTypes>(), ...);
	}

	template<NPCEntity EntityType> inline void entity_registry_t::process_commands() noexcept {
		process_commands<EntityType, ALL_NPC_COMMANDS>();
	}

	template<NonNullEntity EntityType> inline void entity_registry_t::recalculate_goal_map() noexcept {
		entity_goal_map<EntityType>.template recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
	}

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::recalculate_goal_map() noexcept {
		(recalculate_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::recalculate_good_goal_map() noexcept {
		good_goal_map.recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
	}

	inline void entity_registry_t::recalculate_evil_goal_map() noexcept {
		evil_goal_map.recalculate<zone_region_e::Interior>(game_map, cell_e::Open, entity_registry);
	}

	inline void entity_registry_t::recalculate_alignment_goal_maps() noexcept {
		recalculate_good_goal_map();
		recalculate_evil_goal_map();
	}

	inline void entity_registry_t::recalculate_goal_map() noexcept {
		recalculate_goal_map<ALL_ENTITIES>();

		recalculate_alignment_goal_maps();
	}

	template<NonNullEntity EntityType> inline void entity_registry_t::reset_goal_map() noexcept {
		entity_goal_map<EntityType>.reset();
	}

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::reset_goal_map() noexcept {
		(reset_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::reset_good_goal_map() noexcept {
		good_goal_map.reset();
	}

	inline void entity_registry_t::reset_evil_goal_map() noexcept {
		evil_goal_map.reset();
	}

	inline void entity_registry_t::reset_alignment_goal_maps() noexcept {
		reset_good_goal_map();
		reset_evil_goal_map();
	}

	inline void entity_registry_t::reset_goal_map() noexcept {
		reset_goal_map<ALL_ENTITIES>();

		reset_alignment_goal_maps();
	}

	template<NonNullEntity EntityType, Command CommandType> inline bool entity_registry_t::is_command_valid(cref<entity_command_t<EntityType, CommandType>> command) const noexcept {
		static constexpr entity_e entity_enum{ to_entity_enum<EntityType>::value };
		static constexpr command_e command_enum{ to_command_enum<CommandType>::value };

		if constexpr (is_null_command<CommandType>::value) {
			return false;
		}

		if (!game_map.within<zone_region_e::Interior>(command.source_position) || !entity_registry.contains<EntityType>(command.source_position)) {
			return false;
		}

		if constexpr (is_npc_entity<EntityType>::value) {
			switch (command_enum) {
				case command_e::Descend:
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

		if (entity_enum != entity_e::Priest) {
			switch (command_enum) {
				case command_e::Exorcise:
				case command_e::Resurrect:
				case command_e::Anoint:
				case command_e::Suicide: {
					return false;
				} default: {
					break;
				}
			}
		}
		
		if constexpr (is_binary_command<CommandType>::value) {
			if (!game_map.within<zone_region_e::Interior>(command.target_position)) {
				return false;
			}

			switch (command_enum) {
				case command_e::Move:
				case command_e::TargetWarp: {
					if (game_map[command.target_position].solid || entity_registry.contains(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Clash:
				  case command_e::Lunge:
				  case command_e::Exorcise:
				  case command_e::Resurrect:
				  case command_e::Anoint: {
					if (entity_registry.empty(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Descend: {
					if (object_registry.empty(command.target_position) || !object_registry.contains<ladder_t>(command.target_position)) {
						return false;
					}

					cref<ladder_t> descension_point{ *object_registry.at<ladder_t>(command.target_position) };

					if (descension_point.is_up_ladder() || descension_point.has_shackle()) {
						return false;
					}

					break;
				} case command_e::Consume:
				  case command_e::ConsumeWarp: {
					if (entity_registry.empty(command.target_position) && object_registry.empty(command.target_position)) {
						return false;
					}

					break;
				} default: {
					break;
				}
			}

			if constexpr (is_ternary_command<CommandType>::value) {
				if (!game_map.within<zone_region_e::Interior>(command.intermediate_position)) {
					return false;
				}

				const entity_group_e intermediate_types{ entity_registry.at(command.intermediate_position) };

				switch (command_enum) {
					case command_e::Lunge: {
						if (entity_registry.contains(command.intermediate_position) || intermediate_types != entity_e::None) {
							return false;
						}
					} default: {
						break;
					}
				}
			}
		}		

		return true;
	}

	inline bool entity_registry_t::random_warp(offset_t source) noexcept {
		cauto random_safe_position{ evil_goal_map.find_random<zone_region_e::Interior>(game_map, random_engine, cell_e::Open, entity_registry, object_registry, 8) };

		if (!random_safe_position.has_value()) {
			cauto random_unsafe_position{ game_map.find_random<zone_region_e::Interior>(random_engine, cell_e::Open, entity_registry, object_registry) };

			if (!random_unsafe_position.has_value()) {
				player.receive_failed_warp_boon();

				draw_warp_cursor = false;

				return false;
			}

			player.receive_unsafe_warp_boon();

			entity_registry.update<player_t>(source, random_unsafe_position.value());

			++steam_stats::stats<steam_stat_e::RandomWarps, i32>;

			steam_stats::stats<steam_stat_e::MetersWarped, f32> += offset_t::distance<f32>(source, player.position);

			warp_cursor.set(player.position);
			draw_warp_cursor = true;

			return true;
		}

		entity_registry.update<player_t>(source, random_safe_position.value());

		++steam_stats::stats<steam_stat_e::RandomWarps, i32>;

		steam_stats::stats<steam_stat_e::MetersWarped, f32> += offset_t::distance<f32>(source, player.position);

		warp_cursor.set(player.position);
		draw_warp_cursor = true;

		return true;
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::draw() const noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			entity.draw();
		}
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto entity : entity_storage<EntityType>) {
			if (!viewport.within(entity.position)) {
				continue;
			}

			entity.draw(camera);
		}
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto entity : entity_storage<EntityType>) {
			if (!viewport.within(entity.position)) {
				continue;
			}

			entity.draw(camera, offset);
		}
	}

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::draw() const noexcept {
		(draw<EntityTypes>(), ...);
	}

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		(draw<EntityTypes>(camera), ...);
	}

	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		(draw<EntityTypes>(camera, offset), ...);
	}

	inline void entity_registry_t::draw() const noexcept {
		draw<ALL_NON_PLAYER>();

		player.draw();
	}

	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		draw<ALL_NON_PLAYER>(camera);

		player.draw(camera);
	}

	inline void entity_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		draw<ALL_NON_PLAYER>(camera, offset);

		player.draw(camera, offset);
	}
} // namespace necrowarp

#include <necrowarp/entities.tpp>
#include <necrowarp/commands.tpp>
