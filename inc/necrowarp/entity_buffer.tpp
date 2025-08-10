#pragma once

#include <necrowarp/entity_buffer.hpp>

#include <optional>

#include <bleak/sparse.hpp>

#include <necrowarp/entities/evil/player.hpp>

namespace necrowarp {	
	using namespace bleak;
	
	template<map_type_e MapType> extern grid_cursor_t<globals::cell_size<grid_type_e::Game>> warp_cursor;

	extern bool draw_cursor;

	extern std::optional<offset_t> warped_from;

	extern player_t player_buffer;

	template<map_type_e MapType> extern entity_buffer_t<MapType> entity_buffer;

	template<NonPlayerEntity EntityType> extern sparse_t<EntityType> entity_buffer_storage;

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_buffer_t<MapType>::contains(offset_t position) const noexcept { return entity_buffer_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_buffer_t<MapType>::contains(offset_t position) const noexcept { return player_buffer.position == position; }

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_buffer_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) || ...);
	}

	template<map_type_e MapType> inline bool entity_buffer_t<MapType>::contains(offset_t position) const noexcept { return contains<ALL_ENTITIES>(position); }

	template<map_type_e MapType> template<distance_function_e Distance, Entity EntityType> inline bool entity_buffer_t<MapType>::nearby(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<Distance>) {
			cauto current_pos{ position + offset };

			if (contains<EntityType>(current_pos)) {
				return true;
			}
		}

		return false;
	}

	template<map_type_e MapType>
	template<distance_function_e Distance, Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_buffer_t<MapType>::nearby(offset_t position) const noexcept {
		return (nearby<Distance, EntityTypes>(position) || ...);
	}

	template<map_type_e MapType> template<distance_function_e Distance, Entity EntityType> inline std::optional<offset_t> entity_buffer_t<MapType>::nearest(offset_t position) const noexcept {
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
	template<distance_function_e Distance, Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline std::optional<offset_t> entity_buffer_t<MapType>::nearest(offset_t position) const noexcept {
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

	template<map_type_e MapType> inline std::optional<entity_e> entity_buffer_t<MapType>::at(offset_t position) const noexcept {
		for (const entity_e entity : magic_enum::enum_values<entity_e>()) {
			if (magic_enum::enum_switch([&](auto val) -> bool {
				constexpr entity_e cval{ val };

				using entity_type = typename to_entity_type<cval>::type;

				return contains<entity_type>(position);
			}, entity)) {
				return entity;
			}
		}

		return std::nullopt;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline cptr<EntityType> entity_buffer_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_buffer_storage<EntityType>[position];
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline cptr<EntityType> entity_buffer_t<MapType>::at(offset_t position) const noexcept {
		if (player_buffer.position != position) {
			return nullptr;
		}

		return &player_buffer;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline ptr<EntityType> entity_buffer_t<MapType>::at(offset_t position) noexcept {
		if (!contains<EntityType>(position)) {
			return nullptr;
		}

		return entity_buffer_storage<EntityType>[position];
	}

	template<map_type_e MapType> template<PlayerEntity EntityType> inline ptr<EntityType> entity_buffer_t<MapType>::at(offset_t position) noexcept {
		if (player_buffer.position != position) {
			return nullptr;
		}

		return &player_buffer;
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline usize entity_buffer_t<MapType>::count() const noexcept {
		return entity_buffer_storage<EntityType>.size();
	}

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_buffer_t<MapType>::count() const noexcept {
		return (count<EntityTypes>() + ...);
	}

	template<map_type_e MapType> inline usize entity_buffer_t<MapType>::count() const noexcept { return count<ALL_ENTITIES>(); }

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_buffer_t<MapType>::count(offset_t position) const noexcept {
		return (contains<EntityTypes>(position) + ...);
	}

	template<map_type_e MapType> inline usize entity_buffer_t<MapType>::count(offset_t position) const noexcept { return count<ALL_ENTITIES>(position); }
	
	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_buffer_t<MapType>::empty() const noexcept {
		return entity_buffer_storage<EntityType>.empty();
	}
	
	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_buffer_t<MapType>::empty() const noexcept {
		return (empty<EntityTypes>() && ...);
	}
	
	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_buffer_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<EntityTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline bool entity_buffer_t<MapType>::empty(offset_t position) const noexcept { return !entity_buffer_storage<EntityType>.contains(position); }

	template<map_type_e MapType> template<PlayerEntity EntityType> inline bool entity_buffer_t<MapType>::empty(offset_t position) const noexcept { return player_buffer.position != position; }

	template<map_type_e MapType> inline bool entity_buffer_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_ENTITIES>(position); }

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_buffer_t<MapType>::draw() const noexcept {
		for (crauto [position, entity] : entity_buffer_storage<EntityType>) {
			entity.draw(position);
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		for (crauto [position, entity] : entity_buffer_storage<EntityType>) {
			entity.draw(position, offset);
		}
	}

	template<map_type_e MapType> template<NonPlayerEntity EntityType> inline void entity_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		for (crauto [position, entity] : entity_buffer_storage<EntityType>) {
			entity.draw(position, offset, nudge);
		}
	}

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_buffer_t<MapType>::draw() const noexcept {
		(draw<EntityTypes>(), ...);
	}

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		(draw<EntityTypes>(offset), ...);
	}

	template<map_type_e MapType>
	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		(draw<EntityTypes>(offset, nudge), ...);
	}

	template<map_type_e MapType> inline void entity_buffer_t<MapType>::draw() const noexcept {
		draw<ALL_NON_PLAYER>();

		player_buffer.draw();
	}

	template<map_type_e MapType> inline void entity_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		draw<ALL_NON_PLAYER>(offset);

		player_buffer.draw(offset);
	}

	template<map_type_e MapType> inline void entity_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		draw<ALL_NON_PLAYER>(offset, nudge);

		player_buffer.draw(offset, nudge);
	}
} // namespace necrowarp
