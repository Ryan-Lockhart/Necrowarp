#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/chaotic_warp.hpp>
#include <necrowarp/commands/unary/repulse.hpp>
#include <necrowarp/commands/unary/incorporealize.hpp>
#include <necrowarp/commands/unary/necromantic_ascendance.hpp>
#include <necrowarp/commands/unary/calamitous_retaliation.hpp>

#include <necrowarp/commands/binary/consume.hpp>
#include <necrowarp/commands/binary/descend.hpp>
#include <necrowarp/commands/binary/plunge.hpp>
#include <necrowarp/commands/binary/retrieve.hpp>
#include <necrowarp/commands/binary/precise_warp.hpp>
#include <necrowarp/commands/binary/consume_warp.hpp>
#include <necrowarp/commands/binary/calcify.hpp>
#include <necrowarp/commands/binary/annihilate.hpp>
#include <necrowarp/commands/binary/calcitic_invocation.hpp>
#include <necrowarp/commands/binary/spectral_invocation.hpp>
#include <necrowarp/commands/binary/sanguine_invocation.hpp>
#include <necrowarp/commands/binary/galvanic_invocation.hpp>
#include <necrowarp/commands/binary/ravenous_invocation.hpp>
#include <necrowarp/commands/binary/wretched_invocation.hpp>
#include <necrowarp/commands/binary/cerebral_invocation.hpp>
#include <necrowarp/commands/binary/infernal_invocation.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/patronage.hpp>
#include <necrowarp/literature.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_variants<player_t> {
		static constexpr bool value = true;
	};

	TYPE_TRAIT_VALUE(is_entity, player_t, true);

	TYPE_TRAIT_USING(to_entity_type, entity_e::Player, player_t);
	TYPE_TRAIT_VALUE(to_entity_enum, player_t, entity_e::Player);
	
	TYPE_TRAIT_COMPARATOR(is_entity_type, player_t, entity_e::Player, true);
	
	TYPE_TRAIT_VALUE(to_entity_group, entity_e::Player, entity_group_e::Player);

	TYPE_TRAIT_VALUE(is_evil, player_t, true);

	TYPE_TRAIT_VALUE(is_non_player_entity, player_t, false);

	TYPE_TRAIT_VALUE(is_player, player_t, true);

	TYPE_TRAIT_VALUE(is_combatant, player_t, true);

	TYPE_TRAIT_VALUE(is_concussable, player_t, false);

	TYPE_TRAIT_VALUE(is_cleaver, player_t, true);

	TYPE_TRAIT_VALUE(is_vigilant, player_t, true);

	TYPE_TRAIT_VALUE(is_devourable, player_t, true);

	template<> struct is_incorporeal<player_t>{
		static constexpr bool value = true;
		static constexpr bool conditional = true;
	};

	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, consume_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, descend_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, plunge_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, retrieve_t, true);

	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, chaotic_warp_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, precise_warp_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, consume_warp_t, true);

	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, annihilate_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, repulse_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, incorporealize_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, calcify_t, true);

	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, calcitic_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, spectral_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, sanguine_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, galvanic_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, ravenous_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, wretched_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, cerebral_invocation_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, infernal_invocation_t, true);

	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, necromantic_ascendance_t, true);
	TYPE_TRAIT_COMPARATOR(is_entity_command_valid, player_t, calamitous_retaliation_t, true);

	struct player_t {
		command_pack_t command;
		offset_t position;

		patron_e patron;

		static constexpr i8 MinimumEnergy{ 4 };
		static constexpr i8 MaximumEnergy{ 32 };
		static constexpr i8 StartingEnergy{ 3 };

		static constexpr i8 MinimumArmor{ 2 };
		static constexpr i8 MaximumArmor{ 16 };
		static constexpr i8 StartingArmor{ 0 };

		static constexpr i8 MinimumDivinity{ 3 };
		static constexpr i8 MaximumDivinity{ 12 };
		static constexpr i8 StartingDivinity{ 0 };

		static constexpr i8 DivinityErosionRate{ 1 };

		static constexpr i8 MinimumPhantasm{ 3 };
		static constexpr i8 MaximumPhantasm{ 6 };
		static constexpr i8 StartingPhantasm{ 0 };

		static constexpr i8 PhantasmErosionRate{ 1 };

		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 MinimumDamage{ 1 };

		static constexpr std::array<entity_e, 12> EntityPriorities{
			entity_e::Skeleton,
			entity_e::Bonespur,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::Ranger,
			entity_e::Skulker,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
			entity_e::Thetwo,
		};

		static constexpr std::array<object_e, 4> ObjectPriorities{
			object_e::Portal,
			object_e::Ladder,
			object_e::Pedestal,
			object_e::Bones,
		};

		template<grimoire_e Type> static constexpr i8 Cost{};

		template<> constexpr i8 Cost<grimoire_e::ChaoticWarp>{ 2 };
		template<> constexpr i8 Cost<grimoire_e::PreciseWarp>{ 4 };

		template<> constexpr i8 Cost<grimoire_e::Annihilate>{ 8 };
		template<> constexpr i8 Cost<grimoire_e::Repulse>{ 8 };
		template<> constexpr i8 Cost<grimoire_e::Calcify>{ 4 };
		template<> constexpr i8 Cost<grimoire_e::Incorporealize>{ 4 };

		template<> constexpr i8 Cost<grimoire_e::CalciticInvocation>{ 8 };
		template<> constexpr i8 Cost<grimoire_e::SpectralInvocation>{ 8 };
		template<> constexpr i8 Cost<grimoire_e::SanguineInvocation>{ 8 };
		template<> constexpr i8 Cost<grimoire_e::GalvanicInvocation>{ 12 };
		template<> constexpr i8 Cost<grimoire_e::RavenousInvocation>{ 12 };
		template<> constexpr i8 Cost<grimoire_e::WretchedInvocation>{ 12 };
		template<> constexpr i8 Cost<grimoire_e::CerebralInvocation>{ 16 };
		template<> constexpr i8 Cost<grimoire_e::InfernalInvocation>{ 16 };

		template<> constexpr i8 Cost<grimoire_e::NecromanticAscendance>{ 16 };
		template<> constexpr i8 Cost<grimoire_e::CalamitousRetaliation>{ 24 };

		static constexpr i8 BoneBoon{ 1 };

		static constexpr i8 UnsafeWarpBoon{ 1 };
		static constexpr i8 FailedWarpBoon{ 2 };

		static constexpr i8 ProteinValue{ 1 };

	  private:
		static inline std::bernoulli_distribution apathetic_intervention_dis{ 0.01 };
		static inline std::bernoulli_distribution cooperative_intervention_dis{ 0.10 };

		static inline sound_t intervention_sound{ "res/sfx/clips/divine_intervention.flac" };

		i8 energy;
		i8 armor;
		i8 phantasm;
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

		inline void set_phantasm(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.phantasm.is_enabled()) {
				phantasm = clamp<i8>(value, 0, max_phantasm());
			} else {
				phantasm = max<i8>(game_stats.cheats.phantasm.current_minimum(), value);
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
	  	inline player_t() noexcept :
			command{}, 
			position{},
			patron{},
			energy{ StartingEnergy },
			armor{ StartingArmor },
			phantasm{ StartingPhantasm },
			divinity{ StartingDivinity }
		{}

	  	inline player_t(patron_e patron) noexcept :
			command{},
			position{},
			patron{ patron },
			energy{ StartingEnergy },
			armor{ StartingArmor },
			phantasm{ StartingPhantasm },
			divinity{ StartingDivinity }
		{}

		inline player_t(offset_t position) noexcept :
			command{},
			position{ position },
			patron{},
			energy{ StartingEnergy },
			armor{ StartingArmor },
			phantasm{ StartingPhantasm },
			divinity{ StartingDivinity }
		{}

		inline player_t(offset_t position, patron_e patron) noexcept :
			command{},
			position{ position },
			patron{ patron },
			energy{ StartingEnergy },
			armor{ StartingArmor },
			phantasm{ StartingPhantasm },
			divinity{ StartingDivinity }
		{}

		inline player_t(cref<player_t> other) noexcept :
			command{ other.command },
			position{ other.position },
			patron{ other.patron },
			energy{ other.energy },
			armor{ other.armor },
			phantasm{ other.phantasm },
			divinity{ other.divinity }
		{}

		inline player_t(rval<player_t> other) noexcept :
			command{ std::move(other.command) },
			position{ std::move(other.position) },
			patron{ std::move(other.patron) },
			energy{ std::move(other.energy) },
			armor{ std::move(other.armor) },
			phantasm{ std::move(other.phantasm) },
			divinity{ std::move(other.divinity) }
		{}

		inline ~player_t() noexcept {}

		inline ref<player_t> operator=(cref<player_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			command = other.command;
			position = other.position;
			patron = other.patron;
			energy = other.energy;
			armor = other.armor;
			phantasm = other.phantasm;
			divinity = other.divinity;

			return *this;
		}

		inline ref<player_t> operator=(rval<player_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			command = std::move(other.command);
			position = std::move(other.position);
			patron = std::move(other.patron);
			energy = std::move(other.energy);
			armor = std::move(other.armor);
			phantasm = std::move(other.phantasm);
			divinity = std::move(other.divinity);

			return *this;
		}

		inline void refresh() noexcept {
			if (game_stats.cheats.energy.is_enabled()) {
				set_energy(energy);
			}

			if (game_stats.cheats.armor.is_enabled()) {
				set_armor(armor);
			}

			if (game_stats.cheats.phantasm.is_enabled()) {
				set_phantasm(phantasm);
			}

			if (game_stats.cheats.divinity.is_enabled()) {
				set_divinity(divinity);
			}
		}

		inline void reset() noexcept { (*this) = player_t{}; }

		inline void reset(offset_t position) noexcept { (*this) = player_t{ position }; }

		inline void reset(patron_e patron) noexcept { (*this) = player_t{ patron }; }

		inline void reset(offset_t position, patron_e patron) noexcept { (*this) = player_t{ position, patron }; }

		inline i8 get_energy() const noexcept { return energy; }

		inline i8 get_armor() const noexcept { return armor; }

		inline i8 get_phantasm() const noexcept { return phantasm; }

		inline i8 get_divinity() const noexcept { return divinity; }
		
		inline i8 get_discount(grimoire_e type) const noexcept {
			return magic_enum::enum_switch([&, type](auto patron_val) -> i8 {
				constexpr patron_e patron_cval{ patron_val };

				cref<patron_t> current_patron{ patrons<patron_cval> };
				const disposition_e disposition{ current_patron.disposition };

				return magic_enum::enum_switch([&](auto type_val) -> i8 {
					constexpr grimoire_e type_cval{ type_val };

					return current_patron.get_discount<type_cval>().current(disposition);
				}, type);
			}, patron);
		}

		inline discount_type_e get_discount_type(grimoire_e discount) const noexcept {
			const i8 discount_value{ get_discount(discount) };

			return discount_value == 0 ? discount_type_e::Placebo : discount_value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
		}

		inline discount_type_e get_discount_type(i8 value) const noexcept {
			return value == 0 ? discount_type_e::Placebo : value < 0 ? discount_type_e::Malus : discount_type_e::Boon;
		}

		inline bool has_energy() const noexcept { return energy > 0; }

		inline bool has_armor() const noexcept { return armor > 0; }

		inline bool is_incorporeal() const noexcept { return phantasm > 0; }

		inline bool has_ascended() const noexcept { return divinity > 0; }

		inline bool bypass_invocations_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.bypass_invocations; }

		inline i8 max_energy() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumEnergy + game_stats.minion_kills / globals::KillsPerEnergySlot, MinimumEnergy, MaximumEnergy)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.energy.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline i8 max_armor() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumArmor + game_stats.player_kills / globals::KillsPerArmorSlot, MinimumArmor, MaximumArmor)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.armor.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.armor.current_maximum(), calculated_max);
		}

		inline i8 max_phantasm() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumPhantasm + game_stats.total_kills() / globals::KillsPerPhantasmTurn, MinimumPhantasm, MaximumPhantasm)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.phantasm.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.phantasm.current_maximum(), calculated_max);
		}

		inline i8 max_divinity() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumDivinity + game_stats.total_kills() / globals::KillsPerDivinityTurn, MinimumDivinity, MaximumDivinity)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.divinity.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.divinity.current_maximum(), calculated_max);
		}

		inline bool no_hit_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.no_hit; }

		inline bool can_survive(i8 damage_amount) const noexcept {
			return no_hit_enabled() || has_ascended() || armor >= damage_amount;
		}

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline bool receive_damage(i8 damage_amount) noexcept {
			if (no_hit_enabled() || has_ascended() || damage_amount <= 0) {
				return false;
			}
			
			set_armor(armor - damage_amount);

			return true;
		}

		inline bool free_costs_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.free_costs; }

		inline i8 get_cost(grimoire_e type) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> i8 {
				constexpr grimoire_e cval{ val };

				return Cost<cval> - get_discount(val);
			}, type);
		}

		constexpr bool can_consume(entity_e entity) const noexcept {
			switch (entity) {
				case entity_e::Skeleton:
				case entity_e::Bonespur: {
					return true;
				} default: {
					return false;
				}
			}
		}

		constexpr bool can_consume(object_e object) const noexcept {
			switch (object) {
				case object_e::Bones: {
					return true;
				} default: {
					return false;
				}
			}
		}

		inline bool can_perform(grimoire_e type) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				if (!grimoire_s<cval>::can_use()) {
					return false;
				}

				const bool sufficient_energy{ free_costs_enabled() || energy >= get_cost(val) };

				if constexpr (cval == grimoire_e::CalamitousRetaliation) {
					return has_ascended() && sufficient_energy;
				} else {
					return sufficient_energy;
				}
			}, type);
		}

		inline bool can_perform(grimoire_e type, i8 discount) const noexcept {
			return magic_enum::enum_switch([&, this](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				if (!grimoire_s<cval>::can_use()) {
					return false;
				}

				const bool sufficient_energy{ free_costs_enabled() || energy >= get_cost(val) - discount };

				if constexpr (cval == grimoire_e::CalamitousRetaliation) {
					return has_ascended() && sufficient_energy;
				} else {
					return sufficient_energy;
				}
			}, type);
		}

		inline void pay_cost(grimoire_e type) noexcept {
			if (free_costs_enabled()) {
				return;
			}

			set_energy(energy - get_cost(type));
		}

		inline void pay_cost(grimoire_e type, i8 discount) noexcept {
			if (free_costs_enabled()) {
				return;
			}

			set_energy(energy - get_cost(type) + discount);
		}

		inline void receive_skull_boon() noexcept { set_energy(energy + BoneBoon); }

		inline void receive_failed_warp_boon() noexcept { set_energy(energy + clamp<i8>(FailedWarpBoon, 0, get_cost(grimoire_e::ChaoticWarp))); }

		inline void receive_unsafe_warp_boon() noexcept { set_energy(energy + clamp<i8>(UnsafeWarpBoon, 0, get_cost(grimoire_e::ChaoticWarp))); }

		inline void max_out_energy() noexcept { energy = max_energy(); }

		inline void max_out_armor() noexcept { armor = max_armor(); }

		inline void max_out_phantasm() noexcept { phantasm = max_phantasm(); }

		inline void max_out_divinity() noexcept { divinity = max_divinity(); }

		inline void zero_out_energy() noexcept { set_energy(0); }

		inline void zero_out_armor() noexcept { set_armor(0); }

		inline void zero_out_phantasm() noexcept { set_phantasm(0); }

		inline void zero_out_divinity() noexcept { set_divinity(0); }

		template<CombatantEntity EntityType> inline bool will_perish() const noexcept;

		template<CombatantEntity EntityType> inline bool receive_damage() noexcept;

		template<NonNullEntity EntityType> inline void receive_death_boon() noexcept;

		template<NonNullEntity EntityType> inline void receive_death_boon(u8 multiplier) noexcept;

		template<map_type_e MapType> inline command_e clash_or_consume(offset_t position) const noexcept;

		inline bool can_receive_divine_intervention() const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die() noexcept;

		template<RandomEngine Generator> static inline bool intervention(disposition_e disposition, ref<Generator> engine) noexcept {
			switch (disposition) {
				case disposition_e::Cooperative: {
					return cooperative_intervention_dis(engine);
				} case disposition_e::Apathetic: {
					return apathetic_intervention_dis(engine);
				} case disposition_e::Sadistic: {
					return false;
				}
			}
		}

		inline void reinvigorate(i8 value) noexcept { set_energy(energy + value); }

		inline void bolster_armor(i8 value) noexcept { set_armor(armor + value); }

		inline void erode_divinity() noexcept { set_divinity(divinity - DivinityErosionRate); }

		inline void erode_phantasm() noexcept { set_phantasm(phantasm - PhantasmErosionRate); }

		inline glyph_t current_glyph() const noexcept {
			return glyph_t{
				has_armor() ? characters::ArmoredPlayer : characters::UnarmoredPlayer,
				is_incorporeal() ? colors::White.faded(0.1 + stealth_wave.current_value()) : colors::White
			};
		}

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Player; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<player_t> player) noexcept { return offset_t::std_hasher::operator()(player.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<player_t> lhs, cref<player_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<player_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<player_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};

	static inline patron_e desired_patron{ patron_e::None };
} // namespace necrowarp
