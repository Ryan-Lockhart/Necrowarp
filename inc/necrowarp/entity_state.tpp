#pragma once

#include <necrowarp/entity_state.hpp>

#include <necrowarp/entities.hpp>
#include <necrowarp/commands.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

#include <atomic>
#include <optional>

#include <bleak/sparse.hpp>

#include <necrowarp/entity_buffer.hpp>

namespace necrowarp {	
	using namespace bleak;

	static inline bool camera_overridden{ false };

	template<typename T> struct is_entity_registry {
		static constexpr bool value{ false };
	};

	template<typename T> constexpr bool is_entity_registry_v = is_entity_registry<T>::value;

	template<typename T> concept EntityRegistry = is_entity_registry<T>::value;

	template<map_type_e MapType> struct is_entity_registry<entity_registry_t<MapType>> {
		static constexpr bool value{ true };
	};

	template<map_type_e MapType> struct is_entity_registry<entity_buffer_t<MapType>> {
		static constexpr bool value{ true };
	};
	
	template<map_type_e MapType> extern grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor;

	extern bool draw_cursor;

	extern std::optional<offset_t> warped_from;

	static inline player_t player{};

	static inline player_t player_buffer{};

	template<patron_e Patron> constexpr runes_t to_colored_string() noexcept {
		runes_t colored_string{};

		magic_enum::enum_for_each<grimoire_e>([&](auto val) -> void {
			constexpr grimoire_e cval{ val };

			colored_string.concatenate({ std::format("{}:{}", to_string(cval), std::string(padding_size(cval), ' ')) });

			colored_string.concatenate(to_colored_string<Patron>(cval));

			if constexpr (cval == grimoire_e::PreciseWarp || cval == grimoire_e::Incorporealize || cval == grimoire_e::InfernalInvocation) {
				colored_string.concatenate(runes_t{ "\n" });
			} if (cval == grimoire_e::CalamitousRetaliation) {
				return;
			} else {
				colored_string.concatenate(runes_t{ "\n\n" });
			}
		});

		return colored_string;
	}

	template<map_type_e MapType> static inline bool update_camera() noexcept {
		if (camera_overridden) {
			return false;
		}

		bool force_width{ globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w };
		bool force_height{ globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h };

		bool moved{ false };

		const std::optional<offset_t> player_position{
			[&]() -> std::optional<offset_t> {
				if (!registry_access.is_locked()) {
					return player.position;
				} else if (!buffer_access.is_locked()) {
					return player_buffer.position;
				} else {
					return std::nullopt;
				}
			}()
		};

		if (force_width || force_height) {
			moved = camera<MapType>.center_on(
				force_width, globals::MapCenter<MapType>.x,
				force_height, globals::MapCenter<MapType>.y
			);

			return moved;
		}

		if (camera_locked && player_position.has_value()) {
			moved = camera<MapType>.center_on(player_position.value());

			return moved;
		}

		return false;
	}

	template<map_type_e MapType> static inline bool update_camera(offset_t override) noexcept {
		bool force_width{ globals::MapSize<MapType>.w <= globals::grid_size<grid_type_e::Game>().w };
		bool force_height{ globals::MapSize<MapType>.h <= globals::grid_size<grid_type_e::Game>().h };

		bool moved{ false };

		if (force_width || force_height) {
			moved = camera<MapType>.center_on(
				force_width, globals::MapCenter<MapType>.x,
				force_height, globals::MapCenter<MapType>.y
			);

			return moved;
		}

		moved = camera<MapType>.center_on(override);

		return moved;
	}

	template<map_type_e MapType> static inline entity_registry_t<MapType> entity_registry{};

	template<map_type_e MapType> static inline entity_buffer_t<MapType> entity_buffer{};

	template<NonPlayerEntity EntityType> static inline sparse_t<EntityType> entity_registry_storage{};

	template<NonPlayerEntity EntityType> static inline sparse_t<EntityType> entity_buffer_storage{};

	template<NonPlayerEntity EntityType, NonNullCommand CommandType> static inline std::queue<entity_command_t<EntityType, CommandType>> entity_commands{};

	static inline sparse_t<bool> deceased{};

	static inline sparse_t<bool> concussed{};

	static inline sparse_t<affliction_e> afflicted{};

	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> evil_goal_map{};
	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> good_goal_map{};
	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> neutral_goal_map{};

	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> non_evil_goal_map{};
	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> non_good_goal_map{};
	template<map_type_e MapType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> non_neutral_goal_map{};

	template<map_type_e MapType> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> ranger_goal_map{};
	template<map_type_e MapType> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> skulker_goal_map{};

	template<map_type_e MapType, NonNullEntity EntityType, bool Incorporeal = false> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> entity_goal_map{};

	static inline volatile std::atomic<bool> descent_flag{ false };

	static inline volatile std::atomic<bool> plunge_flag{ false };

	static inline volatile std::atomic<dimension_e> plunge_target{ dimension_e::Abyss };

	static inline volatile std::atomic<bool> player_turn_invalidated{ false };

	static inline volatile std::atomic<bool> freshly_divine{ false };

	static inline volatile std::atomic<bool> freshly_incorporeal{ false };

	static inline volatile std::atomic<bool> divine_intervention_invoked{ false };

	template<map_type_e MapType> template<PlayerEntity EntityType> inline void entity_registry_t<MapType>::store() const noexcept {
		player_buffer = player;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::store() const noexcept {
		entity_buffer_storage<EntityType> = entity_registry_storage<EntityType>;
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::store() const noexcept {
		(store<EntityTypes>(), ...);
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return entity_registry_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept { return player.position == position; }

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) || ...);
	}

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

				if (!game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open ||!visited.insert(offset_position).second) {
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

			if ((contains<EntityTypes>(current.position) || ...)) {
				return current.position;
			}

			visited.insert(current.position);

			for (cauto offset : neighbourhood_offsets<Distance>) {
				const offset_t offset_position{ current.position + offset };

				if (!game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open ||!visited.insert(offset_position).second) {
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

			if constexpr (!is_null_entity<entity_type>::value) {
				if (contains<entity_type>(position)) {
					entities += to_entity_group<cval>::value;
				}
			}
		});
		
		return entities;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline cptr<EntityType> entity_registry_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_registry_storage<EntityType>[position];
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

		return entity_registry_storage<EntityType>[position];
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline ptr<EntityType> entity_registry_t<MapType>::at(offset_t position) noexcept {
		if (player.position != position) {
			return nullptr;
		}

		return &player;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline usize entity_registry_t<MapType>::count() const noexcept {
		return entity_registry_storage<EntityType>.size();
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_registry_t<MapType>::count() const noexcept {
		return (count<EntityTypes>() + ...);
	}

	template<map_type_e MapType> inline usize entity_registry_t<MapType>::count() const noexcept { return count<ALL_ENTITIES>(); }

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_registry_t<MapType>::count(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) + ...);
	}

	template<map_type_e MapType> inline usize entity_registry_t<MapType>::count(offset_t position) const noexcept { return count<ALL_ENTITIES>(position); }
	
	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty() const noexcept {
		return entity_registry_storage<EntityType>.empty();
	}
	
	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::empty() const noexcept {
		return (empty<EntityTypes>() && ...);
	}
	
	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<EntityTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return !entity_registry_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return player.position != position; }

	template<map_type_e MapType> inline bool entity_registry_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_ENTITIES>(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::add(offset_t position) noexcept {
		if (contains<ALL_NON_PLAYER>(position)) {
			return false;
		}

		player.position = position;

		if constexpr (is_non_good<EntityType>::value) {
			non_good_goal_map<MapType, false>.add(position);
			non_good_goal_map<MapType, true>.add(position);

			ranger_goal_map<MapType>.add(position);
		}

		if constexpr (is_non_evil<EntityType>::value) {
			non_evil_goal_map<MapType, false>.add(position);
			non_evil_goal_map<MapType, true>.add(position);
		}

		if constexpr (is_non_neutral<EntityType>::value) {
			non_neutral_goal_map<MapType, false>.add(position);
			non_neutral_goal_map<MapType, true>.add(position);
		}

		if constexpr (is_good<EntityType>::value) {
			good_goal_map<MapType, false>.add(position);
			good_goal_map<MapType, true>.add(position);
		}

		if constexpr (is_evil<EntityType>::value) {
			evil_goal_map<MapType, false>.add(position);
			evil_goal_map<MapType, true>.add(position);
		}

		if constexpr (is_neutral<EntityType>::value) {
			neutral_goal_map<MapType, false>.add(position);
			neutral_goal_map<MapType, true>.add(position);
		}

		if constexpr (is_vigilant<EntityType>::value) {
			skulker_goal_map<MapType>.add(position);
		}

		entity_goal_map<MapType, EntityType, false>.add(position);
		entity_goal_map<MapType, EntityType, true>.add(position);

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType, bool Force> inline bool entity_registry_t<MapType>::add(offset_t position) noexcept {
		if constexpr (!Force) {
			if (contains(position)) {
				return false;
			}
		}

		const bool inserted{ entity_registry_storage<EntityType>.add(position, EntityType{}) };

		if (inserted) {
			if constexpr (is_non_good<EntityType>::value) {
				non_good_goal_map<MapType, false>.add(position);
				non_good_goal_map<MapType, true>.add(position);

				ranger_goal_map<MapType>.add(position);
			}

			if constexpr (is_non_evil<EntityType>::value) {
				non_evil_goal_map<MapType, false>.add(position);
				non_evil_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_non_neutral<EntityType>::value) {
				non_neutral_goal_map<MapType, false>.add(position);
				non_neutral_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_good<EntityType>::value) {
				good_goal_map<MapType, false>.add(position);
				good_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_evil<EntityType>::value) {
				evil_goal_map<MapType, false>.add(position);
				evil_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_neutral<EntityType>::value) {
				neutral_goal_map<MapType, false>.add(position);
				neutral_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_vigilant<EntityType>::value) {
				skulker_goal_map<MapType>.add(position);
			}

			entity_goal_map<MapType, EntityType, false>.add(position);
			entity_goal_map<MapType, EntityType, true>.add(position);

			if constexpr (is_afflictable<EntityType>::value) {
				afflicted.add(position, affliction_e::Stable);
			}
		}

		return inserted;
	}

	template<map_type_e MapType> template<bool Force, NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::add(offset_t position, rval<EntityType> entity) noexcept {
		if constexpr (!Force) {
			if (contains(position)) {
				return false;
			}
		}

		const bool inserted{ entity_registry_storage<EntityType>.add(position, std::move(entity)) };

		if (inserted) {
			if constexpr (is_non_good<EntityType>::value) {
				non_good_goal_map<MapType, false>.add(position);
				non_good_goal_map<MapType, true>.add(position);

				ranger_goal_map<MapType>.add(position);
			}

			if constexpr (is_non_evil<EntityType>::value) {
				non_evil_goal_map<MapType, false>.add(position);
				non_evil_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_non_neutral<EntityType>::value) {
				non_neutral_goal_map<MapType, false>.add(position);
				non_neutral_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_good<EntityType>::value) {
				good_goal_map<MapType, false>.add(position);
				good_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_evil<EntityType>::value) {
				evil_goal_map<MapType, false>.add(position);
				evil_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_neutral<EntityType>::value) {
				neutral_goal_map<MapType, false>.add(position);
				neutral_goal_map<MapType, true>.add(position);
			}

			if constexpr (is_vigilant<EntityType>::value) {
				skulker_goal_map<MapType>.add(position);
			}

			entity_goal_map<MapType, EntityType, false>.add(position);
			entity_goal_map<MapType, EntityType, true>.add(position);

			if constexpr (is_afflictable<EntityType>::value) {
				afflicted.add(position, affliction_e::Stable);
			}
		}

		return inserted;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::remove(offset_t position) noexcept {
		if (empty(position)) {
			return false;
		}
		
		if (!entity_registry_storage<EntityType>.remove(position)) {
			return false;
		}

		if constexpr (is_non_good<EntityType>::value) {
			non_good_goal_map<MapType, false>.remove(position);
			non_good_goal_map<MapType, true>.remove(position);

			ranger_goal_map<MapType>.remove(position);
		}

		if constexpr (is_non_evil<EntityType>::value) {
			non_evil_goal_map<MapType, false>.remove(position);
			non_evil_goal_map<MapType, true>.remove(position);
		}

		if constexpr (is_non_neutral<EntityType>::value) {
			non_neutral_goal_map<MapType, false>.remove(position);
			non_neutral_goal_map<MapType, true>.remove(position);
		}

		if constexpr (is_good<EntityType>::value) {
			good_goal_map<MapType, false>.remove(position);
			good_goal_map<MapType, true>.remove(position);
		}

		if constexpr (is_evil<EntityType>::value) {
			evil_goal_map<MapType, false>.remove(position);
			evil_goal_map<MapType, true>.remove(position);
		}

		if constexpr (is_neutral<EntityType>::value) {
			neutral_goal_map<MapType, false>.remove(position);
			neutral_goal_map<MapType, true>.remove(position);
		}

		if constexpr (is_vigilant<EntityType>::value) {
			skulker_goal_map<MapType>.remove(position);
		}

		entity_goal_map<MapType, EntityType, false>.remove(position);
		entity_goal_map<MapType, EntityType, true>.remove(position);

		if constexpr (is_concussable<EntityType>::value) {
			concussed.remove(position);
		}

		if constexpr (is_afflictable<EntityType>::value) {
			afflicted.remove(position);
		}

		return true;
	}

	template<map_type_e MapType>
	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::remove(offset_t position) noexcept {
		(remove<EntityTypes>(position), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::remove(offset_t position) noexcept {
		remove<ALL_NON_PLAYER>(position);
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline void entity_registry_t<MapType>::clear() noexcept {
		player.reset();
		reset_goal_map<EntityType>();
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::clear() noexcept {
		entity_registry_storage<EntityType>.clear();
		reset_goal_map<EntityType>();
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::clear() noexcept {
		(clear<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::clear() noexcept {
		clear<ALL_ENTITIES>();

		reset_alignment_goal_maps();
		reset_specialist_goal_maps();
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), EntityType{});
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t<MapType>::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), EntityType{ args... });
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::spawn(usize count, u32 minimum_distance) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<MapType, EntityType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), EntityType{});

			entity_goal_map<MapType, EntityType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t<MapType>::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		entity_goal_map<MapType, EntityType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<MapType, EntityType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, entity_registry<MapType>, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), EntityType{ args... });

			entity_goal_map<MapType, EntityType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.dependent within<region_e::Interior>(current) || contains(target) || !game_map<MapType>.dependent within<region_e::Interior>(target)) {
			return false;
		}
		
		if (!entity_registry_storage<EntityType>.update(current, target)) {
			return false;
		}

		if constexpr (is_non_good<EntityType>::value) {
			non_good_goal_map<MapType, false>.update(current, target);
			non_good_goal_map<MapType, true>.update(current, target);

			ranger_goal_map<MapType>.update(current, target);
		}

		if constexpr (is_non_evil<EntityType>::value) {
			non_evil_goal_map<MapType, false>.update(current, target);
			non_evil_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_non_neutral<EntityType>::value) {
			non_neutral_goal_map<MapType, false>.update(current, target);
			non_neutral_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_good<EntityType>::value) {
			good_goal_map<MapType, false>.update(current, target);
			good_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_evil<EntityType>::value) {
			evil_goal_map<MapType, false>.update(current, target);
			evil_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_neutral<EntityType>::value) {
			neutral_goal_map<MapType, false>.update(current, target);
			neutral_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_vigilant<EntityType>::value) {
			skulker_goal_map<MapType>.update(current, target);
		}

		entity_goal_map<MapType, EntityType, false>.update(current, target);
		entity_goal_map<MapType, EntityType, true>.update(current, target);

		if constexpr (is_concussable<EntityType>::value) {
			if (concussed.contains(current)) {
				concussed.update(current, target);
			}
		}

		if constexpr (is_afflictable<EntityType>::value) {
			if (afflicted.contains(current)) {
				afflicted.update(current, target);
			}
		}

		return true;
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.dependent within<region_e::Interior>(current) || contains(target) || !game_map<MapType>.dependent within<region_e::Interior>(target)) {
			return false;
		}

		player.position = target;

		if constexpr (is_non_good<EntityType>::value) {
			non_good_goal_map<MapType, false>.update(current, target);
			non_good_goal_map<MapType, true>.update(current, target);

			ranger_goal_map<MapType>.update(current, target);
		}

		if constexpr (is_non_evil<EntityType>::value) {
			non_evil_goal_map<MapType, false>.update(current, target);
			non_evil_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_non_neutral<EntityType>::value) {
			non_neutral_goal_map<MapType, false>.update(current, target);
			non_neutral_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_good<EntityType>::value) {
			good_goal_map<MapType, false>.update(current, target);
			good_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_evil<EntityType>::value) {
			evil_goal_map<MapType, false>.update(current, target);
			evil_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_neutral<EntityType>::value) {
			neutral_goal_map<MapType, false>.update(current, target);
			neutral_goal_map<MapType, true>.update(current, target);
		}

		if constexpr (is_vigilant<EntityType>::value) {
			skulker_goal_map<MapType>.update(current, target);
		}

		entity_goal_map<MapType, EntityType, false>.update(current, target);
		entity_goal_map<MapType, EntityType, true>.update(current, target);

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
						
						command.dependent process<MapType>();
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
						
						command.dependent process<MapType>();
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
						
						command.dependent process<MapType>();
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
		if constexpr (is_sneaky<EntityType>::value) {
			recalculate_skulker_goal_map();
		}

		for (crauto [position, entity] : entity_registry_storage<EntityType>) {
			if (concussed.contains(position)) {
				concussed.remove(position);
				continue;
			}

			const command_pack_t pack{ entity.dependent think<MapType>(position) };

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

		if (!freshly_incorporeal && player.is_incorporeal()) {
			player.erode_phantasm();
		}

		freshly_incorporeal = false;

		if (game_map<MapType>[player.position].solid && !player.is_incorporeal()) {
			const death_info_t<death_e::Crushed> info{ player.die<MapType, death_e::Crushed>() };

			if (info.perished) {
				return;
			}
		}

		if (camera_locked) {
			update_camera<MapType>();
		}

		if (descent_flag || plunge_flag) {
			return;
		}

		for (rauto [_, draugaz] : entity_registry_storage<draugaz_t>) {
			draugaz.regenerate();
		}

		entity_registry<MapType>.recalculate_goal_map();
		object_registry<MapType>.recalculate_goal_map();

		update<ALL_NON_PLAYER>();

		steam_stats::store();
	}

	template<map_type_e MapType>
	template<PlayerEntity EntityType, typename Function>
		requires std::is_invocable<Function, cref<EntityType>>::value
	inline void entity_registry_t<MapType>::execute(rval<Function> func) const noexcept {
		for (crauto [_, entity] : entity_registry_storage<EntityType>) {
			func(entity);
		}
	}

	template<map_type_e MapType>
	template<PlayerEntity EntityType, typename Function>
		requires std::is_invocable<Function, ref<EntityType>>::value
	inline void entity_registry_t<MapType>::execute(rval<Function> func) noexcept {
		for (rauto [_, entity] : entity_registry_storage<EntityType>) {
			func(entity);
		}
	}

	template<map_type_e MapType>
	template<NonPlayerEntity EntityType, typename Function>
		requires std::is_invocable<Function, offset_t, cref<EntityType>>::value
	inline void entity_registry_t<MapType>::execute(rval<Function> func) const noexcept {
		for (crauto [position, entity] : entity_registry_storage<EntityType>) {
			func(position, entity);
		}
	}

	template<map_type_e MapType>
	template<NonPlayerEntity EntityType, typename Function>
		requires std::is_invocable<Function, offset_t, ref<EntityType>>::value
	inline void entity_registry_t<MapType>::execute(rval<Function> func) noexcept {
		for (rauto [position, entity] : entity_registry_storage<EntityType>) {
			func(position, entity);
		}
	}

	template<map_type_e MapType> template<AnimatedEntity EntityType> inline void entity_registry_t<MapType>::advance() noexcept {
		for (crauto [_, entity] : entity_registry_storage<EntityType>) {
			entity.idle_animation.advance();
		}
	}

	template<map_type_e MapType>
	template<AnimatedEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::advance() noexcept {
		(advance<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::advance() noexcept { advance<ALL_ANIMATED_ENTITIES>(); }

	template<map_type_e MapType> template<AnimatedEntity EntityType> inline void entity_registry_t<MapType>::retreat() noexcept {
		for (crauto [_, entity] : entity_registry_storage<EntityType>) {
			entity.idle_animation.retreat();
		}
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

				command.dependent process<MapType>();
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
		entity_goal_map<MapType, EntityType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::recalculate_goal_map() noexcept {
		(recalculate_goal_map<EntityTypes>(), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_alignment_goal_maps() noexcept {
		evil_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		evil_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);

		good_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		good_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);

		neutral_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		neutral_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);

		non_evil_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		non_evil_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);

		non_good_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		non_good_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);

		non_neutral_goal_map<MapType, false>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, entity_registry<MapType>);
		non_neutral_goal_map<MapType, true>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Explored, entity_registry<MapType>);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_skulker_goal_map() noexcept {
		skulker_goal_map<MapType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_specialist_goal_maps() noexcept {
		recalculate_skulker_goal_map();

		ranger_goal_map<MapType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_unique_goal_maps() noexcept {
		recalculate_alignment_goal_maps();
		recalculate_specialist_goal_maps();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::recalculate_goal_map() noexcept {
		recalculate_goal_map<ALL_ENTITIES>();
		recalculate_unique_goal_maps();
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

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_alignment_goal_maps() noexcept {
		evil_goal_map<MapType, false>.reset();
		evil_goal_map<MapType, true>.reset();

		good_goal_map<MapType, false>.reset();
		good_goal_map<MapType, true>.reset();

		neutral_goal_map<MapType, false>.reset();
		neutral_goal_map<MapType, true>.reset();

		non_evil_goal_map<MapType, false>.reset();
		non_evil_goal_map<MapType, true>.reset();

		non_good_goal_map<MapType, false>.reset();
		non_good_goal_map<MapType, true>.reset();

		non_neutral_goal_map<MapType, false>.reset();
		non_neutral_goal_map<MapType, true>.reset();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_specialist_goal_maps() noexcept {
		skulker_goal_map<MapType>.reset();
		ranger_goal_map<MapType>.reset();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_unique_goal_maps() noexcept {
		reset_alignment_goal_maps();
		reset_specialist_goal_maps();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::reset_goal_map() noexcept {
		reset_goal_map<ALL_ENTITIES>();
		reset_unique_goal_maps();
	}

	template<map_type_e MapType> template<NonNullEntity EntityType, Command CommandType> inline bool entity_registry_t<MapType>::is_command_valid(cref<entity_command_t<EntityType, CommandType>> command) const noexcept {
		static constexpr command_e command_enum{ to_command_enum<CommandType>::value };

		if constexpr (is_null_command<CommandType>::value) {
			return false;
		}

		if (!game_map<MapType>.dependent within<region_e::Interior>(command.source_position) || !contains<EntityType>(command.source_position)) {
			return false;
		}

		if constexpr (!is_entity_command_valid<EntityType, CommandType>::value) {
			return false;
		}

		if constexpr (is_binary_command<CommandType>::value) {
			if (!game_map<MapType>.dependent within<region_e::Interior>(command.target_position)) {
				return false;
			}

			switch (command_enum) {
				case command_e::Move:
				case command_e::PreciseWarp: {
					if (contains(command.target_position)) {
						return false;
					}

					if (game_map<MapType>[command.target_position].solid) {
						if constexpr (is_incorporeal<EntityType>::value) {
							if constexpr (is_incorporeal<EntityType>::conditional) {
								if (cptr<EntityType> entity{ at<EntityType>(command.source_position) }; entity == nullptr || !entity->is_incorporeal()) {
									return false;
								} else {
									break;
								}
							} else {
								break;
							}
						}

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
				} case command_e::Devour: {
					if (empty(command.target_position) && object_registry<MapType>.dependent empty<flesh_t>(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Descend: {
					if (object_registry<MapType>.empty(command.target_position) || !object_registry<MapType>.dependent contains<ladder_t>(command.target_position)) {
						return false;
					}

					cptr<ladder_t> ladder{ object_registry<MapType>.dependent at<ladder_t>(command.target_position) };

					if (ladder == nullptr || ladder->is_up_ladder() || ladder->has_shackle()) {
						return false;
					}

					break;
				} case command_e::Plunge: {
					if (object_registry<MapType>.empty(command.target_position) || !object_registry<MapType>.dependent contains<portal_t>(command.target_position)) {
						return false;
					}

					cptr<portal_t> portal{ object_registry<MapType>.dependent at<portal_t>(command.target_position) };

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
					if (object_registry<MapType>.empty(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Calcify: {
					if (!game_map<MapType>[command.target_position].solid && object_registry<MapType>.dependent empty<bones_t>(command.target_position)) {
						return false;
					}

					break;
				} case command_e::Annihilate: {
					if (empty(command.source_position) || !empty(command.target_position) || command.source_position == command.target_position || game_map<MapType>.linear_blockage(command.source_position, command.target_position, cell_e::Solid)) {
						return false;
					}

					break;
				} default: {
					break;
				}
			}

			if constexpr (is_ternary_command<CommandType>::value) {
				if (!game_map<MapType>.dependent within<region_e::Interior>(command.intermediate_position)) {
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

	template<map_type_e MapType> inline bool entity_registry_t<MapType>::is_concussed(offset_t position) const noexcept { return concussed.contains(position); }
	
	template<map_type_e MapType> inline bool entity_registry_t<MapType>::is_afflicted(offset_t position) const noexcept { return afflicted.contains(position); }

	template<map_type_e MapType> inline std::optional<affliction_e> entity_registry_t<MapType>::get_affliction(offset_t position) const noexcept {
		if (!is_afflicted(position)) {
			return std::nullopt;
		}

		cptr<affliction_e> maybe_affliction{ afflicted[position] };

		if (maybe_affliction == nullptr) {
			return std::nullopt;
		}

		return *maybe_affliction;
	}

	template<map_type_e MapType> template<affliction_e Affliction> bool entity_registry_t<MapType>::has_affliction(offset_t position) const noexcept {
		if (!is_afflicted(position)) {
			return false;
		}

		cauto maybe_affliction{ get_affliction(position) };

		if (!maybe_affliction.has_value()) {
			return false;
		}

		return maybe_affliction.value() == Affliction;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw() const noexcept {
		for (crauto [position, entity] : entity_registry_storage<EntityType>) {
			entity.draw(position);
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw(offset_t offset) const noexcept {
		for (crauto [position, entity] : entity_registry_storage<EntityType>) {
			entity.draw(position, offset);
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		for (crauto [position, entity] : entity_registry_storage<EntityType>) {
			entity.draw(position, offset, nudge);
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
	inline void entity_registry_t<MapType>::draw(offset_t offset) const noexcept {
		(draw<EntityTypes>(offset), ...);
	}

	template<map_type_e MapType>
	template<NonNullEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		(draw<EntityTypes>(offset, nudge), ...);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw() const noexcept {
		draw<ALL_NON_PLAYER>();

		player.draw();
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw(offset_t offset) const noexcept {
		draw<ALL_NON_PLAYER>(offset);

		player.draw(offset);
	}

	template<map_type_e MapType> inline void entity_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		draw<ALL_NON_PLAYER>(offset, nudge);

		player.draw(offset, nudge);
	}
} // namespace necrowarp

#include <necrowarp/entity_buffer.tpp>

#include <necrowarp/entities.tpp>
#include <necrowarp/commands.tpp>
