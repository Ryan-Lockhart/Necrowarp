#pragma once

#include <necrowarp/commands/binary/calcitic_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, calcitic_invocation_t>::process() noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::CalciticInvocation) || entity_registry.empty<skull_t>())) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 accumulated_skulls{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ player.position + offset };

			const bool is_empty{ entity_registry.at(position) == entity_e::None };

			if (is_empty && player.bypass_invocations_enabled()) {
				++accumulated_skulls;

				if (!is_exalted) {
					entity_registry.add<true>(skeleton_t{ position });
				}

				continue;
			}

			const bool has_skull{ entity_registry.contains<skull_t>(position) };
			const bool has_ladder{ entity_registry.contains<ladder_t>(position) };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_skull && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_skull) {
				const decay_e state{ entity_registry.at<skull_t>(position)->state };

				entity_registry.remove<skull_t>(position);

				if (!is_exalted) {
					entity_registry.add<true>(skeleton_t{ position, state });
				}

				++accumulated_skulls;
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = entity_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_type_t::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Calcitic) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (entity_registry.contains<skull_t>(player.position)) {
			const offset_t pos{ player.position };
			const decay_e state{ entity_registry.at<skull_t>(pos)->state };

			entity_registry.remove<skull_t>(pos);
			++accumulated_skulls;

			if (!is_exalted) {
				if (!entity_registry.random_warp(source_position)) {
					player.bolster_armor(accumulated_skulls);
				} else {
					entity_registry.add<true>(skeleton_t{ pos, state });
				}
			}
		}

		steam_stats::stats<steam_stat_e::SkullsConsumed, i32> += accumulated_skulls;

		if (accumulated_skulls <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (accumulated_skulls >= 4 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				// unshackle first spooky shackle achievment placeholder : A Rattling Hole?
			} else {
				eligible_ladder->enshackle(shackle_type_t::Calcitic);

				// spooky enshackle first ladder achievment placeholder : Bony Barrier
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::CalciticInvocations, i32>;

		player.pay_cost(discount_e::CalciticInvocation);

		if (!player.has_ascended()) {
			if (accumulated_skulls == 8) {
				// summon max amount of skeletons achievment placeholder : Next Stop: the Bone Zone
			} else if (accumulated_skulls > 1) {
				// summon first crew of skeletons achievment placeholder : Skeleton Crew
			}

			return;
		}
		
		// summon exalted calcitic minion here

		if (accumulated_skulls == 8) {
			// summon exalted calcitic minion with max health achievment placeholder : ?
		} else if (accumulated_skulls > 1) {
			// summon first exalted calcitic minion achievment placeholder : ?
		}
	}
} // namespace necrowarp
