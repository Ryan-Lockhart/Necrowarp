#pragma once

#include <necrowarp/commands/binary/spectral_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, spectral_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::SpectralInvocation) || !fluid_map.template contains<zone_region_e::Interior>(fluid_e::Ichor))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 pools_consumed{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			const bool has_ichor{ fluid_map[position].contains(fluid_e::Ichor) };

			if (!has_ichor && player.bypass_invocations_enabled()) {
				++pools_consumed;

				if (!is_exalted) {
					entity_registry.add<true>(cultist_t{ position });
				}
			}

			const bool has_ladder{ object_registry.contains<ladder_t>(position) };

			if (!game_map.within<zone_region_e::Interior>(position) || (!has_ichor && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_ichor) {
				fluid_map[position] -= fluid_e::Ichor;
				++pools_consumed;

				if (!is_exalted) {
					entity_registry.add<true>(cultist_t{ position });
				}
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = object_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_e::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Spectral) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (fluid_map[target_position].contains(fluid_e::Ichor)) {
			const offset_t pos{ target_position };

			fluid_map[pos] -= fluid_e::Ichor;
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry.add<true>(cultist_t{ pos });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			const offset_t pos{ target_position };
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry.add<true>(cultist_t{ pos });
				}
			}
		}

		steam_stats::stats<steam_stat_e::IchorConsumed, f32> += fluid_pool_volume(pools_consumed);

		if (eligible_ladder == nullptr && source_position != target_position) {
			for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
				const offset_t position{ source_position + offset };

				const bool has_ladder{ object_registry.contains<ladder_t>(position) };

				if (!game_map.within<zone_region_e::Interior>(position) || eligible_ladder != nullptr || !has_ladder) {
					continue;
				}

				if (eligible_ladder == nullptr && has_ladder) {
					eligible_ladder = object_registry.at<ladder_t>(position);

					switch (eligible_ladder->shackle) {
						case shackle_e::None: {
							if (eligible_ladder->is_down_ladder()) {
								eligible_ladder = nullptr;
							}
							break;
						} default: {
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Spectral) {
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
		} else if (pools_consumed >= 4 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				// unshackle first eldritch shackle achievment placeholder : A Chilling Draft
			} else {
				eligible_ladder->enshackle(shackle_e::Spectral);

				// eldritch enshackle first ladder achievment placeholder : Isn't it incorporeal?
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::SpectralInvocations, i32>;

		player.pay_cost(discount_e::SpectralInvocation);

		if (!player.has_ascended()) {
			if (pools_consumed == globals::MaximumCatalyst) {
				// summon max amount of cultists achievment placeholder : ?
			} else if (pools_consumed > 1) {
				// summon first squad of cultists achievment placeholder : ?
			}

			return;
		}

		if (!entity_registry.random_warp(source_position)) {
			player.reinvigorate(pools_consumed);

			return;
		}

		entity_registry.add<true>(wraith_t{ source_position, pools_consumed });

		if (pools_consumed == globals::MaximumCatalyst) {
			// summon first wraith achievment placeholder : Intersticial
		} else {
			// summon wraith with max heatlh achievment placeholder : Summoned from Beyond
		}
	}
} // namespace necrowarp
