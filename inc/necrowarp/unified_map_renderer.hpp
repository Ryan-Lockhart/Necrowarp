#pragma once

#include <bleak.hpp>

#include <necrowarp/animation.hpp>
#include <necrowarp/cell.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<map_type_e MapType> struct unified_map_renderer_s {
	  private:
		template<EntityRegistry Entities> static inline constexpr void draw_entity(cref<Entities> entities, offset_t position) noexcept {
			if (entities.empty(position)) {
				return;
			}

			const usize entity_index{ static_cast<usize>(entity_slideshow.current_entity()) % entities.count(position) };

			const entity_e entity_enum{ at(entities.at(position), entity_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr entity_e cval{ val };

				using entity_type = to_entity_type<cval>::type;

				if constexpr (!is_null_entity<entity_type>::value) {
					cptr<entity_type> maybe_entity{ entities.dependent at<entity_type>(position) };

					if (maybe_entity == nullptr) {
						return;
					}

					if constexpr (is_player<entity_type>::value) {
						maybe_entity->draw(offset_t::Zero, globals::SparseTileNudge);
					} else {
						maybe_entity->draw(position, offset_t::Zero, globals::SparseTileNudge);
					}
				}
			}, entity_enum);
		}

		template<ObjectRegistry Objects> static inline constexpr void draw_object(cref<Objects> objects, offset_t position) noexcept {
			if (objects.empty(position)) {
				return;
			}

			const usize object_index{ static_cast<usize>(object_slideshow.current_object()) % objects.count(position) };

			const object_e object_enum{ at(objects.at(position), object_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr object_e cval{ val };

				using object_type = to_object_type<cval>::type;

				if constexpr (!is_null_object<object_type>::value) {
					cptr<object_type> maybe_object{ objects.dependent at<object_type>(position) };

					if (maybe_object == nullptr) {
						return;
					}

					maybe_object->draw(position, offset_t::Zero, globals::SparseTileNudge);
				}
			}, object_enum);
		}

		template<EntityRegistry Entities> static inline constexpr void draw_entity(cref<Entities> entities, offset_t position, offset_t offset) noexcept {
			if (entities.empty(position)) {
				return;
			}

			const usize entity_index{ static_cast<usize>(entity_slideshow.current_entity()) % entities.count(position) };

			const entity_e entity_enum{ at(entities.at(position), entity_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr entity_e cval{ val };

				using entity_type = to_entity_type<cval>::type;

				if constexpr (!is_null_entity<entity_type>::value) {
					cptr<entity_type> maybe_entity{ entities.dependent at<entity_type>(position) };

					if (maybe_entity == nullptr) {
						return;
					}

					if constexpr (is_player<entity_type>::value) {
						maybe_entity->draw(offset, globals::SparseTileNudge);
					} else {
						maybe_entity->draw(position, offset, globals::SparseTileNudge);
					}
				}
			}, entity_enum);
		}

		template<ObjectRegistry Objects> static inline constexpr void draw_object(cref<Objects> objects, offset_t position, offset_t offset) noexcept {
			if (objects.empty(position)) {
				return;
			}

			const usize object_index{ static_cast<usize>(object_slideshow.current_object()) % objects.count(position) };

			const object_e object_enum{ at(objects.at(position), object_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr object_e cval{ val };

				using object_type = to_object_type<cval>::type;

				if constexpr (!is_null_object<object_type>::value) {
					cptr<object_type> maybe_object{ objects.dependent at<object_type>(position) };

					if (maybe_object == nullptr) {
						return;
					}

					maybe_object->draw(position, offset, globals::SparseTileNudge);
				}
			}, object_enum);
		}

		template<EntityRegistry Entities> static inline constexpr void draw_entity(cref<Entities> entities, offset_t position, offset_t offset, offset_t nudge) noexcept {
			if (entities.empty(position)) {
				return;
			}

			const usize entity_index{ static_cast<usize>(entity_slideshow.current_entity()) % entities.count(position) };

			const entity_e entity_enum{ at(entities.at(position), entity_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr entity_e cval{ val };

				using entity_type = to_entity_type<cval>::type;

				if constexpr (!is_null_entity<entity_type>::value) {
					cptr<entity_type> maybe_entity{ entities.dependent at<entity_type>(position) };

					if (maybe_entity == nullptr) {
						return;
					}

					if constexpr (is_player<entity_type>::value) {
						maybe_entity->draw(offset, nudge + globals::SparseTileNudge);
					} else {
						maybe_entity->draw(position, offset, nudge + globals::SparseTileNudge);
					}
				}
			}, entity_enum);
		}

		template<ObjectRegistry Objects> static inline constexpr void draw_object(cref<Objects> objects, offset_t position, offset_t offset, offset_t nudge) noexcept {
			if (objects.empty(position)) {
				return;
			}

			const usize object_index{ static_cast<usize>(object_slideshow.current_object()) % objects.count(position) };

			const object_e object_enum{ at(objects.at(position), object_index) };

			magic_enum::enum_switch([&](auto val) -> void {
				constexpr object_e cval{ val };

				using object_type = to_object_type<cval>::type;

				if constexpr (!is_null_object<object_type>::value) {
					cptr<object_type> maybe_object{ objects.dependent at<object_type>(position) };

					if (maybe_object == nullptr) {
						return;
					}

					maybe_object->draw(position, offset, nudge + globals::SparseTileNudge);
				}
			}, object_enum);
		}

		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, offset_t position) noexcept {
			const map_cell_t map_cell{ game_map<MapType>[position] };
			const fluid_cell_t fluid_cell{ fluid_map<MapType>[position] };

			if (!map_cell.explored) {
				return;
			}

			fluid_cell.draw_pool(atlas, fluid_map<MapType>, position, offset_t::Zero, globals::FluidTileNudge);

			map_cell.draw_wall(atlas, position, offset_t::Zero, globals::CavernTileNudge);

			map_cell.draw_patch(atlas, game_map<MapType>, position, offset_t::Zero, globals::CavernTileNudge);

			if (!map_cell.seen) {
				return;
			}

			draw_object(objects, position);

			draw_entity(entities, position);
		}

		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, offset_t position, offset_t offset) noexcept {
			const map_cell_t map_cell{ game_map<MapType>[position] };
			const fluid_cell_t fluid_cell{ fluid_map<MapType>[position] };

			if (!map_cell.explored) {
				return;
			}

			fluid_cell.draw_pool(atlas, fluid_map<MapType>, position, offset, globals::FluidTileNudge);

			map_cell.draw_wall(atlas, position, offset, globals::CavernTileNudge);

			map_cell.draw_patch(atlas, game_map<MapType>, position, offset, globals::CavernTileNudge);

			if (!map_cell.seen) {
				return;
			}

			draw_object(objects, position, offset);

			draw_entity(entities, position, offset);
		}

		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, offset_t position, offset_t offset, offset_t nudge) noexcept {
			const map_cell_t map_cell{ game_map<MapType>[position] };
			const fluid_cell_t fluid_cell{ fluid_map<MapType>[position] };

			if (!map_cell.explored) {
				return;
			}

			fluid_cell.draw_pool(atlas, fluid_map<MapType>, position, offset, nudge + globals::FluidTileNudge);

			map_cell.draw_wall(atlas, position, offset, nudge + globals::CavernTileNudge);

			map_cell.draw_patch(atlas, game_map<MapType>, position, offset, nudge + globals::CavernTileNudge);

			if (!map_cell.seen) {
				return;
			}

			draw_object(objects, position, offset, nudge);

			draw_entity(entities, position, offset, nudge);
		}

	  public:
		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, cref<camera_t> camera) {
			const offset_t origin{ camera.get_position() };
			const extent_t camera_extent{ camera.get_extent() };

			if (camera_extent.w == 0 || camera_extent.h == 0 || origin.x > game_map<MapType>.zone_extent.x || origin.y > game_map<MapType>.zone_extent.y) {
				return;
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y - 1) }; y < min(game_map<MapType>.zone_size.h, camera_extent.h + 1); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x - 1) }; x < min(game_map<MapType>.zone_size.w, camera_extent.w + 1); ++x) {
					const offset_t position{ x, y };

					draw(atlas, entities, objects, position, camera.get_offset());
				}
			}
		}

		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, cref<camera_t> camera, offset_t offset) {
			const offset_t origin{ camera.get_position() };
			const extent_t camera_extent{ camera.get_extent() };

			if (camera_extent.w == 0 || camera_extent.h == 0 || origin.x > game_map<MapType>.zone_extent.x || origin.y > game_map<MapType>.zone_extent.y) {
				return;
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y - 1) }; y < min(game_map<MapType>.zone_size.h, camera_extent.h + 1); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x - 1) }; x < min(game_map<MapType>.zone_size.w, camera_extent.w + 1); ++x) {
					const offset_t position{ x, y };

					draw(atlas, entities, objects, position, offset + camera.get_offset());
				}
			}
		}

		template<extent_t AtlasSize, EntityRegistry Entities, ObjectRegistry Objects> static inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<Entities> entities, cref<Objects> objects, cref<camera_t> camera, offset_t offset, offset_t nudge) {
			const offset_t origin{ camera.get_position() };
			const extent_t camera_extent{ camera.get_extent() };

			if (camera_extent.w == 0 || camera_extent.h == 0 || origin.x > game_map<MapType>.zone_extent.x || origin.y > game_map<MapType>.zone_extent.y) {
				return;
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y - 1) }; y < min(game_map<MapType>.zone_size.h, camera_extent.h + 1); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x - 1) }; x < min(game_map<MapType>.zone_size.w, camera_extent.w + 1); ++x) {
					const offset_t position{ x, y };

					game_map<MapType>[position].draw_floor(atlas, position, offset + camera.get_offset(), nudge + globals::CavernTileNudge);
				}
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y - 1) }; y < min(game_map<MapType>.zone_size.h, camera_extent.h + 1); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x - 1) }; x < min(game_map<MapType>.zone_size.w, camera_extent.w + 1); ++x) {
					const offset_t position{ x, y };

					draw(atlas, entities, objects, position, offset + camera.get_offset(), nudge);
				}
			}
		}
	};
} // namespace necrowarp
