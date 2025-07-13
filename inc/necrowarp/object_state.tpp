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
		}

		return inserted;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spill(offset_t position, rval<ObjectType> object) noexcept {
		if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position] != cell_e::Open) {
			return false;
		}

		if (!contains<ObjectType>(position)) {
			const bool inserted{ object_registry_storage<ObjectType>.add(position, std::move(object)) };

			if (inserted) {
				object_goal_map<MapType, ObjectType>.add(position);
			}

			return inserted;
		}

		std::queue<creeper_t<offset_t::product_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(position, 0);
		visited.insert(position);

		while (!frontier.empty()) {
			const creeper_t<offset_t::product_t> current{ frontier.front() };
			frontier.pop();

			visited.insert(current.position);

			if (!contains<ObjectType>(current.position)) {
				const bool inserted{ object_registry_storage<ObjectType>.add(current.position, std::move(object)) };

				if (inserted) {
					object_goal_map<MapType, ObjectType>.add(current.position);
				}

				return inserted;
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

		if (!contains<ObjectType>(position)) {
			const bool inserted{ object_registry_storage<ObjectType>.add(position, std::move(object)) };

			if (inserted) {
				object_goal_map<MapType, ObjectType>.add(position);
			}

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

			if (!contains<ObjectType>(current.position)) {
				const bool inserted{ object_registry_storage<ObjectType>.add(current.position, std::move(object)) };

				if (inserted) {
					object_goal_map<MapType, ObjectType>.add(current.position);
				}

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
		if (empty(position)) {
			return false;
		}
		
		if (!object_registry_storage<ObjectType>.remove(position)) {
			return false;
		}

		object_goal_map<MapType, ObjectType>.remove(position);

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::clear() noexcept {
		object_registry_storage<ObjectType>.clear();
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
		object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);

		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{});

			object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t<MapType>::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.dependent find_random<region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(maybe_position.value(), ObjectType{ args... });

			object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
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

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		object_goal_map<MapType, ObjectType>.dependent recalculate<region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		(recalculate_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept { recalculate_goal_map<ALL_OBJECTS>(); }

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::reset_goal_map() noexcept {
		object_goal_map<MapType, ObjectType>.reset();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::reset_goal_map() noexcept {
		(reset_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::reset_goal_map() noexcept { reset_goal_map<ALL_OBJECTS>(); }

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
