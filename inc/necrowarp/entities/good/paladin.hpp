#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<paladin_t> {
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

	template<> struct to_entity_group<entity_e::Paladin> {
		static constexpr entity_group_e value = entity_group_e::Paladin;
	};

	template<> struct is_good<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<paladin_t> {
		static constexpr fluid_e type = fluid_e::Blood;
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

	template<> inline constexpr glyph_t entity_glyphs<paladin_t>{ glyphs::Paladin };

	enum struct zeal_e : u8 {
		Downtrodden, // ultra-low zeal; lowest damage attacks with lowest damage reduction (5%)

		Vengeant, // lowest zeal; highest damage attacks with lowest damage reduction (10%)
		Fallen, // lower zeal; high damage attacks with low damage reduction (20%)

		Alacritous, // middling zeal (30%)

		Righteous, // higher zeal; low damage attacks with high damage reduction (20%)
		Zealous, // highest zeal; lowest damage attacks with highest damage reduction (10%)

		Ascendant // ultra-high zeal; highest damage attacks with highest damage reduction (5%)
	};

	constexpr cstr to_string(zeal_e zeal) noexcept {
		switch (zeal) {
			case zeal_e::Downtrodden:{
				return "downtrodden";
			} case zeal_e::Vengeant:{
				return "vengeant";
			} case zeal_e::Fallen:{
				return "fallen";
			} case zeal_e::Alacritous:{
				return "alacritous";
			} case zeal_e::Righteous:{
				return "righteous";
			} case zeal_e::Zealous:{
				return "zealous";
			} case zeal_e::Ascendant:{
				return "ascendant";
			}
		}
	}

	constexpr color_t to_color(zeal_e zeal) noexcept {
		switch (zeal) {
			case zeal_e::Downtrodden:{
				return colors::dark::Orange;
			} case zeal_e::Vengeant:{
				return colors::Red;
			} case zeal_e::Fallen:{
				return colors::Orange;
			} case zeal_e::Alacritous:{
				return colors::Blue;
			} case zeal_e::Righteous:{
				return colors::Magenta;
			} case zeal_e::Zealous:{
				return colors::Yellow;
			} case zeal_e::Ascendant:{
				return colors::Green;
			}
		}
	}

	constexpr runes_t to_colored_string(zeal_e zeal) noexcept { return runes_t{ to_string(zeal), to_color(zeal) }; }

	struct paladin_t {
		const zeal_e zeal;

		static constexpr i8 MinimumHealth{ 4 };
		static constexpr i8 MiddlingHealth{ 8 };
		static constexpr i8 MaximumHealth{ 12 };

		static constexpr i8 MinimumDamage{ 2 };
		static constexpr i8 MiddlingDamage{ 4 };
		static constexpr i8 MaximumDamage{ 6 };

		static constexpr std::array<entity_e, 9> EntityPriorities{
			entity_e::Player,
			entity_e::DeathKnight,
			entity_e::Wraith,
			entity_e::FleshGolem,
			entity_e::AnimatedSuit,
			entity_e::Cultist,
			entity_e::Skeleton,
			entity_e::Bloodhound,
			entity_e::Bonespur,
		};

		static constexpr i8 MinimumDamageReceived{ 0 };
		static constexpr i8 MiddlingDamageReceived{ 2 };
		static constexpr i8 MaximumDamageReceived{ 4 };

		static constexpr i8 DeathBoon{ 3 };

	private:
		static inline std::uniform_int_distribution<u16> zeal_dis{ static_cast<u16>(zeal_e::Downtrodden), static_cast<u16>(zeal_e::Ascendant) };

		template<RandomEngine Generator> static inline zeal_e random_zeal(ref<Generator> generator) noexcept { return static_cast<zeal_e>(zeal_dis(generator)); }

		static constexpr i8 determine_health(zeal_e zeal) noexcept {
			switch (zeal) {
				case zeal_e::Downtrodden:
				case zeal_e::Vengeant: {
					return MinimumHealth;
				} case zeal_e::Fallen: {
					return MiddlingHealth - ((MiddlingHealth - MinimumHealth) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingHealth;
				} case zeal_e::Righteous: {
					return MiddlingHealth + ((MaximumHealth - MiddlingHealth) / 2);
				} case zeal_e::Zealous:
				  case zeal_e::Ascendant: {
					return MaximumHealth;
				}
			}
		}

		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		inline paladin_t() noexcept : zeal{ zeal_e::Alacritous }, health{ determine_health(zeal) } {}

		inline paladin_t(zeal_e zeal) noexcept : zeal{ zeal }, health{ determine_health(zeal) } {}

		template<RandomEngine Generator> inline paladin_t(ref<Generator> generator) noexcept :
			zeal{ random_zeal(generator) }, health{ determine_health(zeal) }
		{}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept {
			switch (zeal) {
				case zeal_e::Downtrodden:
				case zeal_e::Vengeant: {
					return MinimumHealth;
				} case zeal_e::Fallen: {
					return MiddlingHealth - ((MiddlingHealth - MinimumHealth) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingHealth;
				} case zeal_e::Righteous: {
					return MiddlingHealth + ((MaximumHealth - MiddlingHealth) / 2);
				} case zeal_e::Zealous:
				  case zeal_e::Ascendant: {
					return MaximumHealth;
				}
			}
		}

		inline i8 get_minimum_damage_received() const noexcept {
			switch (zeal) {
				case zeal_e::Ascendant:
				case zeal_e::Zealous: {
					return MinimumDamageReceived;
				} case zeal_e::Righteous: {
					return MiddlingDamageReceived - ((MiddlingDamageReceived - MinimumDamageReceived) / 2);
				} case zeal_e::Alacritous: {
					return MiddlingDamageReceived;
				} case zeal_e::Fallen: {
					return MiddlingDamageReceived + ((MaximumDamageReceived - MiddlingDamageReceived) / 2);
				} case zeal_e::Vengeant: {
				  case zeal_e::Downtrodden:
					return MaximumDamageReceived;
				}
			}
		}

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		template<CombatantEntity Attacker> constexpr i8 get_minimum_damage_received() const noexcept {
			const i8 min_damage{ get_minimum_damage_received() };

			if constexpr (is_cleaver<Attacker>::value) {
				return max<i8>(static_cast<i8>(min_damage * 1.5), 1);
			} else {
				return min_damage;
			}
		}

		template<CombatantEntity Attacker> inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received<Attacker>(), damage_amount); }

		template<CombatantEntity Attacker> inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage<Attacker>(damage_amount); }

		template<CombatantEntity Attacker> inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage<Attacker>(damage_amount)); }

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

		template<map_type_e MapType> inline void die(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} ({}) [{}/{}]",
				necrowarp::to_string(entity_e::Paladin),
				necrowarp::to_string(zeal),
				get_health(),
				max_health()
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Paladin) };

			colored_string
				.concatenate(runes_t{" (" })
				.concatenate(necrowarp::to_colored_string(zeal))
				.concatenate(runes_t{ std::format(") [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Paladin; }
	};

	static_assert(sizeof(paladin_t) <= NPCSizeCap, "paladin entity size must not exceed npc size cap!");
} // namespace necrowarp
