#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	struct tribulation_s {
	  private:
		static constexpr f16 MinimumTrapChance{ 0.0125f };
		static constexpr f16 MaximumTrapChance{ 0.5000f };

		static constexpr f16 TrapGrowthRate{ 2.0f };

		static inline usize trap_aggravations{ 0 };

		static inline f16 current_trap_chance() noexcept { return clamp<f16>(static_cast<f16>(trap_aggravations * MinimumTrapChance * TrapGrowthRate), MinimumTrapChance, MaximumTrapChance); }

		static inline std::bernoulli_distribution trap_dis{ current_trap_chance() };

	  public:
		template<RandomEngine Generator> static inline bool aggravate(ref<Generator> engine) noexcept {
			const bool triggered{ trap_dis(engine) };

			if (triggered) {
				trap_aggravations = 0;
			} else {
				++trap_aggravations;
			}

			trap_dis = std::bernoulli_distribution{ current_trap_chance() };

			return triggered;
		}
	};
} // namespace necrowarp
