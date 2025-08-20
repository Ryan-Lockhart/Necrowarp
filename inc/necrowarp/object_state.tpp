#pragma once

#include <necrowarp/object_state.hpp>

#include <necrowarp/objects.hpp>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;

	template<typename T> struct is_object_registry {
		static constexpr bool value{ false };
	};

	template<typename T> constexpr bool is_object_registry_v = is_object_registry<T>::value;

	template<typename T> concept ObjectRegistry = is_object_registry<T>::value;

	template<map_type_e MapType> struct is_object_registry<object_registry_t<MapType>> {
		static constexpr bool value{ true };
	};

	template<map_type_e MapType> struct is_object_registry<object_buffer_t<MapType>> {
		static constexpr bool value{ true };
	};

	template<map_type_e MapType> static inline object_registry_t<MapType> object_registry{};

	template<map_type_e MapType> static inline object_buffer_t<MapType> object_buffer{};

	template<NonNullObject ObjectType> static inline sparse_t<ObjectType> object_registry_storage{};

	template<NonNullObject ObjectType> static inline sparse_t<ObjectType> object_buffer_storage{};
	
	template<map_type_e MapType> static inline field_t<f32, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> departure_goal_map{};
	
	static inline bool departure_goals_dirty{ false };

	template<map_type_e MapType, NonNullObject ObjectType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> object_goal_map{};

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::store() const noexcept {
		object_buffer_storage<ObjectType> = object_registry_storage<ObjectType>;
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::store() const noexcept {
		(store<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept { return object_registry_storage<ObjectType>.contains(position); }

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) || ...);
	}

	template<map_type_e MapType> inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept { return contains<ALL_OBJECTS>(position); }

	template<map_type_e MapType> template<distance_function_e Distance, NonNullObject ObjectType> inline bool object_registry_t<MapType>::nearby(offset_t position) const noexcept {
		for (cauto offset : neighbourhood_offsets<Distance>) {
			cauto current_pos{ position + offset };

			if (contains<ObjectType>(current_pos)) {
				return true;
			}
		}

		return false;
	}

	template<map_type_e MapType>
	template<distance_function_e Distance, NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::nearby(offset_t position) const noexcept {
		return (nearby<Distance, ObjectTypes>(position) || ...);
	}

	template<map_type_e MapType> template<distance_function_e Distance, NonNullObject ObjectType> inline std::optional<offset_t> object_registry_t<MapType>::nearest(offset_t position) const noexcept {
		if (empty<ObjectType>()) {
			return std::nullopt;
		}

		if (contains<ObjectType>(position)) {
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

			if (contains<ObjectType>(current.position)) {
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
	template<distance_function_e Distance, NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline std::optional<offset_t> object_registry_t<MapType>::nearest(offset_t position) const noexcept {
		if (empty<ObjectTypes...>()) {
			return std::nullopt;
		}

		if (contains<ObjectTypes...>(position)) {
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

			if ((contains<ObjectTypes>(current.position) || ...)) {
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

	template<map_type_e MapType> inline object_group_e object_registry_t<MapType>::at(offset_t position) const noexcept {
		object_group_e objects{ object_group_e::None };

		magic_enum::enum_for_each<object_e>([&](auto val) -> void {
			constexpr object_e cval{ val };

			using object_type = to_object_type<cval>::type;

			if constexpr (!is_null_object<object_type>::value) {
				if (contains<object_type>(position)) {
					objects += to_object_group<cval>::value;
				}
			}
		});
		
		return objects;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline cptr<ObjectType> object_registry_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_registry_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline ptr<ObjectType> object_registry_t<MapType>::at(offset_t position) noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_registry_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectTypes> inline usize object_registry_t<MapType>::count() const noexcept {
		return object_registry_storage<ObjectTypes>.size();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_registry_t<MapType>::count() const noexcept {
		return (count<ObjectTypes>() + ...);
	}

	template<map_type_e MapType>
	inline usize object_registry_t<MapType>::count() const noexcept { return count<ALL_OBJECTS>(); }

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_registry_t<MapType>::count(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) + ...);
	}

	template<map_type_e MapType> inline usize object_registry_t<MapType>::count(offset_t position) const noexcept { return count<ALL_OBJECTS>(position); }
	
	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::empty() const noexcept {
		return object_registry_storage<ObjectType>.empty();
	}
	
	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::empty() const noexcept {
		return (empty<ObjectTypes>() && ...);
	}

	template<map_type_e MapType> inline bool object_registry_t<MapType>::empty() const noexcept { return empty<ALL_OBJECTS>(); }
	
	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<ObjectTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept { return !object_registry_storage<ObjectType>.contains(position); }

	template<map_type_e MapType> inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_OBJECTS>(position); }

	template<map_type_e MapType> template<NonNullObject ObjectType, bool Force> inline bool object_registry_t<MapType>::add(offset_t position) noexcept {
		if constexpr (!Force) {
			if (contains(position)) {
				return false;
			}
		}

		const bool inserted{ object_registry_storage<ObjectType>.add(position, ObjectType{}) };

		if (inserted) {
			object_goal_map<MapType, ObjectType>.add(position);

			if constexpr (std::is_same<ObjectType, ladder_t>::value) {
				departure_goals_dirty = true;
			}
		}

		return inserted;
	}

	template<map_type_e MapType> template<bool Force, NonNullObject ObjectType> inline bool object_registry_t<MapType>::add(offset_t position, rval<ObjectType> object) noexcept {
		if constexpr (!Force) {
			if (contains(position)) {
				return false;
			}
		}

		const bool inserted{ object_registry_storage<ObjectType>.add(position, std::move(object)) };

		if (inserted) {
			object_goal_map<MapType, ObjectType>.add(position);

			if constexpr (std::is_same<ObjectType, ladder_t>::value) {
				departure_goals_dirty = true;
			}
		}

		return inserted;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spill(offset_t position, rval<ObjectType> object) noexcept {
		if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open) {
			return false;
		}

		if (empty<ObjectType>(position)) {
			return add<true>(position, std::move(object));
		}

		std::queue<creeper_t<offset_t::product_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			const creeper_t<offset_t::product_t> current{ frontier.front() };
			frontier.pop();

			visited.insert(current.position);

			if (empty<ObjectType>(current.position)) {
				return add<true>(current.position, std::move(object));
			}

			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
					continue;
				}

				frontier.emplace(offset_position, offset_t::product_t{ current.distance + 1 });
			}
		}

		return false;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spill(offset_t position, rval<ObjectType> object, usize amount) noexcept {
		if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open || amount <= 0) {
			return false;
		}

		if (empty<ObjectType>(position)) {
			const bool inserted{ add<true>(position, std::move(object)) };

			if (amount <= 1) {
				return inserted;
			} else {
				--amount;
			}
		}

		std::queue<creeper_t<offset_t::product_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			const creeper_t<offset_t::product_t> current{ frontier.front() };
			frontier.pop();

			visited.insert(current.position);

			if (empty<ObjectType>(current.position)) {
				const bool inserted{ add<true>(position, std::move(object)) };

				if (amount <= 1) {
					return inserted;
				} else {
					--amount;
				}
			}

			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !game_map<MapType>.dependent within<region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
					continue;
				}

				frontier.emplace(offset_position, offset_t::product_t{ current.distance + 1 });					
			}
		}

		return false;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::remove(offset_t position) noexcept {
		if (empty<ObjectType>(position)) {
			return false;
		}
		
		if (!object_registry_storage<ObjectType>.remove(position)) {
			return false;
		}

		object_goal_map<MapType, ObjectType>.remove(position);

		if constexpr (std::is_same<ObjectType, ladder_t>::value) {
			departure_goals_dirty = true;
		}

		return true;
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::remove(offset_t position) noexcept {
		if (empty<ObjectTypes...>(position)) {
			return;
		}

		(remove<ObjectTypes>(position), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::remove(offset_t position) noexcept {
		if (empty(position)) {
			return;
		}

		remove<ALL_OBJECTS>(position);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::clear() noexcept {
		object_registry_storage<ObjectType>.clear();

		if constexpr (std::is_same<ObjectType, ladder_t>::value) {
			departure_goals_dirty = true;
		}

		reset_goal_map<ObjectType>();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::clear() noexcept {
		(clear<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::clear() noexcept {
		clear<ALL_OBJECTS>();
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{});
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t<MapType>::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.dependent find_random<region_e::Interior>(random_engine, cell_e::Open, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{ args... });
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spawn(usize count, u32 minimum_distance) noexcept {
		recalculate_goal_map<ObjectType>();

		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{});

			recalculate_goal_map<ObjectType>();
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t<MapType>::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		recalculate_goal_map<ObjectType>();
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{ args... });

			recalculate_goal_map<ObjectType>();
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.dependent within<region_e::Interior>(current) || contains(target) || !game_map<MapType>.dependent within<region_e::Interior>(target)) {
			return false;
		}
		
		if (!object_registry_storage<ObjectType>.move(current, target)) {
			return false;
		}

		object_goal_map<MapType, ObjectType>.update(current, target);

		return true;
	}

	template<map_type_e MapType> template<AnimatedObject ObjectType> inline void object_registry_t<MapType>::advance() noexcept {
		for (crauto [_, object] : object_registry_storage<ObjectType>) {
			object.idle_animation.advance();
		}
	}

	template<map_type_e MapType> 
	template<AnimatedObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::advance() noexcept {
		(advance<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::advance() noexcept { advance<ALL_ANIMATED_OBJECTS>(); }

	template<map_type_e MapType> template<AnimatedObject ObjectType> inline void object_registry_t<MapType>::retreat() noexcept {
		for (crauto [_, object] : object_registry_storage<ObjectType>) {
			object.idle_animation.retreat();
		}
	}

	template<map_type_e MapType> 
	template<AnimatedObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::retreat() noexcept {
		(retreat<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::retreat() noexcept { retreat<ALL_ANIMATED_OBJECTS>(); }

	template<map_type_e MapType> inline void object_registry_t<MapType>::rescan_departure_goals() noexcept {
		departure_goals_dirty = false;

		departure_goal_map<MapType>.reset();

		if (object_registry<MapType>.dependent empty<ladder_t>()) {
			return;
		}

		for (cauto position : game_map<MapType>.interior_offsets) {
			if (game_map<MapType>[position].solid || !object_registry<MapType>.dependent empty<ladder_t>(position)) {
				continue;
			}

			if (cptr<ladder_t> maybe_ladder{ object_registry<MapType>.dependent at<ladder_t>(position) }; maybe_ladder == nullptr || maybe_ladder->is_down_ladder() || maybe_ladder->has_shackle()) {
				continue;
			}

			departure_goal_map<MapType>.add(position);
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry_storage<ObjectType>);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		(recalculate_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		recalculate_departure_goal_map();

		recalculate_goal_map<ALL_OBJECTS>();
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::recalculate_departure_goal_map() noexcept {
		if (departure_goals_dirty) {
			rescan_departure_goals();
		}

		departure_goal_map<MapType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::reset_goal_map() noexcept {
		object_goal_map<MapType, ObjectType>.reset();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::reset_goal_map() noexcept {
		(reset_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::reset_goal_map() noexcept {
		reset_departure_goal_map();

		reset_goal_map<ALL_OBJECTS>();
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::reset_departure_goal_map() noexcept {
		departure_goal_map<MapType>.reset();
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw() const noexcept {
		for (crauto [position, object] : object_registry_storage<ObjectType>) {
			object.draw(position);
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw(offset_t offset) const noexcept {
		for (crauto [position, object] : object_registry_storage<ObjectType>) {
			object.draw(position, offset);
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		for (crauto [position, object] : object_registry_storage<ObjectType>) {
			object.draw(position, offset, nudge);
		}
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::draw() const noexcept {
		(draw<ObjectTypes>(), ...);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::draw(offset_t offset) const noexcept {
		(draw<ObjectTypes>(offset), ...);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		(draw<ObjectTypes>(offset, nudge), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw() const noexcept { draw<ALL_OBJECTS>(); }

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw(offset_t offset) const noexcept { draw<ALL_OBJECTS>(offset); }

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept { draw<ALL_OBJECTS>(offset, nudge); }
} // namespace necrowarp

#include <necrowarp/object_buffer.tpp>

#include <necrowarp/objects.tpp>
