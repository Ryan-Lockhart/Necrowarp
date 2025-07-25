#pragma once

#include <necrowarp/object_buffer.hpp>

#include <bleak/sparse.hpp>

namespace necrowarp {	
	using namespace bleak;

	template<map_type_e MapType> extern object_buffer_t<MapType> object_buffer;

	template<NonNullObject ObjectType> extern sparse_t<ObjectType> object_buffer_storage;

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_buffer_t<MapType>::contains(offset_t position) const noexcept { return object_buffer_storage<ObjectType>.contains(position); }

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_buffer_t<MapType>::contains(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) || ...);
	}

	template<map_type_e MapType> inline bool object_buffer_t<MapType>::contains(offset_t position) const noexcept { return contains<ALL_OBJECTS>(position); }

	template<map_type_e MapType> inline object_group_e object_buffer_t<MapType>::at(offset_t position) const noexcept {
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

	template<map_type_e MapType> template<NonNullObject ObjectType> inline cptr<ObjectType> object_buffer_t<MapType>::at(offset_t position) const noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_buffer_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline ptr<ObjectType> object_buffer_t<MapType>::at(offset_t position) noexcept {
		if (!contains<ObjectType>(position)) {
			return nullptr;
		}

		return object_buffer_storage<ObjectType>[position];
	}

	template<map_type_e MapType> template<NonNullObject ObjectTypes> inline usize object_buffer_t<MapType>::count() const noexcept {
		return object_buffer_storage<ObjectTypes>.size();
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_buffer_t<MapType>::count() const noexcept {
		return (count<ObjectTypes>() + ...);
	}

	template<map_type_e MapType>
	inline usize object_buffer_t<MapType>::count() const noexcept { return count<ALL_OBJECTS>(); }

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline usize object_buffer_t<MapType>::count(offset_t position) const noexcept {
		return (contains<ObjectTypes>(position) + ...);
	}

	template<map_type_e MapType> inline usize object_buffer_t<MapType>::count(offset_t position) const noexcept { return count<ALL_OBJECTS>(position); }
	
	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_buffer_t<MapType>::empty() const noexcept {
		return object_buffer_storage<ObjectType>.empty();
	}
	
	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_buffer_t<MapType>::empty() const noexcept {
		return (empty<ObjectTypes>() && ...);
	}
	
	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline bool object_buffer_t<MapType>::empty(offset_t position) const noexcept {
		return (empty<ObjectTypes>(position) && ...);
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline bool object_buffer_t<MapType>::empty(offset_t position) const noexcept { return !object_buffer_storage<ObjectType>.contains(position); }

	template<map_type_e MapType> inline bool object_buffer_t<MapType>::empty(offset_t position) const noexcept { return empty<ALL_OBJECTS>(position); }

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_buffer_t<MapType>::draw() const noexcept {
		for (crauto [position, object] : object_buffer_storage<ObjectType>) {
			object.draw();
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		for (crauto [position, object] : object_buffer_storage<ObjectType>) {
			object.draw(position, offset);
		}
	}

	template<map_type_e MapType> template<NonNullObject ObjectType> inline void object_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		for (crauto [position, object] : object_buffer_storage<ObjectType>) {
			object.draw(position, offset, nudge);
		}
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_buffer_t<MapType>::draw() const noexcept {
		(draw<ObjectTypes>(), ...);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		(draw<ObjectTypes>(offset), ...);
	}

	template<map_type_e MapType>
	template<NonNullObject... ObjectTypes>
		requires is_plurary<ObjectTypes...>::value
	inline void object_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		(draw<ObjectTypes>(offset, nudge), ...);
	}

	template<map_type_e MapType> inline void object_buffer_t<MapType>::draw() const noexcept {
		draw<ALL_OBJECTS>();
	}

	template<map_type_e MapType> inline void object_buffer_t<MapType>::draw(offset_t offset) const noexcept {
		draw<ALL_OBJECTS>(offset);
	}

	template<map_type_e MapType> inline void object_buffer_t<MapType>::draw(offset_t offset, offset_t nudge) const noexcept {
		draw<ALL_OBJECTS>(offset, nudge);
	}
} // namespace necrowarp

#include <necrowarp/objects.tpp>
