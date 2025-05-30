#pragma once

#include <necrowarp/commands/binary/sanguine_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, sanguine_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::SanguineInvocation) || !fluid_map.template contains<zone_region_t::Interior>(fluid_type_e::Blood))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 pools_consumed{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ target_position + offset };

			const bool no_blood{ fluid_map[position] != fluid_type_e::Blood };

			if (no_blood && player.bypass_invocations_enabled()) {
				++pools_consumed;

				if (!is_exalted) {
					entity_registry.add<true>(bloodhound_t{ position });
				}

				continue;
			}

			const bool has_blood{ fluid_map[position].contains(fluid_type_e::Blood) };
			const bool has_ladder{ object_registry.contains<ladder_t>(position) };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_blood && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_blood) {
				fluid_map[position] -= fluid_type_e::Blood;
				++pools_consumed;

				if (!is_exalted) {
					entity_registry.add<true>(bloodhound_t{ position });
				}
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = object_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_type_t::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Sanguine) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (fluid_map[target_position].contains(fluid_type_e::Blood)) {
			const offset_t pos{ target_position };

			fluid_map[pos] -= fluid_type_e::Blood;
			++pools_consumed;

			if (!is_exalted) {
				if (!entity_registry.random_warp(source_position)) {
					player.bolster_armor(pools_consumed);
				} else {
					entity_registry.add<true>(bloodhound_t{ pos });
				}
			}
		}

		if (player.bypass_invocations_enabled()) {
			pools_consumed = flesh_golem_t::MaximumHealth;
		}

		steam_stats::stats<steam_stat_e::BloodConsumed, f32> += fluid_pool_volume(pools_consumed);

		if (eligible_ladder == nullptr) {
			for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
				const offset_t position{ source_position + offset };

				const bool has_ladder{ object_registry.contains<ladder_t>(position) };

				if (!game_map.within<zone_region_t::Interior>(position) || eligible_ladder != nullptr || !has_ladder) {
					continue;
				}

				if (eligible_ladder == nullptr && has_ladder) {
					eligible_ladder = object_registry.at<ladder_t>(position);

					switch (eligible_ladder->shackle) {
						case shackle_type_t::None: {
							if (eligible_ladder->is_down_ladder()) {
								eligible_ladder = nullptr;
							}
							break;
						} default: {
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Sanguine) {
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
				eligible_ladder->enshackle(shackle_type_t::Sanguine);

				// bloody enshackle first ladder achievment placeholder : Don't Touch That
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::SanguineInvocations, i32>;

		player.pay_cost(discount_e::SanguineInvocation);

		if (!player.has_ascended()) {
			if (pools_consumed == 9) {
				// summon max amount of bloodhounds achievment placeholder : The Harrying Hounds
			} else if (pools_consumed > 1) {
				// summon first pack of bloodhounds achievment placeholder : Ankle-biter
			}

			return;
		}

		if (!entity_registry.random_warp(source_position)) {
			player.bolster_armor(pools_consumed);

			return;
		}

		entity_registry.add<true>(flesh_golem_t{ source_position, pools_consumed });

		if (pools_consumed == 9) {
			// summon first flesh golem achievment placeholder : The Shambling Horror
		} else {
			// summon flesh golem with max health achievment placeholder : Mountain of Flesh
		}
	}
} // namespace necrowarp
