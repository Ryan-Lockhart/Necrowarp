#pragma once

#include <necrowarp/commands/binary/calcitic_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, calcitic_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::CalciticInvocation) || object_registry.empty<skull_t>())) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 accumulated_skulls{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			const bool has_skull{ object_registry.contains<skull_t>(position) };

			if (!has_skull && player.bypass_invocations_enabled()) {
				++accumulated_skulls;

				if (!is_exalted) {
					entity_registry.add<true>(skeleton_t{ position });
				}
			}

			const bool has_ladder{ object_registry.contains<ladder_t>(position) };

			if (!game_map.within<zone_region_e::Interior>(position) || (!has_skull && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_skull) {
				const decay_e state{ object_registry.at<skull_t>(position)->state };

				object_registry.remove<skull_t>(position);
				++accumulated_skulls;

				if (!is_exalted) {
					entity_registry.add<true>(skeleton_t{ position, state });
				}
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = object_registry.at<ladder_t>(position);

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

		if (object_registry.contains<skull_t>(target_position)) {
			const offset_t pos{ target_position };

			const decay_e state{ object_registry.at<skull_t>(pos)->state };

			object_registry.remove<skull_t>(pos);
			++accumulated_skulls;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.bolster_armor(accumulated_skulls);
				} else {
					entity_registry.add<true>(skeleton_t{ pos, state });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++accumulated_skulls;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.bolster_armor(accumulated_skulls);
				} else {
					entity_registry.add<true>(skeleton_t{ target_position });
				}
			}
		}

		steam_stats::stats<steam_stat_e::SkullsConsumed, i32> += accumulated_skulls;

		if (eligible_ladder == nullptr && source_position != target_position) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t position{ source_position + offset };
				
				const bool has_ladder{ object_registry.contains<ladder_t>(position) };

				if (!game_map.within<zone_region_e::Interior>(position) || (eligible_ladder != nullptr || !has_ladder)) {
					continue;
				}

				if (eligible_ladder == nullptr && has_ladder) {
					eligible_ladder = object_registry.at<ladder_t>(position);

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

				// unshackle first spooky shackle achievment placeholder : A Rattling Hole?
			} else {
				eligible_ladder->enshackle(shackle_e::Calcitic);

				// spooky enshackle first ladder achievment placeholder : Bony Barrier
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::CalciticInvocations, i32>;

		player.pay_cost(discount_e::CalciticInvocation);

		if (!player.has_ascended()) {
			if (accumulated_skulls == globals::MaximumCatalyst) {
				// summon max amount of skeletons achievment placeholder : Next Stop: the Bone Zone
			} else if (accumulated_skulls > 1) {
				// summon first crew of skeletons achievment placeholder : Skeleton Crew
			}

			return;
		}

		if (!entity_registry.random_warp(source_position)) {
			player.bolster_armor(accumulated_skulls);

			return;
		}

		entity_registry.add<true>(bonespur_t{ source_position, accumulated_skulls });

		if (accumulated_skulls == globals::MaximumCatalyst) {
			// summon bonespur with max health achievment placeholder : 28 STAB WOUNDS (SELF-INFLICTED)
		} else if (accumulated_skulls > 1) {
			// summon first bonespur achievment placeholder : Right to the Point
		}
	}
} // namespace necrowarp
