#pragma once

#include <necrowarp/patronage.hpp>

#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<RandomEngine Generator> static inline void randomize_patrons(ref<Generator> engine) noexcept {
		static std::discrete_distribution<u16> distribution{ 16, 8, 16 };

		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			patrons<cval>.disposition = static_cast<disposition_e>(distribution(engine));
		});
	}
}
