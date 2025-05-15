#pragma once

#include <necrowarp/entities/entity.hpp>

#include <random>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<player_t, entity_type_t::Player> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Player> {
		using type = player_t;
	};

	template<> struct is_evil_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_non_player_entity<player_t> {
		static constexpr bool value = false;
	};

	template<> struct is_player<player_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<player_t>{ glyphs::UnarmoredPlayer };

	template<> inline constexpr glyph_t command_icons<command_type_t::RandomWarp>{ 0x00, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::TargetWarp>{ 0x01, colors::White };

	template<> inline constexpr glyph_t command_icons<command_type_t::CalciticInvocation>{ 0x02, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::SpectralInvocation>{ 0x10, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::SanguineInvocation>{ 0x11, colors::White };

	template<> inline constexpr glyph_t command_icons<command_type_t::NecromanticAscendance>{ 0x12, colors::White };

	enum class patron_e : u8 {
		None,
		Rathghul,
		Akurakhaithan,
		Merirfin,
		Saeiligarkeuss
	};

	enum class disposition_e : u8 {
		Sadistic,
		Apathetic,
		Cooperative,
	};

	enum class discount_e : u8 {
		RandomWarp,
		TargetWarp,

		CalciticInvocation,
		SpectralInvocation,
		SanguineInvocation,

		NecromanticAscendance
	};

	struct discount_t {
		const i8 negative{ 0 };
		const i8 neutral{ 0 };
		const i8 positive{ 0 };
		
		constexpr i8 current(disposition_e disposition) const noexcept {
			switch (disposition) {
				case disposition_e::Sadistic: {
					return negative;
				} case disposition_e::Apathetic: {
					return neutral;
				} case disposition_e::Cooperative: {
					return positive;
				}
			}
		} 
	};

	struct patron_t {
		disposition_e disposition{ disposition_e::Apathetic };
		
		const discount_t random_warp{};
		const discount_t target_warp{};

		const discount_t calcitic_invocation{};
		const discount_t spectral_invocation{};
		const discount_t sanguine_invocation{};

		const discount_t necromantic_ascendance{};
	};

	template<patron_e Patron> static inline patron_t patrons;

	template<> inline patron_t patrons<patron_e::None>{
		disposition_e::Apathetic,
		{ 0, 1, 2 },
		{ 0, 2, 4 }
	};

	template<> inline patron_t patrons<patron_e::Rathghul>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ 0, 4, 6 },
		{ -6, -2, 2 },
		{ -4, 0, 4 },
		{ -8, 0, 4 },
	};

	template<> inline patron_t patrons<patron_e::Akurakhaithan>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ -4, 0, 4 },
		{ 0, 4, 6 },
		{ -6, -2, 2 },
		{ -4, 0, 8 },
	};

	template<> inline patron_t patrons<patron_e::Merirfin>{
		disposition_e::Apathetic,
		{ 0, 0, 1 },
		{ 0, 0, 2 },
		{ -4, 0, 4 },
		{ -6, -2, 2 },
		{ 0, 4, 6 },
		{ -6, 0, 6 },
	};

	template<> inline patron_t patrons<patron_e::Saeiligarkeuss>{
		disposition_e::Apathetic,
		{ -1, 1, 2 },
		{ -2, 2, 4 },
		{ -4, 0, 2 },
		{ -4, 0, 2 },
		{ -4, 0, 2 },
		{ 0, 8, 16 },
	};

	static inline void reset_patrons() noexcept {
		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			patrons<cval>.disposition = disposition_e::Apathetic;
		});
	}

	static inline void randomize_patrons() noexcept {
		static std::mt19937 generator{ std::random_device{}() };
		static std::uniform_int_distribution<i16> distribution{
			-1,
			1,
		};

		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			const i16 random_number{ distribution(generator) };

			disposition_e disposition{ disposition_e::Apathetic };

			if (random_number < 0) {
				disposition = disposition_e::Sadistic;
			} else if (random_number > 0) {
				disposition = disposition_e::Cooperative;
			}

			patrons<cval>.disposition = disposition;
		});
	}

	struct player_t {
		entity_command_t command;
		offset_t position;

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

		template<discount_e Type> static constexpr i8 Cost{};

		template<> constexpr i8 Cost<discount_e::RandomWarp>{ 2 };
		template<> constexpr i8 Cost<discount_e::TargetWarp>{ 4 };

		template<> constexpr i8 Cost<discount_e::CalciticInvocation>{ 8 };
		template<> constexpr i8 Cost<discount_e::SpectralInvocation>{ 8 };
		template<> constexpr i8 Cost<discount_e::SanguineInvocation>{ 8 };

		template<> constexpr i8 Cost<discount_e::NecromanticAscendance>{ 16 };

		static constexpr i8 SkullBoon{ 1 };
		static constexpr i8 FailedWarpBoon{ 1 };
		static constexpr i8 UnsafeWarpBoon{ 1 };

	  private:
		i8 energy;
		i8 armor;
		i8 divinity;

		patron_e patron;

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
					} case discount_e::NecromanticAscendance: {
						return current_patron.necromantic_ascendance.current(disposition);
					}
				}
			}, patron);
		}

		inline void set_patron(patron_e value) noexcept {
			if (static_cast<i32>(value) < static_cast<i32>(patron_e::None) || static_cast<i32>(value) > static_cast<i32>(patron_e::Saeiligarkeuss)) {
				return;
			}

			patron = value;
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

		inline void receive_failed_warp_boon() noexcept { set_energy(energy + FailedWarpBoon); }

		inline void receive_unsafe_warp_boon() noexcept { set_energy(energy + UnsafeWarpBoon); }

		inline void max_out_energy() noexcept { energy = max_energy(); }

		inline void max_out_armor() noexcept { armor = max_armor(); }

		inline void max_out_divinity() noexcept { divinity = max_divinity(); }

		inline void zero_out_energy() noexcept { set_energy(0); }

		inline void zero_out_armor() noexcept { set_armor(0); }

		inline void zero_out_divinity() noexcept { set_divinity(0); }

		template<entity_type_t EntityType> inline bool will_perish() const noexcept;

		template<entity_type_t EntityType> inline void receive_damage() noexcept;

		template<entity_type_t EntityType> inline void receive_death_boon() noexcept;

		inline command_type_t clash_or_consume(offset_t position) const noexcept;

		inline void bolster_armor(i8 value) noexcept { set_armor(armor + value); }

		inline void erode_divinity() noexcept { set_divinity(divinity - DivinityErosionRate); }

		inline glyph_t current_glyph() const noexcept { return !has_armor() ? entity_glyphs<player_t> : glyphs::ArmoredPlayer; }

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Player; }
	};

	static inline patron_e desired_patron{ patron_e::None };
} // namespace necrowarp
