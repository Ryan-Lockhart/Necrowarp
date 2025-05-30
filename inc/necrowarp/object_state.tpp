#pragma once

#include <necrowarp/object_state.hpp>

#include <necrowarp/objects.hpp>

#include <optional>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;

	template<NonNullObject ObjectType> static inline sparse_t<ObjectType> object_storage{};

	template<NonNullObject ObjectType> static inline field_t<float, globals::DistanceFunction, globals::MapSize, globals::BorderSize> object_goal_map{};

	template<NonNullObject ObjectType> inline bool object_registry_t::contains(offset_t position) const noexcept { return object_storage<ObjectType>.contains(position); }

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t::contains(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) || ...);
	}

	template<NullObject ObjectType> inline bool object_registry_t::contains(offset_t position) const noexcept { return !contains<ALL_OBJECTS>(position); }

	inline bool object_registry_t::contains(offset_t position) const noexcept { return contains<ALL_OBJECTS>(position); }

	inline object_group_e object_registry_t::at(offset_t position) const noexcept {
		object_group_e objects{ object_group_e::None };

		magic_enum::enum_for_each<object_e>([this, position, &objects](auto val) -> void {
			constexpr object_e cval{ val };

			using object_type = to_object_type<cval>::type;

			if (contains<object_type>(position)) {
				objects += to_object_group<cval>::value;
			}
		});
		
		return objects;
	}

	template<NonNullObject ObjectType> inline cptr<ObjectType> object_registry_t::at(offset_t position) const noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_storage<ObjectType>[position];
	}

	template<NonNullObject ObjectType> inline ptr<ObjectType> object_registry_t::at(offset_t position) noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_storage<ObjectType>[position];
	}

	template<NonNullObject ObjectTypes> inline usize object_registry_t::count() const noexcept {
		return object_storage<ObjectTypes>.size();
	}

	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_registry_t::count() const noexcept {
		return (count<ObjectTypes>() + ...);
	}

	inline usize object_registry_t::count() const noexcept { return count<ALL_OBJECTS>(); }
	
	template<NonNullObject ObjectType> inline bool object_registry_t::empty() const noexcept {
		return object_storage<ObjectType>.empty();
	}
	
	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t::empty() const noexcept {
		return (empty<ObjectTypes>() && ...);
	}
	
	template<Object... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_registry_t::empty(offset_t position) const noexcept {
		return (empty<ObjectTypes>(position) && ...);
	}

	template<NonNullObject ObjectType> inline bool object_registry_t::empty(offset_t position) const noexcept { return !object_storage<ObjectType>.contains(position); }

	inline bool object_registry_t::empty(offset_t position) const noexcept { return empty<ALL_OBJECTS>(position); }

	template<bool Force, NonNullObject ObjectType> inline bool object_registry_t::add(rval<ObjectType> object) noexcept {
		if constexpr (!Force) {
			if (object_registry.contains(object.position)) {
				return false;
			}
		}
		
		const offset_t position{ object.position };
		const bool inserted{ object_storage<ObjectType>.add(std::move(object)) };

		if (inserted) {
			object_goal_map<ObjectType>.add(position);
		}

		return inserted;
	}

	template<NonNullObject ObjectType> inline bool object_registry_t::remove(offset_t position) noexcept {
		if (object_registry.empty(position)) {
			return false;
		}
		
		if (!object_storage<ObjectType>.remove(position)) {
			return false;
		}

		object_goal_map<ObjectType>.remove(position);

		return true;
	}

	template<NonNullObject ObjectType> inline void object_registry_t::clear() noexcept {
		object_storage<ObjectType>.clear();
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::clear() noexcept {
		(clear<ObjectTypes>(), ...);
	}

	inline void object_registry_t::clear() noexcept {
		clear<ALL_OBJECTS>();
	}

	template<NonNullObject ObjectType> inline void object_registry_t::reset() noexcept {
		object_storage<ObjectType>.clear();
		reset_goal_map<ObjectType>();
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::reset() noexcept {
		(clear<ObjectTypes>(), ...);
		(reset_goal_map<ObjectTypes>(), ...);
	}

	inline void object_registry_t::reset() noexcept {
		clear();

		reset_goal_map();
	}

	template<NonNullObject ObjectType> inline bool object_registry_t::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, object_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			object_registry.add(ObjectType{ maybe_position.value() });
		}

		return true;
	}

	template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, object_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			object_registry.add(ObjectType{ maybe_position.value(), args... });
		}

		return true;
	}

	template<NonNullObject ObjectType> inline bool object_registry_t::spawn(usize count, u32 minimum_distance) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<ObjectType>.template find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, object_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			object_registry.add(ObjectType{ maybe_position.value() });

			object_goal_map<ObjectType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, object_registry);
		}

		return true;
	}

	template<NonNullObject ObjectType, typename... Args> inline bool object_registry_t::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		object_goal_map<ObjectType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, object_registry);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ object_goal_map<ObjectType>.template find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, object_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			object_registry.add(ObjectType{ maybe_position.value(), args... });

			object_goal_map<ObjectType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, object_registry);
		}

		return true;
	}

	template<NonNullObject ObjectType> inline bool object_registry_t::update(offset_t current, offset_t target) noexcept {
		if (object_registry.empty(current) || !game_map.within<zone_region_t::Interior>(current) || object_registry.contains(target) || !game_map.within<zone_region_t::Interior>(target)) {
			return false;
		}
		
		if (!object_storage<ObjectType>.move(current, target)) {
			return false;
		}

		object_goal_map<ObjectType>.update(current, target);

		return true;
	}

	template<NonNullObject ObjectType> inline void object_registry_t::recalculate_goal_map() noexcept {
		object_goal_map<ObjectType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, object_registry);
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::recalculate_goal_map() noexcept {
		(recalculate_goal_map<ObjectTypes>(), ...);
	}

	inline void object_registry_t::recalculate_goal_map() noexcept {
		recalculate_goal_map<ALL_OBJECTS>();
	}

	template<NonNullObject ObjectType> inline void object_registry_t::reset_goal_map() noexcept {
		object_goal_map<ObjectType>.reset();
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::reset_goal_map() noexcept {
		(reset_goal_map<ObjectTypes>(), ...);
	}

	inline void object_registry_t::reset_goal_map() noexcept {
		reset_goal_map<ALL_OBJECTS>();
	}

	template<NonNullObject ObjectType> inline void object_registry_t::draw() const noexcept {
		for (crauto object : object_storage<ObjectType>) {
			object.draw();
		}
	}

	template<NonNullObject ObjectType> inline void object_registry_t::draw(cref<camera_t> camera) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto object : object_storage<ObjectType>) {
			if (!viewport.within(object.position)) {
				continue;
			}

			object.draw(camera);
		}
	}

	template<NonNullObject ObjectType> inline void object_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		cauto viewport{ camera.get_viewport() }; 

		for (crauto object : object_storage<ObjectType>) {
			if (!viewport.within(object.position)) {
				continue;
			}

			object.draw(camera, offset);
		}
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::draw() const noexcept {
		(draw<ObjectTypes>(), ...);
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::draw(cref<camera_t> camera) const noexcept {
		(draw<ObjectTypes>(camera), ...);
	}

	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		(draw<ObjectTypes>(camera, offset), ...);
	}

	inline void object_registry_t::draw() const noexcept {
		draw<ALL_OBJECTS>();
	}

	inline void object_registry_t::draw(cref<camera_t> camera) const noexcept {
		draw<ALL_OBJECTS>(camera);
	}

	inline void object_registry_t::draw(cref<camera_t> camera, offset_t offset) const noexcept {
		draw<ALL_OBJECTS>(camera, offset);
	}
} // namespace necrowarp

#include <necrowarp/objects.tpp>
