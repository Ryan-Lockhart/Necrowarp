#pragma once

#include <necrowarp/commands/binary/cerebral_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, cerebral_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::CerebralInvocation) || object_registry<MapType>.dependent empty<cerebra_t>())) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 accumulated_cerebra{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position)) {
				continue;
			}

			const bool has_cerebra{ object_registry<MapType>.dependent contains<cerebra_t>(position) };

			if (!has_cerebra && player.bypass_invocations_enabled()) {
				++accumulated_cerebra;

				if (!is_exalted) {
					// entity_registry<MapType>.dependent add<true>(position, hamaz_t{ random_engine });
				}
			}

			const bool has_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

			if (!has_cerebra && (eligible_ladder != nullptr || !has_ladder)) {
				continue;
			}

			if (has_cerebra) {
				// const entity_e possessed{ object_registry<MapType>.dependent at<cerebra_t>(position)->entity };

				object_registry<MapType>.dependent remove<cerebra_t>(position);
				++accumulated_cerebra;

				if (!is_exalted) {
					// entity_registry<MapType>.dependent add<true>(position, hamaz_t{ possessed });
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
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Ravenous) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (object_registry<MapType>.dependent contains<cerebra_t>(target_position)) {
			const offset_t position{ target_position };

			// const entity_e possessed{ object_registry<MapType>.dependent at<cerebra_t>(position)->entity };

			object_registry<MapType>.dependent remove<cerebra_t>(position);
			++accumulated_cerebra;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_cerebra);
				} else {
					// entity_registry<MapType>.dependent add<true>(position, hamaz_t{ possessed });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++accumulated_cerebra;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_cerebra);
				} else {
					// entity_registry<MapType>.dependent add<true>(target_position, hamaz_t{ random_engine });
				}
			}
		}

		steam_stats::stats<steam_stat_e::BonesConsumed> += accumulated_cerebra;

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
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Ravenous) {
								eligible_ladder = nullptr;
							}
							break;
						}
					}
				}
			}
		}

		if (accumulated_cerebra <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (accumulated_cerebra >= globals::MinimumCatalyst && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				// unshackle first entrancing shackle achievment placeholder : ?
			} else {
				eligible_ladder->enshackle(shackle_e::Ravenous);

				// entrancing enshackle first ladder achievment placeholder : ?
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::RavenousInvocations>;

		player.pay_cost(discount_e::CerebralInvocation);

		if (!player.has_ascended()) {
			if (accumulated_cerebra >= globals::MaximumCatalyst) {
				// summon max amount of hamr achievment placeholder : ?
			} else if (accumulated_cerebra > 1) {
				// summon first group of hamr achievment placeholder : ?
			}

			return;
		}

		if (!chaotic_warp_t::execute<MapType>(source_position, true)) {
			player.reinvigorate(accumulated_cerebra);

			return;
		}

		// entity_registry<MapType>.dependent add<true>(source_position, furtive_horror_t{ accumulated_cerebra });

		if (accumulated_cerebra > 1) {
			// summon first furtive horror achievment placeholder : ?

			if (accumulated_cerebra >= globals::MaximumCatalyst) {
				// summon furtive horror with max health achievment placeholder : ?
			}
		}
	}
} // namespace necrowarp
