#pragma once

#include <necrowarp/entities/evil/player.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<NonNullEntity EntityType> inline bool player_t::will_perish() const noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return false;
		}

		return armor < EntityType::MaximumDamage;
	}

	template<NonNullEntity EntityType> inline void player_t::receive_damage() noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return;
		}

		set_armor(armor - EntityType::MaximumDamage);
	}

	template<NonNullEntity EntityType> inline void player_t::receive_death_boon() noexcept { set_energy(energy + EntityType::DeathBoon); }

	inline command_e player_t::clash_or_consume(offset_t position) const noexcept {
		const entity_group_e entity_group{ entity_registry.at(position) };

		const entity_e entity_target { determine_target<player_t>(entity_group) };
		
		switch (entity_target) {
			case entity_e::Adventurer:
			case entity_e::Mercenary:
			case entity_e::Paladin:
			case entity_e::Priest: {
				return command_e::Clash;
			} case entity_e::Skeleton: {
				return command_e::Consume;
			} default: {
				break;
			}
		}
		
		const object_group_e object_group{ object_registry.at(position) };

		const object_e object_target { determine_target<player_t>(object_group) };
		
		switch (object_target) {
			case object_e::Skull: {
				return command_e::Consume;
			} case object_e::Ladder: {
				return command_e::Descend;
			} default: {
				break;
			}
		}

		return command_e::None;
	}

	inline void player_t::die() noexcept {
		phase.transition(phase_e::GameOver);

		++steam_stats::stats<steam_stat_e::PlayerDeaths, i32>;
	}
} // namespace necrowarp
