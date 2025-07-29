#pragma once

#include <necrowarp/commands/unary/wander.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

#include <necrowarp/entities/good/mist_lady.hpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, wander_t>::process() const noexcept {
		if constexpr (is_afflictable<EntityType>::value || is_berker<EntityType>::value) {
			ptr<EntityType> entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

			if (entity == nullptr) {
				return;
			}

		 	if (entity_goal_map<MapType, mist_lady_t>[source_position] <= mist_lady_t::EffectRadius) {
		 		entity->soothe();
		 	}
		}

		if constexpr (is_berker<EntityType>::value) {
			ptr<EntityType> entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

			if (entity == nullptr) {
				return;
			}

			if (entity->is_calm()) {
				entity->recuperate();
			} else {
				entity->exhaust();
			}
		}

		const std::optional<offset_t> wander_pos{
			[&]() -> std::optional<offset_t> {
				static std::uniform_int_distribution<usize> wander_dis{ 0, 7 };

				usize tries{ 0 };
				

				while (tries < 8) {
					const offset_t maybe_pos{ source_position + neighbourhood_offsets<distance_function_e::Chebyshev>[wander_dis(random_engine)] };

					if (!game_map<MapType>.dependent within<region_e::Interior>(maybe_pos) || !entity_registry<MapType>.empty(maybe_pos)) {
						++tries;

						continue;
					}

					if (game_map<MapType>[maybe_pos].solid) {
						if constexpr (!is_incorporeal<EntityType>::value) {
							++tries;

							continue;
						} else {
							if constexpr (is_incorporeal<EntityType>::conditional) {
								cptr<EntityType> entity{ entity_registry<MapType>.dependent at<EntityType>(source_position) };

								if (entity == nullptr) {
									return std::nullopt;
								} else if (!entity->is_incorporeal()) {
									++tries;

									continue;
								}
							}
						}
					}

					return maybe_pos;
				}

				return std::nullopt;
			}()
		};

		if (!wander_pos.has_value()) {
			return;
		}

		entity_registry<MapType>.dependent update<EntityType>(source_position, wander_pos.value());

		if constexpr (is_player<EntityType>::value) {
			steam_stats::stats<steam_stat_e::MetersMoved> += offset_t::distance<f32>(source_position, wander_pos.value());
		}
	}
} // namespace necrowarp
