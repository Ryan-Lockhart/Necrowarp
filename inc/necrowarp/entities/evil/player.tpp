#pragma once

#include <necrowarp/entities/evil/player.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
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

	template<NonNullEntity EntityType> inline void player_t::receive_death_boon() noexcept { set_energy(energy + EntityType::DeathBoon); }

	template<NonNullEntity EntityType> inline void player_t::receive_death_boon(u8 multiplier) noexcept { set_energy(energy + EntityType::DeathBoon * multiplier); }

	template<map_type_e MapType> inline command_e player_t::clash_or_consume(offset_t position) const noexcept {
		const entity_group_e entities{ entity_registry<MapType>.at(position) };		
		const object_group_e objects{ object_registry<MapType>.at(position) };

		const entity_e entity_target { determine_target<player_t>(entities) };
		const object_e object_target { determine_target<player_t>(objects) };

		if (entity_target == entity_e::None && object_target == object_e::None) {
			return command_e::Move;
		}
		
		switch (entity_target) {
			case entity_e::Adventurer:
			case entity_e::Mercenary:
			case entity_e::Ranger:
			case entity_e::Skulker:
			case entity_e::MistLady:
			case entity_e::BannerBearer:
			case entity_e::BattleMonk:
			case entity_e::Berserker:
			case entity_e::Paladin:
			case entity_e::Thetwo: {
				return command_e::Clash;
			} case entity_e::Skeleton:
			  case entity_e::Bonespur: {
				return command_e::Consume;
			} default: {
				break;
			}
		}

		if (entity_target != entity_e::None) {
			return command_e::None;
		}
		
		switch (object_target) {
			case object_e::Bones: {
				return command_e::Consume;
			} case object_e::Ladder: {
				return command_e::Descend;
			} case object_e::Portal: {
				return command_e::Plunge;
			} default: {
				break;
			}
		}

		return command_e::Move;
	}

	inline bool player_t::can_receive_divine_intervention() const noexcept { return !divine_intervention_invoked; }

	template<map_type_e MapType, death_e Death> inline death_info_t<Death> player_t::die() noexcept {
		if constexpr (Death == death_e::Killed) {
			if (no_hit_enabled() || has_ascended()) {
				return death_info_t<Death>{ false };
			} else if (can_receive_divine_intervention() && player_t::intervention(get_patron_disposition(patron), random_engine)) {
				divine_intervention_invoked = true;

				intervention_sound.delay(random_epoch_interval(random_engine));

				return death_info_t<Death>{ false };
			}
		}

		++steam_stats::stats<steam_stat_e::PlayerDeaths>;

		phase.transition(phase_e::GameOver);

		if constexpr (Death == death_e::Devoured) {
			return death_info_t<Death>{ true, ProteinValue };
		} else {
			return death_info_t<Death>{ true };
		}
	}
} // namespace necrowarp
