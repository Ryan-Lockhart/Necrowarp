#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

#include <necrowarp/patronage.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct has_animation<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<hexeater_t> {
		static constexpr entity_e value = entity_e::Hexeater;
	};

	template<> struct is_entity_type<hexeater_t, entity_e::Hexeater> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Hexeater> {
		using type = hexeater_t;
	};

	template<> struct is_good<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<hexeater_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::BloodyEctoplasm;

		static constexpr bool conditional = false;
	};

	template<> struct is_armored<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<hexeater_t> {
		static constexpr bool value = true;
	};

	template<> struct is_holy<hexeater_t> {
		static constexpr bool value = true;
	};

	enum struct exudance_e : u8 {
		Extinguished,
		Smoldering,
		Swelling,
		Blazing,
	};

	constexpr cstr to_string(exudance_e temperament) noexcept {
		switch (temperament) {
			case exudance_e::Extinguished: {
				return "extinguished";
			} case exudance_e::Smoldering: {
				return "smoldering";
			} case exudance_e::Swelling: {
				return "swelling";
			} case exudance_e::Blazing: {
				return "blazing";
			}
		}
	}

	constexpr color_t to_color(exudance_e temperament) noexcept {
		switch (temperament) {
			case exudance_e::Extinguished: {
				return colors::light::Grey;
			} case exudance_e::Smoldering: {
				return colors::Yellow;
			} case exudance_e::Swelling: {
				return colors::Orange;
			} case exudance_e::Blazing: {
				return colors::Red;
			}
		}
	}

	constexpr runes_t to_colored_string(exudance_e temperament) noexcept { return runes_t{ to_string(temperament), to_color(temperament) }; }

	struct hexeater_t {
		keyframe_t idle_animation;

		template<disposition_e Disposition> static constexpr i8 EffectRadius{};

		template<> inline constexpr i8 EffectRadius<disposition_e::Sadistic>{ 64 };
		template<> inline constexpr i8 EffectRadius<disposition_e::Apathetic>{ 16 };
		template<> inline constexpr i8 EffectRadius<disposition_e::Cooperative>{ 4 };

		static constexpr i8 get_effect_radius() noexcept;

		static constexpr i8 BaseMaximumHealth{ 4 };
		static constexpr i8 BaseMaximumDamage{ 2 };

		static constexpr i8 HexesPerMaximumHealth{ 2 };
		static constexpr i8 HexesPerMaximumDamage{ 4 };
		
		static constexpr i8 MaximumHexes{ 8 };
		static constexpr i8 SwellingPoint{ MaximumHexes / 2 };

		template<grimoire_e Grimoire> static constexpr i8 SwellAmount{};

		template<> constexpr i8 SwellAmount<grimoire_e::ChaoticWarp>{ 1 };
		template<> constexpr i8 SwellAmount<grimoire_e::PreciseWarp>{ 1 };

		template<> constexpr i8 SwellAmount<grimoire_e::Annihilate>{ 2 };
		template<> constexpr i8 SwellAmount<grimoire_e::Repulse>{ 2 };
		template<> constexpr i8 SwellAmount<grimoire_e::Calcify>{ 1 };
		template<> constexpr i8 SwellAmount<grimoire_e::Incorporealize>{ 1 };

		template<> constexpr i8 SwellAmount<grimoire_e::CalciticInvocation>{ 3 };
		template<> constexpr i8 SwellAmount<grimoire_e::SpectralInvocation>{ 3 };
		template<> constexpr i8 SwellAmount<grimoire_e::SanguineInvocation>{ 3 };
		template<> constexpr i8 SwellAmount<grimoire_e::GalvanicInvocation>{ 3 };
		template<> constexpr i8 SwellAmount<grimoire_e::RavenousInvocation>{ 4 };
		template<> constexpr i8 SwellAmount<grimoire_e::WretchedInvocation>{ 4 };
		template<> constexpr i8 SwellAmount<grimoire_e::CerebralInvocation>{ 4 };
		template<> constexpr i8 SwellAmount<grimoire_e::InfernalInvocation>{ 4 };

		template<> constexpr i8 SwellAmount<grimoire_e::NecromanticAscendance>{ 8 };
		template<> constexpr i8 SwellAmount<grimoire_e::CalamitousRetaliation>{ 8 };

		template<grimoire_e Grimoire> static constexpr i8 RejuvenateAmount{};

		template<> constexpr i8 RejuvenateAmount<grimoire_e::ChaoticWarp>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::PreciseWarp>{ 1 };

		template<> constexpr i8 RejuvenateAmount<grimoire_e::Annihilate>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::Repulse>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::Calcify>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::Incorporealize>{ 1 };

		template<> constexpr i8 RejuvenateAmount<grimoire_e::CalciticInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::SpectralInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::SanguineInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::GalvanicInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::RavenousInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::WretchedInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::CerebralInvocation>{ 1 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::InfernalInvocation>{ 1 };

		template<> constexpr i8 RejuvenateAmount<grimoire_e::NecromanticAscendance>{ 2 };
		template<> constexpr i8 RejuvenateAmount<grimoire_e::CalamitousRetaliation>{ 2 };

		static constexpr i8 MaximumHealth{ BaseMaximumHealth + MaximumHexes / HexesPerMaximumHealth };
		static constexpr i8 MaximumDamage{ BaseMaximumDamage + MaximumHexes / HexesPerMaximumDamage };

		static constexpr i8 MaximumDamageReceived{ 2 };

		static constexpr std::array<entity_e, 19> EntityPriorities{
			entity_e::Player,
			entity_e::Skeleton,
			entity_e::Cultist,
			entity_e::Bloodhound,
			entity_e::AnimatedSuit,
			entity_e::Abomination,
			entity_e::Draugaz,
			entity_e::Hamaz,
			entity_e::Chromalese,
			entity_e::Bonespur,
			entity_e::Wraith,
			entity_e::Hemogheist,
			entity_e::DeathKnight,
			entity_e::FleshGolem,
			entity_e::Dreadwurm,
			entity_e::FurtiveHorror,
			entity_e::Isoscel,
			entity_e::Thetwo,
			entity_e::Fauna,
		};

		static constexpr i8 DeathBoon{ 3 };

		static constexpr i8 ProteinValue{ 1 };
		
	private:
		i8 health;
		i8 hexes;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		inline void set_hexes(i8 value) noexcept { hexes = clamp<i8>(value, 0, max_hexes()); sync_animation(); }

		static constexpr u8 get_index(exudance_e exudance) noexcept {
			switch (exudance) {
				case exudance_e::Extinguished: {
					return indices::ExtinguishedHexeater;
				} case exudance_e::Smoldering: {
					return indices::SmolderingHexeater;
				} case exudance_e::Swelling: {
					return indices::SwellingHexeater;
				} case exudance_e::Blazing: {
					return indices::BlazingHexeater;
				}
			}
		}

		inline void sync_animation() noexcept { idle_animation.index = get_index(get_exudance()); }

	  public:
		inline hexeater_t() noexcept : idle_animation{ random_engine, true }, health{ BaseMaximumHealth }, hexes{ 0 } { sync_animation(); }
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		inline i8 max_health() const noexcept { return BaseMaximumHealth + hexes / HexesPerMaximumHealth; }

		inline bool is_grievously_wounded() const noexcept { return health == 1; }
		
		inline i8 get_hexes() const noexcept { return hexes; }

		inline bool has_hexes() const noexcept { return hexes > 0; }

		constexpr i8 max_hexes() const noexcept { return MaximumHexes; }

		inline exudance_e get_exudance() const noexcept {
			if (has_hexes()) {
				if (hexes >= max_hexes()) {
					return exudance_e::Blazing;
				} else if (hexes >= SwellingPoint) {
					return exudance_e::Swelling;
				} else {
					return exudance_e::Smoldering;
				}
			} else {
				return exudance_e::Extinguished;
			}
		}

		constexpr i8 get_maximum_damage_received() const noexcept { return MaximumDamageReceived; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };
			
			if (actual_damage <= 0) {
				return true;
			}

			return is_grievously_wounded() ? hexes >= actual_damage : health > actual_damage;
		}

		inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };

			if (actual_damage <= 0) {
				return false;
			}

			if (is_grievously_wounded() && hexes >= actual_damage) {
				set_hexes(hexes - actual_damage);

				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		template<CombatantEntity Attacker> constexpr i8 get_maximum_damage_received() const noexcept {
			const i8 max_damage{ get_maximum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(max_damage * 2), 1);
			} else {
				return max_damage;
			}
		}

		template<CombatantEntity Attacker> inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received<Attacker>(), damage_amount); }

		template<CombatantEntity Attacker> inline bool can_survive(i8 damage_amount) const noexcept {
			const i8 actual_damage{ filter_damage<Attacker>(damage_amount) };
			
			if (actual_damage <= 0) {
				return true;
			}

			return is_grievously_wounded() ? hexes >= actual_damage : health > actual_damage;
		}

		template<CombatantEntity Attacker> inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };

			if (actual_damage <= 0) {
				return false;
			}

			if (is_grievously_wounded() && hexes >= actual_damage) {
				set_hexes(hexes - actual_damage);

				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		inline i8 get_damage() const noexcept { return BaseMaximumDamage + hexes / HexesPerMaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return BaseMaximumDamage + hexes / HexesPerMaximumDamage; }

		template<map_type_e MapType> static constexpr bool in_range() noexcept;

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline void swell() { set_hexes(hexes + 1); }

		inline void swell(i8 amount) { set_hexes(hexes + amount); }

		inline void exude() { set_hexes(hexes - 1); }

		inline void exude(i8 amount) { set_hexes(hexes - amount); }

		inline void rejuvenate() { set_health(health + 1); }

		inline void rejuvenate(i8 amount) { set_health(health + amount); }

		inline std::string to_string() const noexcept { return std::format("{} [{}/{}] ({})", necrowarp::to_string(entity_e::Hexeater), get_health(), max_health(), necrowarp::to_string(get_exudance())); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Hexeater) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{}] (", get_health(), max_health()) })
				.concatenate(necrowarp::to_colored_string(get_exudance()))
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Hexeater; }
	};

	static_assert(sizeof(hexeater_t) <= NPCSizeCap, "hexeater entity size must not exceed npc size cap!");
} // namespace necrowarp
