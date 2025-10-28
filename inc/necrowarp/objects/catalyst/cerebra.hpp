#pragma once

#include <necrowarp/objects/object.hpp>
#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct has_unique_descriptor<cerebra_t> {
		static constexpr bool value = true;
	};

	template<> struct has_variants<cerebra_t> {
		static constexpr bool value = true;
	};

	template<> struct is_object<cerebra_t> {
		static constexpr bool value = true;
	};

	template<> struct to_object_enum<cerebra_t> {
		static constexpr object_e value = object_e::Cerebra;
	};

	template<> struct is_object_type<cerebra_t, object_e::Cerebra> {
		static constexpr bool value = true;
	};

	template<> struct to_object_type<object_e::Cerebra> {
		using type = cerebra_t;
	};

	template<> struct to_object_group<object_e::Cerebra> {
		static constexpr object_group_e value = object_group_e::Cerebra;
	};

	struct cerebra_t {
		const entity_e entity;
		const bool healthy;

		inline cerebra_t(entity_e entity, bool healthy = true) noexcept : entity{ entity }, healthy{ healthy } {}

		inline std::string to_string() const noexcept {
			return std::format("{} ({} | {})",
				necrowarp::to_string<plurality_e::Multiple>(object_e::Cerebra),
				necrowarp::to_string(entity),
				healthy ? "healthy" : "stricken"
			);
		}

		inline runes_t to_colored_string() const noexcept {
			runes_t colored_string{ necrowarp::to_colored_string<plurality_e::Multiple>(object_e::Cerebra) };

			colored_string
				.concatenate(runes_t{ " (" })
				.concatenate(necrowarp::to_colored_string(entity))
				.concatenate(runes_t{ " | " })
				.concatenate(healthy ? runes_t{ "healthy", colors::dark::Green } : runes_t{ "stricken", colors::dark::Red })
				.concatenate(runes_t{ ")" });
			
			return colored_string;
		}

		inline glyph_t current_glyph() const noexcept { return healthy ? glyphs::HealthyCerebrum : glyphs::StrickenCerebrum; }

		inline void draw(offset_t position) const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(offset_t position, offset_t offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(offset_t position, offset_t offset, offset_t nudge) const noexcept { game_atlas.draw(current_glyph(), position + offset, nudge); }

		constexpr operator object_e() const noexcept { return object_e::Cerebra; }
	};
} // namespace necrowarp
