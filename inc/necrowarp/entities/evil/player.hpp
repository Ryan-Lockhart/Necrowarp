#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/random_warp.hpp>
#include <necrowarp/commands/unary/necromantic_ascendance.hpp>

#include <necrowarp/commands/binary/consume.hpp>
#include <necrowarp/commands/binary/descend.hpp>
#include <necrowarp/commands/binary/plunge.hpp>
#include <necrowarp/commands/binary/target_warp.hpp>
#include <necrowarp/commands/binary/consume_warp.hpp>
#include <necrowarp/commands/binary/calcitic_invocation.hpp>
#include <necrowarp/commands/binary/spectral_invocation.hpp>
#include <necrowarp/commands/binary/sanguine_invocation.hpp>
#include <necrowarp/commands/binary/galvanic_invocation.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/patronage.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<player_t> {
		static constexpr entity_e value = entity_e::Player;
	};

	template<> struct is_entity_type<player_t, entity_e::Player> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Player> {
		using type = player_t;
	};

	template<> struct to_entity_group<entity_e::Player> {
		static constexpr entity_group_e value = entity_group_e::Player;
	};

	template<> struct is_evil_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_non_player_entity<player_t> {
		static constexpr bool value = false;
	};

	template<> struct is_player<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_concussable<player_t> {
		static constexpr bool value = false;
	};

	template<> struct is_cleaver<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_vigilant<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, random_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, necromantic_ascendance_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, consume_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, descend_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, plunge_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, target_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, consume_warp_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, calcitic_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, spectral_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, sanguine_invocation_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<player_t, galvanic_invocation_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<player_t>{ glyphs::UnarmoredPlayer };

	template<> inline constexpr glyph_t command_icons<command_e::RandomWarp>{ 0x00, colors::White };
	template<> inline constexpr glyph_t command_icons<command_e::TargetWarp>{ 0x01, colors::White };

	template<> inline constexpr glyph_t command_icons<command_e::CalciticInvocation>{ 0x10, colors::White };
	template<> inline constexpr glyph_t command_icons<command_e::SpectralInvocation>{ 0x11, colors::White };
	template<> inline constexpr glyph_t command_icons<command_e::SanguineInvocation>{ 0x20, colors::White };
	template<> inline constexpr glyph_t command_icons<command_e::GalvanicInvocation>{ 0x21, colors::White };

	template<> inline constexpr glyph_t command_icons<command_e::NecromanticAscendance>{ 0x30, colors::White };

	struct player_t {
		command_pack_t command;
		offset_t position;

		patron_e patron;

		static constexpr i8 MinimumEnergy{ 4 };
		static constexpr i8 MaximumEnergy{ 24 };

		static constexpr i8 MinimumArmor{ 2 };
		static constexpr i8 MaximumArmor{ 12 };

		static constexpr i8 MinimumDivinity{ 3 };
		static constexpr i8 MaximumDivinity{ 9 };

		static constexpr i8 DivinityErosionRate{ 1 };

		static constexpr i8 StartingEnergy{ 3 };
		static constexpr i8 StartingArmor{ 0 };
		static constexpr i8 StartingDivinity{ 0 };

		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 MinimumDamage{ 1 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Skeleton,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Thetwo,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
		};

		static constexpr std::array<object_e, 5> ObjectPriorities{
			object_e::Portal,
			object_e::Ladder,
			object_e::Skull,
			object_e::Metal,
			object_e::Flesh,
		};

		template<discount_e Type> static constexpr i8 Cost{};

		template<> constexpr i8 Cost<discount_e::RandomWarp>{ 2 };
		template<> constexpr i8 Cost<discount_e::TargetWarp>{ 4 };

		template<> constexpr i8 Cost<discount_e::CalciticInvocation>{ 8 };
		template<> constexpr i8 Cost<discount_e::SpectralInvocation>{ 8 };
		template<> constexpr i8 Cost<discount_e::SanguineInvocation>{ 8 };
		template<> constexpr i8 Cost<discount_e::GalvanicInvocation>{ 12 };

		template<> constexpr i8 Cost<discount_e::NecromanticAscendance>{ 16 };

		static constexpr i8 SkullBoon{ 1 };
		static constexpr i8 FailedWarpBoon{ 2 };
		static constexpr i8 UnsafeWarpBoon{ 1 };

	  private:
		i8 energy;
		i8 armor;
		i8 divinity;

		inline void set_energy(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.energy.is_enabled()) {
				energy = clamp<i8>(value, 0, max_energy());
			} else {
				energy = max<i8>(game_stats.cheats.energy.current_minimum(), value);
			}
		}

		inline void set_armor(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.armor.is_enabled()) {
				armor = clamp<i8>(value, 0, max_armor());
			} else {
				armor = max<i8>(game_stats.cheats.armor.current_minimum(), value);
			}
		}

		inline void set_divinity(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.divinity.is_enabled()) {
				divinity = clamp<i8>(value, 0, max_divinity());
			} else {
				divinity = max<i8>(game_stats.cheats.divinity.current_minimum(), value);
			}
		}

	  public:
		inline player_t() noexcept : command{}, position{}, energy{ StartingEnergy }, armor{ StartingArmor }, divinity{ StartingDivinity } {}

		inline player_t(offset_t position) noexcept : command{}, position{ position }, energy{ StartingEnergy }, armor{ StartingArmor }, divinity{ StartingDivinity } {}

		inline i8 get_energy() const noexcept { return energy; }

		inline i8 get_armor() const noexcept { return armor; }

		inline i8 get_divinity() const noexcept { return divinity; }
		
		inline i8 get_discount(discount_e type) const noexcept {
			return magic_enum::enum_switch([&, type](auto val) -> i8 {
				constexpr patron_e cval{ val };

				cref<patron_t> current_patron{ patrons<cval> };
				const disposition_e disposition{ current_patron.disposition };

				switch (type) {
					case discount_e::RandomWarp: {
						return current_patron.random_warp.current(disposition);
					} case discount_e::TargetWarp: {
						return current_patron.target_warp.current(disposition);
					} case discount_e::CalciticInvocation: {
						return current_patron.calcitic_invocation.current(disposition);
					} case discount_e::SpectralInvocation: {
						return current_patron.spectral_invocation.current(disposition);
					} case discount_e::SanguineInvocation: {
						return current_patron.sanguine_invocation.current(disposition);
					} case discount_e::GalvanicInvocation: {
						return current_patron.galvanic_invocation.current(disposition);
					} case discount_e::NecromanticAscendance: {
						return current_patron.necromantic_ascendance.current(disposition);
					}
				}
			}, patron);
		}

		inline discount_type_e get_discount_type(discount_e discount) const noexcept {
			const i8 discount_value{ get_discount(discount) };

			return discount_value == 0 ? discount_type_e::Placebo : discount_value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
		}

		inline discount_type_e get_discount_type(i8 value) const noexcept {
			return value == 0 ? discount_type_e::Placebo : value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
		}

		inline bool has_energy() const noexcept { return energy > 0; }

		inline bool has_armor() const noexcept { return armor > 0; }

		inline bool has_ascended() const noexcept { return divinity > 0; }

		inline bool bypass_invocations_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.bypass_invocations; }

		inline i8 max_energy() const noexcept {
			const i8 calculated_max{ clamp<i8>(MinimumEnergy + game_stats.minion_kills / globals::KillsPerEnergySlot, MinimumEnergy, MaximumEnergy) };

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.energy.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline i8 max_armor() const noexcept {
			const i8 calculated_max{ clamp<i8>(MinimumArmor + game_stats.player_kills / globals::KillsPerArmorSlot, MinimumArmor, MaximumArmor) };

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.armor.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline i8 max_divinity() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumDivinity + game_stats.total_kills() / globals::KillsPerDivinityTurn, MinimumDivinity, MaximumDivinity)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.divinity.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline bool no_hit_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.no_hit; }

		inline bool can_survive(i8 damage_amount) const noexcept {
			return no_hit_enabled() || has_ascended() || armor >= damage_amount;
		}

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept {
			if (no_hit_enabled() || has_ascended()) {
				return;
			}
			
			set_armor(armor - damage_amount);
		}

		inline bool free_costs_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.free_costs; }

		inline i8 get_cost(discount_e type) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> i8 {
				constexpr discount_e cval{ val };

				return Cost<cval> - get_discount(val);
			}, type);
		}

		inline bool can_perform(discount_e type) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> bool {
				return free_costs_enabled() || energy >= get_cost(val);
			}, type);
		}

		inline bool can_perform(discount_e type, i8 discount) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> bool {
				return free_costs_enabled() || energy >= get_cost(val) - discount;
			}, type);
		}

		inline void pay_cost(discount_e type) noexcept {
			if (free_costs_enabled()) {
				return;
			}

			set_energy(energy - get_cost(type));
		}

		inline void pay_cost(discount_e type, i8 discount) noexcept {
			if (free_costs_enabled()) {
				return;
			}

			set_energy(energy - get_cost(type) + discount);
		}

		inline void receive_skull_boon() noexcept { set_energy(energy + SkullBoon); }

		inline void receive_failed_warp_boon() noexcept { set_energy(energy + clamp<i8>(FailedWarpBoon, 0, get_cost(discount_e::RandomWarp))); }

		inline void receive_unsafe_warp_boon() noexcept { set_energy(energy + clamp<i8>(UnsafeWarpBoon, 0, get_cost(discount_e::RandomWarp))); }

		inline void max_out_energy() noexcept { energy = max_energy(); }

		inline void max_out_armor() noexcept { armor = max_armor(); }

		inline void max_out_divinity() noexcept { divinity = max_divinity(); }

		inline void zero_out_energy() noexcept { set_energy(0); }

		inline void zero_out_armor() noexcept { set_armor(0); }

		inline void zero_out_divinity() noexcept { set_divinity(0); }

		template<NonNullEntity EntityType> inline bool will_perish() const noexcept;

		template<NonNullEntity EntityType> inline void receive_damage() noexcept;

		template<NonNullEntity EntityType> inline void receive_death_boon() noexcept;

		template<map_type_e MapType> inline command_e clash_or_consume(offset_t position) const noexcept;

		template<map_type_e MapType> inline void die() noexcept;

		inline void reinvigorate(i8 value) noexcept { set_energy(energy + value); }

		inline void bolster_armor(i8 value) noexcept { set_armor(armor + value); }

		inline void erode_divinity() noexcept { set_divinity(divinity - DivinityErosionRate); }

		inline glyph_t current_glyph() const noexcept { return !has_armor() ? entity_glyphs<player_t> : glyphs::ArmoredPlayer; }

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Player; }
	};

	static inline patron_e desired_patron{ patron_e::None };
} // namespace necrowarp
