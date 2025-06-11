#pragma once

#include <necrowarp/commands/binary/sanguine_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, sanguine_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::SanguineInvocation) || !fluid_map<MapType>.template contains<zone_region_e::Interior>(fluid_e::Blood))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 pools_consumed{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			const bool no_blood{ fluid_map<MapType>[position] != fluid_e::Blood };

			if (no_blood && player.bypass_invocations_enabled()) {
				++pools_consumed;

				if (!is_exalted) {
					entity_registry<MapType>.template add<true>(bloodhound_t{ position });
				}
			}

			const bool has_blood{ fluid_map<MapType>[position].contains(fluid_e::Blood) };
			const bool has_ladder{ object_registry<MapType>.template contains<ladder_t>(position) };

			if (!game_map<MapType>.template within<zone_region_e::Interior>(position) || (!has_blood && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_blood) {
				fluid_map<MapType>[position] -= fluid_e::Blood;
				++pools_consumed;

				if (!is_exalted) {
					entity_registry<MapType>.template add<true>(bloodhound_t{ position });
				}
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = object_registry<MapType>.template at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_e::Unshackled: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Sanguine) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (fluid_map<MapType>[target_position].contains(fluid_e::Blood)) {
			fluid_map<MapType>[target_position] -= fluid_e::Blood;
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry<MapType>.random_warp(source_position)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry<MapType>.template add<true>(bloodhound_t{ target_position });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry<MapType>.random_warp(source_position)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry<MapType>.template add<true>(bloodhound_t{ target_position });
				}
			}
		}

		steam_stats::stats<steam_stat_e::BloodConsumed, f32> += fluid_pool_volume(pools_consumed);

		if (eligible_ladder == nullptr && source_position != target_position) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t position{ source_position + offset };

				const bool has_ladder{ object_registry<MapType>.template contains<ladder_t>(position) };

				if (!game_map<MapType>.template within<zone_region_e::Interior>(position) || eligible_ladder != nullptr || !has_ladder) {
					continue;
				}

				if (eligible_ladder == nullptr && has_ladder) {
					eligible_ladder = object_registry<MapType>.template at<ladder_t>(position);

					switch (eligible_ladder->shackle) {
						case shackle_e::Unshackled: {
							if (eligible_ladder->is_down_ladder()) {
								eligible_ladder = nullptr;
							}
							break;
						} default: {
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Sanguine) {
								eligible_ladder = nullptr;
							}
							break;
						}
					}
				}
			}
		}

		if (pools_consumed <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (pools_consumed >= globals::MinimumCatalyst && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				// unshackle first bloody shackle achievment placeholder : Raw and Wriggling
			} else {
				eligible_ladder->enshackle(shackle_e::Sanguine);

				// bloody enshackle first ladder achievment placeholder : Don't Touch That
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::SanguineInvocations, i32>;

		player.pay_cost(discount_e::SanguineInvocation);

		if (!player.has_ascended()) {
			if (pools_consumed == globals::MaximumCatalyst) {
				// summon max amount of bloodhounds achievment placeholder : The Harrying
			} else if (pools_consumed > 1) {
				// summon first pack of bloodhounds achievment placeholder : Ankle-biter
			}

			return;
		}

		if (!entity_registry<MapType>.random_warp(source_position)) {
			player.reinvigorate(pools_consumed);

			return;
		}

		entity_registry<MapType>.template add<true>(flesh_golem_t{ source_position, pools_consumed });

		if (pools_consumed == globals::MaximumCatalyst) {
			// summon flesh golem with max health achievment placeholder : Mountain of Flesh
		} else {
			// summon first flesh golem achievment placeholder : The Shambling Horror
		}
	}
} // namespace necrowarp
