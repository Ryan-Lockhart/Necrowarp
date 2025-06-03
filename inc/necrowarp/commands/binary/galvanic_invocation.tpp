#pragma once

#include <necrowarp/commands/binary/galvanic_invocation.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/entities/entity.tpp>
#include <necrowarp/objects/object.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline void entity_command_t<EntityType, galvanic_invocation_t>::process() const noexcept {
		if (!player.bypass_invocations_enabled() && (!player.can_perform(discount_e::GalvanicInvocation) || object_registry.empty<metal_t>())) {
			player_turn_invalidated = true;

			return;
		}

		const bool is_exalted{ player.has_ascended() };

		i8 metal_consumed{ 0 };
		f32 aggregate_quality{ 0.0f };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (cauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ target_position + offset };

			const bool has_metal{ object_registry.contains<metal_t>(position) };

			if (!has_metal && player.bypass_invocations_enabled()) {
				++metal_consumed;
				aggregate_quality += static_cast<u8>(galvanisation_e::Writhing) + 1;

				if (!is_exalted) {
					entity_registry.add<true>(animated_suit_t{ position, galvanisation_e::Writhing });
				}
			}

			const bool has_ladder{ object_registry.contains<ladder_t>(position) };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_metal && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_metal) {
				const galvanisation_e state{ object_registry.at<metal_t>(position)->state };

				object_registry.remove<metal_t>(position);

				++metal_consumed;
				aggregate_quality += static_cast<u8>(state) + 1;

				if (!is_exalted) {
					entity_registry.add<true>(animated_suit_t{ position, triflip(random_engine) ? galvanise(state) : state });
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
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Galvanic) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (object_registry.contains<metal_t>(target_position)) {
			const galvanisation_e state{ object_registry.at<metal_t>(target_position)->state };

			object_registry.remove<metal_t>(target_position);

			++metal_consumed;
			aggregate_quality += static_cast<u8>(state) + 1;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.bolster_armor(metal_consumed);
				} else {
					entity_registry.add<true>(animated_suit_t{ target_position, triflip(random_engine) ? galvanise(state) : state });
				}
			}
		} else if (player.bypass_invocations_enabled()) {
			++metal_consumed;
			aggregate_quality += static_cast<u8>(galvanisation_e::Writhing) + 1;

			if (!is_exalted) {
				if (source_position == target_position && !entity_registry.random_warp(source_position)) {
					player.bolster_armor(metal_consumed);
				} else {
					entity_registry.add<true>(animated_suit_t{ target_position, galvanisation_e::Writhing });
				}
			}
		}

		steam_stats::stats<steam_stat_e::MetalConsumed, i32> += metal_consumed;

		if (eligible_ladder == nullptr && source_position != target_position) {
			for (cauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
				const offset_t position{ source_position + offset };

				const bool has_ladder{ object_registry.contains<ladder_t>(position) };

				if (!game_map.within<zone_region_t::Interior>(position) || eligible_ladder != nullptr || !has_ladder) {
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
							if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_e::Galvanic) {
								eligible_ladder = nullptr;
							}
							break;
						}
					}
				}
			}
		}

		if (metal_consumed <= 0) {
			player_turn_invalidated = true;

			return;
		} else if (metal_consumed >= 6 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();

				// unshackle first eldritch shackle achievment placeholder : A Chilling Draft
			} else {
				eligible_ladder->enshackle(shackle_e::Galvanic);

				// eldritch enshackle first ladder achievment placeholder : Isn't it incorporeal?
			}

			eligible_ladder = nullptr;
		}

		++steam_stats::stats<steam_stat_e::GalvanicInvocations, i32>;

		player.pay_cost(discount_e::GalvanicInvocation);

		if (!player.has_ascended()) {
			if (metal_consumed == globals::MaximumCatalyst) {
				// summon max amount of animated suits of armor achievment placeholder : Chosen of the Void
			} else if (metal_consumed > 1) {
				// summon first squad of animated suits of armor achievment placeholder : Anyone home?
			}

			return;
		}

		if (!entity_registry.random_warp(source_position)) {
			player.bolster_armor(metal_consumed * 2);

			return;
		}

		const u8 average_quality{ static_cast<u8>(std::round(aggregate_quality / metal_consumed)) };

		const galvanisation_e dk_quality{ static_cast<galvanisation_e>(
			clamp<u8>(average_quality,
				static_cast<u8>(galvanisation_e::Twisted),
				static_cast<u8>(galvanisation_e::Writhing)
			)
		)};

		entity_registry.add<true>(death_knight_t{ target_position, metal_consumed, triflip(random_engine) ? galvanise(dk_quality) : dk_quality });

		if (metal_consumed == globals::MaximumCatalyst) {
			// summon first death knight achievment placeholder : A Pale Gaze
		} else {
			// summon death knight with max heatlh achievment placeholder : Death Incarnate
		}
	}
} // namespace necrowarp
