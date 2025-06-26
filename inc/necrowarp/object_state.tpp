#pragma once

#include <necrowarp/object_state.hpp>

#include <necrowarp/objects.hpp>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;

	template<map_type_e MapType> static inline object_registry_t<MapType> object_registry{};

	template<NonNullObject ObjectType> static inline sparse_t<ObjectType> object_storage{};

	template<map_type_e MapType, NonNullObject ObjectType> static inline field_t<float, globals::DistanceFunction, globals::MapSize<MapType>, globals::BorderSize<MapType>> object_goal_map{};

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept { return object_storage<ObjectType>.contains(position); }

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) || ...);
	}

	template<map_type_e MapType> template<NullObject ObjectType> inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept { return !contains<ALL_OBJECTS>(position); }

	template<map_type_e MapType> inline bool object_registry_t<MapType>::contains(offset_t position) const noexcept { return contains<ALL_OBJECTS>(position); }

	template<map_type_e MapType> inline object_group_e object_registry_t<MapType>::at(offset_t position) const noexcept {
		object_group_e objects{ object_group_e::None };

		magic_enum::enum_for_each<object_e>([&](auto val) -> void {
			constexpr object_e cval{ val };

			using object_type = to_object_type<cval>::type;

			if (contains<object_type>(position)) {
				objects += to_object_group<cval>::value;
			}
		});
		
		return objects;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline cptr<ObjectType> object_registry_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline ptr<ObjectType> object_registry_t<MapType>::at(offset_t position) noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectTypes> inline usize object_registry_t<MapType>::count() const noexcept {
		return object_storage<ObjectTypes>.size();
	}

	template<map_type_e MapType>
	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_registry_t<MapType>::count() const noexcept {
		return (count<ObjectTypes>() + ...);
	}

	template<map_type_e MapType>
	inline usize object_registry_t<MapType>::count() const noexcept { return count<ALL_OBJECTS>(); }
	
	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::empty() const noexcept {
		return object_storage<ObjectType>.empty();
	}
	
	template<map_type_e MapType>
	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::empty() const noexcept {
		return (empty<ObjectTypes>() && ...);
	}
	
	template<map_type_e MapType>
	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<ObjectTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept { return !object_storage<ObjectType>.contains(position); }

	template<map_type_e MapType> inline bool object_registry_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_OBJECTS>(position); }

	template<map_type_e MapType> template<bool Force, NonNullObject ObjectType> inline bool object_registry_t<MapType>::add(rval<ObjectType> object) noexcept {
		if constexpr (!Force) {
			if (contains(object.position)) {
				return false;
			}
		}
		
		const offset_t position{ object.position };
		const bool inserted{ object_storage<ObjectType>.add(std::move(object)) };

		if (inserted) {
			object_goal_map<MapType, ObjectType>.add(position);
		}

		return inserted;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spill(rval<ObjectType> object) noexcept {
		if (!game_map<MapType>.template within<zone_region_e::Interior>(object.position) || game_map<MapType>[object.position] != cell_e::Open) {
			return false;
		}

		if (!contains<ObjectType>(object.position)) {
			const bool inserted{ object_storage<ObjectType>.add(std::move(object)) };

			if (inserted) {
				object_goal_map<MapType, ObjectType>.add(object.position);
			}

			return inserted;
		}

		std::queue<creeper_t<offset_t::product_t>> frontier{};
		std::unordered_set<offset_t, offset_t::std_hasher> visited{};

		frontier.emplace(object.position, 0);
		visited.insert(object.position);

		while (!frontier.empty()) {
			const creeper_t<offset_t::product_t> current{ frontier.front() };
			frontier.pop();

			visited.insert(current.position);

			if (!contains<ObjectType>(current.position)) {
				object.position = current.position;
				const bool inserted{ object_storage<ObjectType>.add(std::move(object)) };

				if (inserted) {
					object_goal_map<MapType, ObjectType>.add(current.position);
				}

				return inserted;
			}

			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t offset_position{ current.position + offset };

				if (!visited.insert(offset_position).second || !game_map<MapType>.template within<zone_region_e::Interior>(offset_position) || game_map<MapType>[offset_position] != cell_e::Open) {
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
		
		if (!object_storage<ObjectType>.remove(position)) {
			return false;
		}

		object_goal_map<MapType, ObjectType>.remove(position);

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::clear() noexcept {
		object_storage<ObjectType>.clear();
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

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::reset() noexcept {
		object_storage<ObjectType>.clear();
		reset_goal_map<ObjectType>();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::reset() noexcept {
		(clear<ObjectTypes>(), ...);
		(reset_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::reset() noexcept {
		clear();

		reset_goal_map();
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(ObjectType{ maybe_position.value() });
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t<MapType>::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map<MapType>.template find_random<zone_region_e::Interior>(random_engine, cell_e::Open, object_registry<MapType>) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(ObjectType{ maybe_position.value(), args... });
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::spawn(usize count, u32 minimum_distance) noexcept {
		object_goal_map<MapType, ObjectType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);

		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.template find_random<zone_region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(ObjectType{ maybe_position.value() });

			object_goal_map<MapType, ObjectType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t<MapType>::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		object_goal_map<MapType, ObjectType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<MapType, ObjectType>.template find_random<zone_region_e::Interior>(game_map<MapType>, random_engine, cell_e::Open, object_registry<MapType>, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			add(ObjectType{ maybe_position.value(), args... });

			object_goal_map<MapType, ObjectType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
		}

		return true;
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_registry_t<MapType>::update(offset_t current, offset_t target) noexcept {
		if (empty(current) || !game_map<MapType>.template within<zone_region_e::Interior>(current) || contains(target) || !game_map<MapType>.template within<zone_region_e::Interior>(target)) {
			return false;
		}
		
		if (!object_storage<ObjectType>.move(current, target)) {
			return false;
		}

		object_goal_map<MapType, ObjectType>.update(current, target);

		return true;
	}

	template<map_type_e MapType> template<AnimatedObject ObjectType> inline void object_registry_t<MapType>::advance() noexcept {
		for (crauto object : object_storage<ObjectType>) { object.idle_animation.advance(); }
	}

	template<map_type_e MapType> 
	template<AnimatedObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::advance() noexcept {
		(advance<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::advance() noexcept { advance<ALL_ANIMATED_OBJECTS>(); }

	template<map_type_e MapType> template<AnimatedObject ObjectType> inline void object_registry_t<MapType>::retreat() noexcept {
		for (crauto object : object_storage<ObjectType>) { object.idle_animation.retreat(); }
	}

	template<map_type_e MapType> 
	template<AnimatedObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::retreat() noexcept {
		(retreat<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::retreat() noexcept { retreat<ALL_ANIMATED_OBJECTS>(); }

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		object_goal_map<MapType, ObjectType>.template recalculate<zone_region_e::Interior>(game_map<MapType>, cell_e::Open, object_registry<MapType>);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		(recalculate_goal_map<ObjectTypes>(), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::recalculate_goal_map() noexcept {
		recalculate_goal_map<ALL_OBJECTS>();
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
		reset_goal_map<ALL_OBJECTS>();
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw() const noexcept {
		for (crauto object : object_storage<ObjectType>) {
			object.draw();
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto object : object_storage<ObjectType>) {
			if (!viewport.within(object.position)) {
				continue;
			}

			object.draw(camera);
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto object : object_storage<ObjectType>) {
			if (!viewport.within(object.position)) {
				continue;
			}

			object.draw(camera, offset);
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
	inline void object_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		(draw<ObjectTypes>(camera), ...);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		(draw<ObjectTypes>(camera, offset), ...);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw() const noexcept {
		draw<ALL_OBJECTS>();
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw(cref<camera_t> camera) const noexcept {
		draw<ALL_OBJECTS>(camera);
	}

	template<map_type_e MapType> inline void object_registry_t<MapType>::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		draw<ALL_OBJECTS>(camera, offset);
	}
} // namespace necrowarp

#include <necrowarp/objects.tpp>
