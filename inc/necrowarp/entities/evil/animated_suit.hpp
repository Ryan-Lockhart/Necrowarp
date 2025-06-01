#pragma once

#include <necrowarp/entities/entity.hpp>
#include <necrowarp/commands/command.hpp>
#include <necrowarp/objects/object.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct globals::has_unique_descriptor<animated_suit_t> {
		static constexpr bool value = true;
	};
	
	template<> struct is_entity<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_enum<animated_suit_t> {
		static constexpr entity_e value = entity_e::AnimatedSuit;
	};

	template<> struct is_entity_type<animated_suit_t, entity_e::AnimatedSuit> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_e::AnimatedSuit> {
		using type = animated_suit_t;
	};

	template<> struct to_entity_group<entity_e::AnimatedSuit> {
		static constexpr entity_group_e value = entity_group_e::AnimatedSuit;
	};

	template<> struct is_evil_entity<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct is_combatant<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct is_bleeder<animated_suit_t> {
		static constexpr bool value = true;
	};

	template<> struct fluid_type<animated_suit_t> {
		static constexpr fluid_type_e type = fluid_type_e::Ichor;
	};

	template<> inline constexpr glyph_t entity_glyphs<animated_suit_t>{ glyphs::TwistedKnight };

	struct animated_suit_t {
		offset_t position;
		const galvanisation_e state;

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:
		static constexpr i8 MaximumHealth{ 3 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr std::array<entity_e, 4> EntityPriorities{
			entity_e::Priest,
			entity_e::Paladin,
			entity_e::Mercenary,
			entity_e::Adventurer,
		};

		inline animated_suit_t(offset_t position) noexcept : position{ position }, state{ galvanisation_e::Twisted }, health{ MaximumHealth } {}

		inline animated_suit_t(offset_t position, galvanisation_e state) noexcept : position{ position }, state{ state }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline i8 get_minimum_damage_received() const noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return 4;
				} case galvanisation_e::Shimmering: {
					return 3;
				} case galvanisation_e::Wriggling: {
					return 2;
				} case galvanisation_e::Writhing: {
					return 1;
				}
			}
		}

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(get_minimum_damage_received(), damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline i8 get_damage() const noexcept { return MaximumDamage; }

		inline i8 get_damage(entity_e target) const noexcept { return MaximumDamage; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		inline command_pack_t think() const noexcept;

		inline void die() noexcept;

		inline std::string to_string() const noexcept { return std::format("{} ({}) [{}/{}]", necrowarp::to_string(entity_e::AnimatedSuit), necrowarp::to_string(state), get_health(), max_health()); }

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string(entity_e::AnimatedSuit) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(state))
				.concatenate(runes_t{ ")" })
				.concatenate(runes_t{ std::format(" [{}/{}]", get_health(), max_health()) });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept {
			switch (state) {
				case galvanisation_e::Twisted: {
					return glyphs::TwistedSuit;
				} case galvanisation_e::Shimmering: {
					return glyphs::ShimmeringSuit;
				} case galvanisation_e::Wriggling: {
					return glyphs::WrigglingSuit;
				} case galvanisation_e::Writhing: {
					return glyphs::WrithingSuit;
				}
			}
		}

		inline void draw() const noexcept { entity_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position, offset); }

		inline void draw(cref<camera_t> camera) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, offset_t offset) const noexcept { entity_atlas.draw(current_glyph(), position + camera.get_offset(), offset); }

		constexpr operator entity_e() const noexcept { return entity_e::AnimatedSuit; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<animated_suit_t> animated_suit) noexcept { return offset_t::std_hasher::operator()(animated_suit.position); }

				static constexpr usize operator()(offset_t position) noexcept { return offset_t::std_hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<animated_suit_t> lhs, cref<animated_suit_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<animated_suit_t> lhs, offset_t rhs) noexcept { return offset_t::std_hasher::operator()(lhs.position) == offset_t::std_hasher::operator()(rhs); }

				static constexpr bool operator()(offset_t lhs, cref<animated_suit_t> rhs) noexcept { return offset_t::std_hasher::operator()(lhs) == offset_t::std_hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
