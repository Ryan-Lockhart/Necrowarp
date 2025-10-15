#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_animation<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<paladin_t> {
		static constexpr entity_e value = entity_e::Paladin;
	};

	template<> struct is_entity_type<paladin_t, entity_e::Paladin> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Paladin> {
		using type = paladin_t;
	};

	template<> struct is_good<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<paladin_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_armored<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_cleaver<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_encouragable<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_holy<paladin_t> {
		static constexpr bool value = true;
	};

	enum struct zeal_e : u8 {
		Vengeant, // ultra-low zeal; highest damage attacks with low damage reduction (1%)

		Downtrodden, // lowest zeal; lowest damage attacks with lowest damage reduction (4%)
		Fallen, // lower zeal; low damage attacks with low damage reduction (20%)

		Alacritous, // middling zeal; middling damage attacks with middle damage reduction (30%)

		Righteous, // higher zeal; low damage attacks with high damage reduction (20%)
		Zealous, // highest zeal; lowest damage attacks with highest damage reduction (4%)

		Ascendant // ultra-high zeal; low damage attacks with highest damage reduction (1%)
	};

	constexpr cstr to_string(zeal_e zeal) noexcept {
		switch (zeal) {
			case zeal_e::Vengeant: {
				return "vengeant";
			} case zeal_e::Downtrodden: {
				return "downtrodden";
			} case zeal_e::Fallen: {
				return "fallen";
			} case zeal_e::Alacritous: {
				return "alacritous";
			} case zeal_e::Righteous: {
				return "righteous";
			} case zeal_e::Zealous: {
				return "zealous";
			} case zeal_e::Ascendant: {
				return "ascendant";
			}
		}
	}

	constexpr color_t to_color(zeal_e zeal) noexcept {
		switch (zeal) {
			case zeal_e::Vengeant: {
				return colors::Red;
			} case zeal_e::Downtrodden: {
				return colors::dark::Orange;
			} case zeal_e::Fallen: {
				return colors::Orange;
			} case zeal_e::Alacritous: {
				return colors::Blue;
			} case zeal_e::Righteous: {
				return colors::Magenta;
			} case zeal_e::Zealous: {
				return colors::Yellow;
			} case zeal_e::Ascendant: {
				return colors::Green;
			}
		}
	}

	constexpr runes_t to_colored_string(zeal_e zeal) noexcept { return runes_t{ to_string(zeal), to_color(zeal) }; }

	struct paladin_t {
		const zeal_e zeal;

		keyframe_t idle_animation;

		static constexpr i8 MinimumHealth{ 4 };
		static constexpr i8 MiddlingHealth{ 8 };
		static constexpr i8 MaximumHealth{ 12 };

		static constexpr i8 MinimumDamage{ 2 };
		static constexpr i8 MiddlingDamage{ 4 };
		static constexpr i8 MaximumDamage{ 6 };

		static constexpr affliction_e AfflictionEquilibrium{ affliction_e::Resolute };

		static constexpr f32 BaseDemoralizeChance{ 0.25f };
		static constexpr f32 BaseEmboldenChance{ 0.125f };

		static constexpr std::array<entity_e, 19> EntityPriorities{
			entity_e::Player,
			entity_e::DeathKnight,
			entity_e::AnimatedSuit,
			entity_e::Isoscel,
			entity_e::FurtiveHorror,
			entity_e::Dreadwurm,
			entity_e::Chromalese,
			entity_e::Hamaz,
			entity_e::Draugaz,
			entity_e::FleshGolem,
			entity_e::Abomination,
			entity_e::Wraith,
			entity_e::Cultist,
			entity_e::Hemogheist,
			entity_e::Bloodhound,
			entity_e::Bonespur,
			entity_e::Skeleton,
			entity_e::Thetwo,
			entity_e::Fauna,
		};

		static constexpr i8 LowestMaximumDamageReceived{ 0 };
		static constexpr i8 MiddlingMaximumDamageReceived{ 2 };
		static constexpr i8 HighestMaximumDamageReceived{ 4 };

		static constexpr i8 DeathBoon{ 3 };

		static constexpr i8 ProteinValue{ 1 };

	private:
		static inline std::uniform_int_distribution<u16> zeal_dis{ static_cast<u16>(zeal_e::Downtrodden), static_cast<u16>(zeal_e::Ascendant) };

		static inline std::uniform_int_distribution<u16> scaled_zeal_dis{ 0, 100 };

		template<RandomEngine Generator> static inline zeal_e random_zeal(ref<Generator> generator) noexcept { return static_cast<zeal_e>(zeal_dis(generator)); }

		template<RandomEngine Generator> static inline zeal_e random_scaled_zeal(ref<Generator> generator) noexcept {
			const u16 chance{ scaled_zeal_dis(generator) };

			if (chance < 1) {
				return zeal_e::Vengeant;
			} else if (chance < 5) {
				return zeal_e::Downtrodden;
			} else if (chance < 35) {
				return zeal_e::Fallen;
			} else if (chance < 65) {
				return zeal_e::Alacritous;
			} else if (chance < 95) {
				return zeal_e::Righteous;
			} else if (chance < 99) {
				return zeal_e::Zealous;
			} else {
				return zeal_e::Ascendant;
			}
		}

		static constexpr i8 determine_health(zeal_e zeal) noexcept {
			switch (zeal) {
				case zeal_e::Downtrodden: {
					return MinimumHealth;
				} case zeal_e::Fallen: {
					return MiddlingHealth - ((MiddlingHealth - MinimumHealth) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingHealth;
				} case zeal_e::Righteous: {
					return MiddlingHealth + ((MaximumHealth - MiddlingHealth) / 2);
				} case zeal_e::Zealous:
				  case zeal_e::Vengeant:
				  case zeal_e::Ascendant: {
					return MaximumHealth;
				}
			}
		}

		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

		inline u8 get_index() const noexcept {
			switch (zeal) {
				case zeal_e::Vengeant: {
					return indices::VengeantPaladin;
				} case zeal_e::Downtrodden: {
					return indices::DowntroddenPaladin;
				} case zeal_e::Fallen: {
					return indices::FallenPaladin;
				} case zeal_e::Alacritous: {
					return indices::AlacritousPaladin;
				} case zeal_e::Righteous: {
					return indices::RighteousPaladin;
				} case zeal_e::Zealous: {
					return indices::ZealousPaladin;
				} case zeal_e::Ascendant: {
					return indices::AscendantPaladin;
				}
			}
		}

	public:
		inline paladin_t() noexcept : zeal{ zeal_e::Alacritous }, idle_animation{ get_index(), random_engine, true }, health{ determine_health(zeal) } {}

		inline paladin_t(zeal_e zeal) noexcept : zeal{ zeal }, idle_animation{ get_index(), random_engine, true }, health{ determine_health(zeal) } {}

		template<RandomEngine Generator> inline paladin_t(ref<Generator> generator) noexcept :
			zeal{ random_scaled_zeal(generator) }, idle_animation{ get_index(), random_engine, true }, health{ determine_health(zeal) }
		{}

		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept {
			switch (zeal) {
				case zeal_e::Downtrodden: {
					return MinimumHealth;
				} case zeal_e::Fallen: {
					return MiddlingHealth - ((MiddlingHealth - MinimumHealth) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingHealth;
				} case zeal_e::Righteous: {
					return MiddlingHealth + ((MaximumHealth - MiddlingHealth) / 2);
				} case zeal_e::Zealous:
				  case zeal_e::Ascendant:
				  case zeal_e::Vengeant: {
					return MaximumHealth;
				}
			}
		}

		inline i8 get_maximum_damage_received() const noexcept {
			switch (zeal) {
				case zeal_e::Ascendant:
				case zeal_e::Zealous: {
					return LowestMaximumDamageReceived;
				} case zeal_e::Righteous: {
					return MiddlingMaximumDamageReceived - ((MiddlingMaximumDamageReceived - LowestMaximumDamageReceived) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingMaximumDamageReceived;
				} case zeal_e::Fallen:
				  case zeal_e::Vengeant: {
					return MiddlingMaximumDamageReceived + ((HighestMaximumDamageReceived - MiddlingMaximumDamageReceived) / 2);
				} case zeal_e::Downtrodden: {
					return LowestMaximumDamageReceived;
				}
			}
		}

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage(damage_amount) };
			
			if (actual_damage <= 0) {
				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		template<CombatantEntity Attacker> constexpr i8 get_maximum_damage_received() const noexcept {
			const i8 max_damage{ get_maximum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(max_damage * 1.5), 1);
			} else {
				return max_damage;
			}
		}

		template<CombatantEntity Attacker> inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_maximum_damage_received<Attacker>(), damage_amount); }

		template<CombatantEntity Attacker> inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage<Attacker>(damage_amount); }

		template<CombatantEntity Attacker> inline bool receive_damage(i8 damage_amount) noexcept {
			const i8 actual_damage{ filter_damage<Attacker>(damage_amount) };
			
			if (actual_damage <= 0) {
				return false;
			}

			set_health(health - actual_damage);

			return true;
		}

		inline i8 get_damage() const noexcept {
			switch (zeal) {
				case zeal_e::Ascendant:
				case zeal_e::Zealous: {
					return MinimumDamage;
				} case zeal_e::Righteous: {
					return MiddlingDamage - ((MiddlingDamage - MinimumDamage) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingDamage;
				} case zeal_e::Fallen: {
					return MiddlingDamage + ((MaximumDamage - MiddlingDamage) / 2);
				} case zeal_e::Vengeant:
				  case zeal_e::Downtrodden: {
					return MaximumDamage;
				}
			}
		}

		inline i8 get_damage(entity_e target) const noexcept { return get_damage(); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType, death_e Death> inline death_info_t<Death> die(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} {} [{}/{}]",
				necrowarp::to_string(zeal),
				necrowarp::to_string(entity_e::Paladin),
				get_health(),
				max_health()
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(zeal) };

			colored_string
				.concatenate(runes_t{" " })
				.concatenate(necrowarp::to_colored_string(entity_e::Paladin))
				.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { animated_atlas.draw(idle_animation, colors::White, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { animated_atlas.draw(idle_animation, colors::White, position + offset, nudge); }

		constexpr operator entity_e() const noexcept { return entity_e::Paladin; }
	};

	static_assert(sizeof(paladin_t) <= NPCSizeCap, "paladin entity size must not exceed npc size cap!");
} // namespace necrowarp
