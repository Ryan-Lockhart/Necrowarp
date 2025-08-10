#pragma once

#include <necrowarp/commands/binary/wretched_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, wretched_invocation_t>::process() const noexcept {
		if (!player.can_perform(grimoire_e::WretchedInvocation) || (!player.bypass_invocations_enabled() && !fluid_map<MapType>.dependent contains<region_e::Interior>(fluid_e::Filth))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 pools_consumed{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position)) {
				continue;
			}

			const bool no_filth{ fluid_map<MapType>[position] != fluid_e::Filth };

			if (no_filth && player.bypass_invocations_enabled()) {
				++pools_consumed;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, draugaz_t{});
				}
			}

			const bool has_filth{ fluid_map<MapType>[position].contains(fluid_e::Filth) };
			const bool has_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

			if ((!has_filth && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_filth) {
				fluid_map_dirty = true;

				fluid_map<MapType>[position] -= fluid_e::Filth;
				++pools_consumed;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, draugaz_t{});
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
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Wretched) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (fluid_map<MapType>[target_position].contains(fluid_e::Filth)) {
			fluid_map_dirty = true;

			fluid_map<MapType>[target_position] -= fluid_e::Filth;
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry<MapType>.dependent add<true>(target_position, draugaz_t{});
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++pools_consumed;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.reinvigorate(pools_consumed);
				} else {
					entity_registry<MapType>.dependent add<true>(target_position, draugaz_t{});
				}
			}
		}

		steam_stats::stats<steam_stat_e::FilthConsumed> += fluid_pool_volume(pools_consumed);

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
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Wretched) {
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

				// unshackle first festering shackle achievment placeholder : What is that stench?
			} else {
				eligible_ladder->enshackle(shackle_e::Wretched);

				// festering enshackle first ladder achievment placeholder : Bulbous Barrier
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::WretchedInvocations>;

		player.pay_cost(grimoire_e::WretchedInvocation);

		literature::use(grimoire_e::WretchedInvocation);

		if (!player.has_ascended()) {
			if (pools_consumed > 1) {
				// summon first pack of draugr achievment placeholder : Desiccate and Decayed

				if (pools_consumed >= globals::MaximumCatalyst) {
					// summon max amount of draugr achievment placeholder : The Grave Legion
				}
			}

			return;
		}

		if (!chaotic_warp_t::execute<MapType>(source_position, true)) {
			player.reinvigorate(pools_consumed);

			return;
		}

		entity_registry<MapType>.dependent add<true>(source_position, dreadwurm_t{ pools_consumed });

		// summon first dreadwurm achievment placeholder : Eldritch Chimera

		if (pools_consumed >= globals::MaximumCatalyst) {
			// summon dreadwurm with max health achievment placeholder : Dread Incarnate
		}
	}
} // namespace necrowarp
