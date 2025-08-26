#pragma once

#include <necrowarp/entities/evil/player.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

namespace necrowarp {

	template<map_type_e MapType> inline void player_t::swell_hexeaters(grimoire_e type) noexcept {
		if (!hexeater_t::in_range<MapType>()) {
			return;
		}

		cauto current_radius{ hexeater_t::get_effect_radius() };

		cauto [swell_amount, rejuvenate_amount] = magic_enum::enum_switch([&](auto val) {
			constexpr grimoire_e cval{ val };
			return std::make_pair(hexeater_t::SwellAmount<cval>, hexeater_t::RejuvenateAmount<cval>);
		}, type);

		for (rauto [position, hexeater] : entity_registry_storage<hexeater_t>) {
			if (entity_goal_map<MapType, player_t>.dependent average<region_e::Interior, distance_function_e::Chebyshev>(position) > current_radius) {
				continue;
			}

			hexeater.swell(swell_amount);
			hexeater.rejuvenate(rejuvenate_amount);
		}
	}

	template<map_type_e MapType> inline bool player_t::can_perform(grimoire_e type) const noexcept {
		return magic_enum::enum_switch([&, this](auto val) -> bool {
			constexpr grimoire_e cval{ val };

			if (!endow_knowledge_enabled() && !grimoire_s<cval>::can_use()) {
				return false;
			}

			const bool sufficient_energy{ free_costs_enabled() || energy >= get_cost(val) };

			const bool blocked{ mansling_t::in_range<MapType>() };

			if constexpr (cval == grimoire_e::CalamitousRetaliation) {
				return has_ascended() && sufficient_energy;
			} else if constexpr (cval == grimoire_e::ChaoticWarp || cval == grimoire_e::NecromanticAscendance) {
				return sufficient_energy;
			} else {
				return !blocked && sufficient_energy;
			}
		}, type);
	}

	template<map_type_e MapType> inline bool player_t::can_perform(grimoire_e type, i8 discount) const noexcept {
		return magic_enum::enum_switch([&, this](auto val) -> bool {
			constexpr grimoire_e cval{ val };

			if (!endow_knowledge_enabled() && !grimoire_s<cval>::can_use()) {
				return false;
			}

			const bool sufficient_energy{ free_costs_enabled() || energy >= get_cost(val) - discount };

			const bool blocked{ mansling_t::in_range<MapType>() };

			if constexpr (cval == grimoire_e::CalamitousRetaliation) {
				return has_ascended() && sufficient_energy;
			} else if constexpr (cval == grimoire_e::ChaoticWarp || cval == grimoire_e::NecromanticAscendance) {
				return sufficient_energy;
			} else {
				return !blocked && sufficient_energy;
			}
		}, type);
	}

	template<map_type_e MapType> inline void player_t::pay_cost(grimoire_e type) noexcept {
		swell_hexeaters<MapType>(type);

		if (free_costs_enabled()) {
			return;
		}

		set_energy(energy - get_cost(type));
	}

	template<map_type_e MapType> inline void player_t::pay_cost(grimoire_e type, i8 discount) noexcept {
		swell_hexeaters<MapType>(type);

		if (free_costs_enabled()) {
			return;
		}

		set_energy(energy - get_cost(type) + discount);
	}

	template<CombatantEntity EntityType> inline bool player_t::will_perish() const noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return false;
		}

		return armor < EntityType::MaximumDamage;
	}

	template<CombatantEntity EntityType> inline bool player_t::receive_damage() noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return false;
		}

		set_armor(armor - EntityType::MaximumDamage);

		return true;
	}

	template<Entity EntityType> inline void player_t::receive_death_boon() noexcept { set_energy(energy + EntityType::DeathBoon); }

	template<Entity EntityType> inline void player_t::receive_death_boon(u8 multiplier) noexcept { set_energy(energy + EntityType::DeathBoon * multiplier); }

	template<map_type_e MapType> inline command_e player_t::clash_or_consume(offset_t position) const noexcept {
		const std::optional<entity_e> maybe_entity{ entity_registry<MapType>.at(position) };

		if (maybe_entity.has_value()) {
			const entity_e entity_target{ maybe_entity.value() };

			switch (entity_target) {
				case entity_e::Adventurer:
				case entity_e::Mercenary:
				case entity_e::Ranger:
				case entity_e::Skulker:
				case entity_e::Mansling:
				case entity_e::MistLady:
				case entity_e::BannerBearer:
				case entity_e::Medicus:
				case entity_e::BattleMonk:
				case entity_e::Berserker:
				case entity_e::Hexeater:
				case entity_e::Paladin:
				case entity_e::Thetwo: {
					return command_e::Clash;
				} case entity_e::Skeleton:
				  case entity_e::Bonespur: {
					return command_e::Consume;
				} default: {
					return command_e::None;
				}
			}
		}

		const object_e object_target { determine_target<player_t>(object_registry<MapType>.at(position)) };
		
		switch (object_target) {
			case object_e::Portal: {
				return command_e::Plunge;
			} case object_e::Ladder: {
				return command_e::Descend;
			} case object_e::Pedestal: {
				return command_e::Retrieve;
			} case object_e::Bones: {
				return command_e::Consume;
			} default: {
				return command_e::Move;
			}
		}
	}

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> player_t::die() noexcept {
		if constexpr (Death == death_e::Killed) {
			if (no_hit_enabled() || has_ascended()) {
				return death_info_t<Death>{ false };
			} else if (player_t::intervention(get_patron_disposition(patron), random_engine)) {
				annoy(player.patron);

				intervention_sound.delay(random_epoch_interval(random_engine));

				steam_stats::unlock(achievement_e::ReceiveDivineIntervention);

				return death_info_t<Death>{ false };
			}
		}

		++steam_stats::stats<stat_e::PlayerDeaths>;

		phase.transition(phase_e::GameOver);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
