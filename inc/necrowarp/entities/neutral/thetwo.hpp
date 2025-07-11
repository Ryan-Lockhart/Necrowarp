#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>

#include <necrowarp/entity_command.hpp>

#include <necrowarp/commands/unary/metabolise.hpp>
#include <necrowarp/commands/unary/shed.hpp>

#include <necrowarp/commands/binary/devour.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct globals::has_variants<thetwo_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<thetwo_t> {
		static constexpr entity_e value = entity_e::Thetwo;
	};

	template<> struct is_entity_type<thetwo_t, entity_e::Thetwo> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::Thetwo> {
		using type = thetwo_t;
	};

	template<> struct to_entity_group<entity_e::Thetwo> {
		static constexpr entity_group_e value = entity_group_e::Thetwo;
	};

	template<> struct is_neutral<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<thetwo_t> {
		static constexpr bool value = true;
		static constexpr fluid_e type = fluid_e::Blood;

		static constexpr bool conditional = false;
	};

	template<> struct is_cleaver<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_devourable<thetwo_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<thetwo_t, metabolise_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<thetwo_t, shed_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_command_valid<thetwo_t, devour_t> {
		static constexpr bool value = true;
	};

	enum struct bulk_e : u8 {
		Neonatal,
		Young,
		Mature,
		Gross,
		Titanic
	};

	static constexpr bulk_e grow(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return bulk_e::Young;
			} case bulk_e::Young: {
				return bulk_e::Mature;
			} case bulk_e::Mature: {
				return bulk_e::Gross;
			} case bulk_e::Gross: {
				return bulk_e::Titanic;
			} case bulk_e::Titanic: {
				return bulk_e::Titanic;
			}
		}
	}

	constexpr cstr to_string(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return "neonatal";
			} case bulk_e::Young: {
				return "young";
			} case bulk_e::Mature: {
				return "mature";
			} case bulk_e::Gross: {
				return "gross";
			} case bulk_e::Titanic: {
				return "titanic";
			}
		}
	}

	constexpr color_t to_color(bulk_e bulk) noexcept {
		switch (bulk) {
			case bulk_e::Neonatal: {
				return colors::light::Blue;
			} case bulk_e::Young: {
				return colors::light::Green;
			} case bulk_e::Mature: {
				return colors::light::Yellow;
			} case bulk_e::Gross: {
				return colors::light::Orange;
			} case bulk_e::Titanic: {
				return colors::light::Red;
			}
		}
	}

	constexpr runes_t to_colored_string(bulk_e bulk) noexcept { return runes_t{ to_string(bulk), to_color(bulk) }; }

	struct thetwo_t {
		static constexpr i8 MaximumHealth{ 32 };
		static constexpr i8 MinimumHealth{ 2 };

		static_assert(MaximumHealth >> 4 == MinimumHealth, "minimum health should be precisely four powers below maximum health!");

		static constexpr i8 MaximumProtein{ 64 };
		static constexpr i8 MinimumProtein{ 4 };

		static_assert(MaximumProtein >> 4 == MinimumProtein, "minimum protein should be precisely four powers below maximum protein!");

		static constexpr i8 MinimumDamage{ 1 };
		static constexpr i8 MaximumDamage{ 8 };

		static constexpr std::array<entity_e, 20> EntityPriorities{
			entity_e::Player,
			entity_e::Abomination,
			entity_e::FleshGolem,
			entity_e::Skulker,
			entity_e::Ranger,
			entity_e::MistLady,
			entity_e::BannerBearer,
			entity_e::Adventurer,
			entity_e::Mercenary,
			entity_e::BattleMonk,
			entity_e::Berserker,
			entity_e::Paladin,
			entity_e::Bloodhound,
		 	entity_e::Hemogheist,
			entity_e::Skeleton,
			entity_e::Bonespur,
			entity_e::Cultist,
			entity_e::Wraith,
			entity_e::AnimatedSuit,
			entity_e::DeathKnight,
		};

		static constexpr std::array<object_e, 1> ObjectPriorities{ object_e::Flesh };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 SheddingTurns{ 8 };
		static constexpr i8 MinimumShedAmount{ 2 };

		static constexpr i8 MetabolisedProtein{ 2 };
		static constexpr i8 MetabolisedHealth{ 1 };
		
	private:
		static constexpr i8 determine_health(bulk_e bulk) noexcept {
			switch (bulk) {
				case bulk_e::Neonatal: {
					return MaximumHealth >> 4;
				} case bulk_e::Young: {
					return MaximumHealth >> 3;
				} case bulk_e::Mature: {
					return MaximumHealth >> 2;
				} case bulk_e::Gross: {
					return MaximumHealth >> 1;
				} case bulk_e::Titanic: {
					return MaximumHealth;
				}
			}
		}

		static inline std::uniform_int_distribution<u16> bulk_dis{ static_cast<u16>(bulk_e::Neonatal), static_cast<u16>(bulk_e::Titanic) };

		template<RandomEngine Generator> static inline bulk_e random_bulk(ref<Generator> generator) noexcept { return static_cast<bulk_e>(bulk_dis(generator)); }

		bulk_e bulk;

		i8 health;
		i8 protein;

		bool shedding;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
		inline void set_protein(i8 value) noexcept { protein = clamp<i8>(value, 0, max_protein()); }

	public:
		inline thetwo_t() noexcept : bulk{ bulk_e::Neonatal }, health{ determine_health(bulk) }, protein{ 0 }, shedding{ false } {}

		inline thetwo_t(bulk_e bulk) noexcept : bulk{ bulk }, health{ determine_health(bulk) }, protein{ 0 }, shedding{ false } {}

		template<RandomEngine Generator> inline thetwo_t(ref<Generator> engine) noexcept :
			bulk{ random_bulk(engine) }, health{ determine_health(bulk) }, protein{ 0 }, shedding{ false }
		{}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept {
			switch (bulk) {
				case bulk_e::Neonatal: {
					return MaximumHealth >> 4;
				} case bulk_e::Young: {
					return MaximumHealth >> 3;
				} case bulk_e::Mature: {
					return MaximumHealth >> 2;
				} case bulk_e::Gross: {
					return MaximumHealth >> 1;
				} case bulk_e::Titanic: {
					return MaximumHealth;
				}
			}
		}
		
		inline i8 get_protein() const noexcept { return protein; }

		inline bool has_protein() const noexcept { return protein > 0; }

		constexpr i8 max_protein() const noexcept {
			switch (bulk) {
				case bulk_e::Neonatal: {
					return MaximumProtein >> 4;
				} case bulk_e::Young: {
					return MaximumProtein >> 3;
				} case bulk_e::Mature: {
					return MaximumProtein >> 2;
				} case bulk_e::Gross: {
					return MaximumProtein >> 1;
				} case bulk_e::Titanic: {
					return MaximumProtein;
				}
			}
		}

		template<bulk_e Bulk> inline bool can_devour(entity_e entity) const noexcept {
			return false;
		}
		
		template<> inline bool can_devour<bulk_e::Titanic>(entity_e entity) const noexcept {
			switch (entity) {
				case entity_e::Player:
				case entity_e::Abomination:
				case entity_e::Skulker:
				case entity_e::Ranger:
				case entity_e::MistLady:
				case entity_e::BannerBearer:
				case entity_e::Adventurer:
				case entity_e::Mercenary:
				case entity_e::BattleMonk:
				case entity_e::Berserker:
				case entity_e::Paladin: {
					return true;
				} default: {
					return false;
				}
			}
		}
		
		template<> inline bool can_devour<bulk_e::Gross>(entity_e entity) const noexcept {
			switch (entity) {
				case entity_e::Skulker:
				case entity_e::Ranger:
				case entity_e::MistLady:
				case entity_e::Adventurer:
				case entity_e::Mercenary: {
					return true;
				} default: {
					return false;
				}
			}
		}
		
		template<> inline bool can_devour<bulk_e::Mature>(entity_e entity) const noexcept {
			switch (entity) {
				case entity_e::Adventurer: {
					return true;
				} default: {
					return false;
				}
			}
		}

		inline bool can_devour(entity_e entity) const noexcept {
			if (protein >= max_protein()) {
				return false;
			}
			
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr bulk_e cval{ val };

				return can_devour<cval>(entity);
			}, bulk);
		}

		inline bool can_devour(object_e object) const noexcept { return protein < max_protein() && object == object_e::Flesh; }

		inline void fatten() noexcept { set_protein(protein + 1); }

		inline void fatten(i8 amount) noexcept { set_protein(protein + amount); }

		inline bool can_metabolise() const noexcept { return protein >= MetabolisedProtein && health < max_health(); }

		inline void metabolise() noexcept {
			if (!can_metabolise()) {
				return;
			}

			set_protein(protein - MetabolisedProtein);
			set_health(health + MetabolisedHealth);
		}

		inline bool is_shedding() const noexcept { return shedding; }

		inline bool can_shed() const noexcept { return !is_shedding() && protein >= max_protein() && bulk != bulk_e::Titanic; }

		inline i8 shed_amount() const noexcept { return max<i8>(max_protein() / SheddingTurns, MinimumShedAmount); }

		inline void shed() noexcept {
			if (!shedding) {
				if (can_shed()) {
					shedding = true;
				} else {
					return;
				}
			}

			set_protein(protein - shed_amount());

			if (!has_protein()) {
				shedding = false;

				bulk = grow(bulk);

				set_health(max_health());
			}
		}

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline i8 get_damage() const noexcept {
			switch (bulk) {
				case bulk_e::Neonatal: {
					return MinimumDamage;
				} case bulk_e::Young: {
					return MinimumDamage + 1;
				} case bulk_e::Mature: {
					return MaximumDamage / 2;
				} case bulk_e::Gross: {
					return MaximumDamage - 2;
				} case bulk_e::Titanic: {
					return MaximumDamage;
				}
			}
		}

		inline i8 get_damage(entity_e target) const noexcept { return get_damage(); }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline u8 get_droppings() const noexcept { return static_cast<u8>(bulk); }

		template<map_type_e MapType> inline command_pack_t think(offset_t position) const noexcept;

		template<map_type_e MapType> inline void killed(offset_t position) noexcept;

		template<map_type_e MapType> inline i8 devoured(offset_t position) noexcept;

		inline std::string to_string() const noexcept {
			return std::format("{} [{}/{}] ({}, [{}/{}])",
				necrowarp::to_string(entity_e::Thetwo),
				get_health(), max_health(),
				necrowarp::to_string(bulk),
				get_protein(), max_protein()
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::Thetwo) };

			colored_string
				.concatenate(runes_t{ std::format(" [{}/{}] (", get_health(), max_health()) })
				.concatenate(necrowarp::to_colored_string(bulk))
				.concatenate(shedding ?
					runes_t{ ", shedding)" } :
					runes_t{ std::format(", [{}/{}])", get_protein(), max_protein())});

			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (bulk) {
				case bulk_e::Neonatal: {
					return glyphs::NeonatalThetwo;
				} case bulk_e::Young: {
					return glyphs::YoungThetwo;
				} case bulk_e::Mature: {
					return glyphs::MatureThetwo;
				} case bulk_e::Gross: {
					return glyphs::GrossThetwo;
				} case bulk_e::Titanic: {
					return glyphs::TitanicThetwo;
				}
			}
		}

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position, offset); }

		inline void draw(offset_t position, cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(offset_t position, cref<camera_t> camera, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::Thetwo; }
	};

	static_assert(sizeof(thetwo_t) <= NPCSizeCap, "thetwo entity size must not exceed npc size cap!");
} // namespace necrowarp
