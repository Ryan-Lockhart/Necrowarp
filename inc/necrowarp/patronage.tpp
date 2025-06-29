#pragma once

#include <necrowarp/patronage.hpp>

#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	static inline void randomize_patrons() noexcept {
		static std::mt19937 generator{ std::random_device{}() };
		static std::uniform_int_distribution<i16> distribution{ -1, 1 };

		const bool force_sadism{ divine_intervention_invoked };

		divine_intervention_invoked = false;

		magic_enum::enum_for_each<patron_e>([&](auto val) -> void {
			constexpr patron_e cval{ val };

			if (force_sadism) {
				patrons<cval>.disposition = disposition_e::Sadistic;

				return;
			}

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
}
