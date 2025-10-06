#pragma once

#include <necrowarp/commands/binary/calcitic_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, calcitic_invocation_t>::process() const noexcept {
		if (!player.can_perform<MapType>(grimoire_e::CalciticInvocation) || (!player.bypass_invocations_enabled() && !object_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, bones_t>(target_position))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 accumulated_skulls{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position)) {
				continue;
			}

			const bool has_skull{ object_registry<MapType>.dependent contains<bones_t>(position) };

			if (!has_skull && player.bypass_invocations_enabled()) {
				++accumulated_skulls;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, skeleton_t{});
				}
			}

			const bool has_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

			if (!has_skull && (eligible_ladder != nullptr || !has_ladder)) {
				continue;
			}

			if (has_skull) {
				const decay_e state{ object_registry<MapType>.dependent at<bones_t>(position)->get_state() };

				object_registry<MapType>.dependent remove<bones_t>(position);
				++accumulated_skulls;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, skeleton_t{ state });
				}
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = object_registry<MapType>.dependent at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_e::Unshackled: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}

						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Calcitic) {
							eligible_ladder = nullptr;
						}

						break;
					}
				}
			}
		}

		if (object_registry<MapType>.dependent contains<bones_t>(target_position)) {
			const offset_t position{ target_position };

			const decay_e state{ object_registry<MapType>.dependent at<bones_t>(position)->get_state() };

			object_registry<MapType>.dependent remove<bones_t>(position);
			++accumulated_skulls;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_skulls);
				} else {
					entity_registry<MapType>.dependent add<true>(position, skeleton_t{ state });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++accumulated_skulls;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_skulls);
				} else {
					entity_registry<MapType>.dependent add<true>(target_position, skeleton_t{});
				}
			}
		}

		steam_stats::stats<stat_e::BonesConsumed> += accumulated_skulls;

		if (eligible_ladder == nullptr && source_position != target_position) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t position{ source_position + offset };

				if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid) {
					continue;
				}

				const bool has_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

				if (!has_ladder || eligible_ladder != nullptr) {
					continue;
				}

				if (eligible_ladder == nullptr && has_ladder) {
					eligible_ladder = object_registry<MapType>.dependent at<ladder_t>(position);

					switch (eligible_ladder->shackle) {
						case shackle_e::Unshackled: {
							if (eligible_ladder->is_down_ladder()) {
								eligible_ladder = nullptr;
							}

							break;
						} default: {
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Calcitic) {
								eligible_ladder = nullptr;
							}

							break;
						}
					}
				}
			}
		}

		if (accumulated_skulls <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (accumulated_skulls >= globals::MinimumCatalyst && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				steam_stats::unlock(achievement_e::CalciticUnshackling);
			} else {
				eligible_ladder->enshackle(shackle_e::Calcitic);

				steam_stats::unlock(achievement_e::CalciticEnshackling);
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<stat_e::CalciticInvocations>;

		player.pay_cost<MapType>(grimoire_e::CalciticInvocation);

		literature::use(grimoire_e::CalciticInvocation);

		if (!player.has_ascended()) {
			  if (accumulated_skulls == 1) {
				steam_stats::unlock(achievement_e::PatheticSkeletonSummoning);
			} else if (accumulated_skulls > 1) {
				steam_stats::unlock(achievement_e::LesserSkeletonSummoning);

				if (accumulated_skulls >= globals::MaximumCatalyst) {
					steam_stats::unlock(achievement_e::GreaterSkeletonSummoning);
				}
			}

			return;
		}

		if (!chaotic_warp_t::execute<MapType>(source_position, true)) {
			player.bolster_armor(accumulated_skulls);

			return;
		}

		entity_registry<MapType>.dependent add<true>(source_position, bonespur_t{ accumulated_skulls });

		steam_stats::unlock(achievement_e::LesserBonespurSummoning);

		if (accumulated_skulls >= globals::MaximumCatalyst) {
			steam_stats::unlock(achievement_e::GreaterBonespurSummoning);
		}
	}
} // namespace necrowarp
