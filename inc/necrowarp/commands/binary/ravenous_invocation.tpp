#pragma once

#include <necrowarp/commands/binary/ravenous_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<Entity EntityType> template<map_type_e MapType> inline void entity_command_t<EntityType, ravenous_invocation_t>::process() const noexcept {
		if (!player.can_perform<MapType>(grimoire_e::RavenousInvocation) || (!player.bypass_invocations_enabled() && !object_registry<MapType>.dependent nearby<distance_function_e::Chebyshev, flesh_t>(target_position))) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 accumulated_flesh{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_e::Chebyshev>) {
			const offset_t position{ target_position + offset };

			if (!game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || !entity_registry<MapType>.empty(position)) {
				continue;
			}

			const bool has_flesh{ object_registry<MapType>.dependent contains<flesh_t>(position) };

			if (!has_flesh && player.bypass_invocations_enabled()) {
				++accumulated_flesh;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, abomination_t{});
				}
			}

			const bool has_ladder{ object_registry<MapType>.dependent contains<ladder_t>(position) };

			if (!has_flesh && (eligible_ladder != nullptr || !has_ladder)) {
				continue;
			}

			if (has_flesh) {
				object_registry<MapType>.dependent remove<flesh_t>(position);
				++accumulated_flesh;

				if (!is_exalted) {
					entity_registry<MapType>.dependent add<true>(position, abomination_t{});
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

		if (object_registry<MapType>.dependent contains<flesh_t>(target_position)) {
			const offset_t position{ target_position };

			object_registry<MapType>.dependent remove<flesh_t>(position);
			++accumulated_flesh;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_flesh);
				} else {
					entity_registry<MapType>.dependent add<true>(position, abomination_t{});
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++accumulated_flesh;

			if (!is_exalted) {
				if (source_position == target_position && !chaotic_warp_t::execute<MapType>(source_position, true)) {
					player.bolster_armor(accumulated_flesh);
				} else {
					entity_registry<MapType>.dependent add<true>(target_position, abomination_t{});
				}
			}
		}

		steam_stats::stats<stat_e::BonesConsumed> += accumulated_flesh;

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

		if (accumulated_flesh <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (accumulated_flesh >= globals::MinimumCatalyst && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				steam_stats::unlock(achievement_e::RavenousUnshackling);
			} else {
				eligible_ladder->enshackle(shackle_e::Ravenous);

				steam_stats::unlock(achievement_e::RavenousEnshackling);
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<stat_e::RavenousInvocations>;

		player.pay_cost<MapType>(grimoire_e::RavenousInvocation);

		literature::use(grimoire_e::RavenousInvocation);

		if (!player.has_ascended()) {
			if (accumulated_flesh > 1) {
				steam_stats::unlock(achievement_e::LesserAbominationSummoning);

				if (accumulated_flesh >= globals::MaximumCatalyst) {
					steam_stats::unlock(achievement_e::GreaterAbominationSummoning);
				}
			}

			return;
		}

		if (!chaotic_warp_t::execute<MapType>(source_position, true)) {
			player.reinvigorate(accumulated_flesh);

			return;
		}

		entity_registry<MapType>.dependent add<true>(source_position, flesh_golem_t{ accumulated_flesh });

		steam_stats::unlock(achievement_e::LesserFleshGolemSummoning);

		if (accumulated_flesh >= globals::MaximumCatalyst) {
			steam_stats::unlock(achievement_e::GreaterFleshGolemSummoning);
		}
	}
} // namespace necrowarp
