#pragma once

#include "necrowarp/constants/enums/grimoire.hpp"
#include <bleak.hpp>

#include <magic_enum/magic_enum_switch.hpp>

#include <necrowarp/constants/enums/grimoire.tpp>

namespace necrowarp {
	using namespace bleak;

	template<grimoire_e Grimoire> struct grimoire_s {
	  private:
	  	static inline u32 uses{ 0 };

		static inline bool acquired{ false };

	  public:
	  	static constexpr bool Locked{ false };

		static inline bool can_acquire() noexcept { return !acquired && !Locked; }

		static inline bool acquire() noexcept {
			if (!can_acquire()) {
				return false;
			}

			acquired = true;

			return acquired;
		}

		static inline bool can_use() noexcept { return acquired && !Locked; }

		static inline bool use() noexcept {
			if (!can_use()) {
				return false;
			}

			++uses;

			return true;
		}
	};

	template<> inline constexpr bool grimoire_s<grimoire_e::Annihilate>::Locked{ true };
	template<> inline constexpr bool grimoire_s<grimoire_e::Repulse>::Locked{ true };

	namespace literature {
		static inline bool can_acquire(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::can_acquire();
			}, grimoire);
		}

		static inline bool acquire(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::acquire();
			}, grimoire);
		}

		static inline bool can_use(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::can_use();
			}, grimoire);
		}


		static inline bool use(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::use();
			}, grimoire);
		}
	} // namespace literature
} // namespace necrowarp
