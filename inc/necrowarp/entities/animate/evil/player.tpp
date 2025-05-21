#pragma once

#include <necrowarp/entities/animate/evil/player.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<entity_e EntityType> inline bool player_t::will_perish() const noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return false;
		}

		if constexpr (EntityType == entity_e::Adventurer) {
			return armor < adventurer_t::MaximumDamage;
		} else if constexpr (EntityType == entity_e::Paladin) {
			return armor < paladin_t::MaximumDamage;
		} else if constexpr (EntityType == entity_e::Priest) {
			return armor < priest_t::MaximumDamage;
		}

		return false;
	}

	template<entity_e EntityType> inline void player_t::receive_damage() noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return;
		}
		
		if constexpr (EntityType == entity_e::Adventurer) {
			set_armor(armor - adventurer_t::MaximumDamage);
		} else if constexpr (EntityType == entity_e::Paladin) {
			set_armor(armor - paladin_t::MaximumDamage);
		} else if constexpr (EntityType == entity_e::Priest) {
			set_armor(armor - priest_t::MaximumDamage);
		}
	}

	template<entity_e EntityType> inline void player_t::receive_death_boon() noexcept {
		if constexpr (EntityType == entity_e::Adventurer) {
			set_energy(energy + adventurer_t::DeathBoon);
		} else if constexpr (EntityType == entity_e::Paladin) {
			set_energy(energy + paladin_t::DeathBoon);
		} else if constexpr (EntityType == entity_e::Priest) {
			set_energy(energy + priest_t::DeathBoon);
		}
	}

	inline command_e player_t::clash_or_consume(offset_t position) const noexcept {
		const entity_group_e types{ entity_registry.at(position) };
		
		switch (types) {
			case entity_e::Adventurer:
			case entity_e::Paladin:
			case entity_e::Priest: {
				return command_e::Clash;			
			} case entity_e::Skull:
			  case entity_e::Skeleton: {
				return command_e::Consume;
			} case entity_e::Ladder: {
				return command_e::Descend;
			} default: {
				return command_e::None;
			}
		}
	}
} // namespace necrowarp
