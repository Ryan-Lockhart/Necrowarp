#pragma once

#include <bleak.hpp>

#include <optional>

#include <magic_enum/magic_enum_switch.hpp>
#include <magic_enum/magic_enum_utility.hpp>

#include <necrowarp/patronage.hpp>

#include <necrowarp/constants/enums/grimoire.tpp>

namespace necrowarp {
	using namespace bleak;

	template<grimoire_e Grimoire> struct grimoire_s {
	  private:
	  	static inline u32 uses{ 0 };

		static inline bool acquired{ false };

	  public:
	  	static constexpr bool Locked{ false };

		static inline u32 get_uses() noexcept { return uses; }

		static inline bool is_acquired() noexcept { return acquired; }

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

		static inline void reset() noexcept { uses = 0; acquired = false; }
	};

	//template<> inline constexpr bool grimoire_s<grimoire_e::Repulse>::Locked{ true };

	template<patron_e Patron> static constexpr std::array<grimoire_e, 2> patron_dowry{};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::None>{
		grimoire_e::ChaoticWarp,
		grimoire_e::PreciseWarp,
	};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::Rathghul>{
		grimoire_e::ChaoticWarp,
		grimoire_e::CalciticInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::Akurakhaithan>{
		grimoire_e::ChaoticWarp,
		grimoire_e::SpectralInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::Merirfin>{
		grimoire_e::ChaoticWarp,
		grimoire_e::SanguineInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::Saeiligarkeuss>{
		grimoire_e::ChaoticWarp,
		grimoire_e::NecromanticAscendance,
	};

	namespace literature {
		static inline u32 get_uses(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> u32 {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::get_uses();
			}, grimoire);
		}

		static inline bool is_acquired(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> bool {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::is_acquired();
			}, grimoire);
		}

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

		static inline void acquire() noexcept {
			magic_enum::enum_for_each<grimoire_e>([&](auto val) {
				constexpr grimoire_e cval{ val };

				grimoire_s<cval>::acquire();
			});
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

		static inline void reset(grimoire_e grimoire) noexcept {
			return magic_enum::enum_switch([&](auto val) -> void {
				constexpr grimoire_e cval{ val };

				return grimoire_s<cval>::reset();
			}, grimoire);
		}

		static inline void reset() noexcept {
			magic_enum::enum_for_each<grimoire_e>([&](auto val) {
				constexpr grimoire_e cval{ val };

				grimoire_s<cval>::reset();
			});
		}

		static inline void receive_dowry(patron_e patron) noexcept {
			magic_enum::enum_switch([&](auto val) -> void {
				constexpr patron_e cval{ val };

				for (cauto grimoire : patron_dowry<cval>) {
					acquire(grimoire);
				}
			}, patron);
		}

		template<RandomEngine Generator> static inline std::optional<grimoire_e> random_unacquired_grimoire(ref<Generator> engine) noexcept {
			static std::uniform_int_distribution<usize> grimoire_distribution{
				static_cast<usize>(grimoire_e::ChaoticWarp),
				static_cast<usize>(grimoire_e::CalamitousRetaliation)
			};

			static constexpr usize max_tries{ (static_cast<usize>(grimoire_e::CalamitousRetaliation) + 1) * 2 };

			for (usize tries{ 0 }; tries < max_tries; ++tries) {
				const grimoire_e selected{ static_cast<grimoire_e>(grimoire_distribution(engine)) };

				if (!can_acquire(selected)) {
					continue;
				}

				return selected;
			}

			return std::nullopt;
		}

		template<RandomEngine Generator> static inline std::optional<grimoire_e> random_unacquired_invocation(ref<Generator> engine) noexcept {
			static std::uniform_int_distribution<usize> grimoire_distribution{
				static_cast<usize>(grimoire_e::CalciticInvocation),
				static_cast<usize>(grimoire_e::InfernalInvocation)
			};

			static constexpr usize max_tries{ (static_cast<usize>(grimoire_e::InfernalInvocation) - static_cast<usize>(grimoire_e::CalciticInvocation) + 1) * 2 };

			for (usize tries{ 0 }; tries < max_tries; ++tries) {
				const grimoire_e selected{ static_cast<grimoire_e>(grimoire_distribution(engine)) };

				if (!can_acquire(selected) || !is_invocation(selected)) {
					continue;
				}

				return selected;
			}

			return std::nullopt;
		}

		template<RandomEngine Generator> static inline std::optional<grimoire_e> random_unacquired_spell(ref<Generator> engine) noexcept {
			static std::uniform_int_distribution<usize> lower_distribution{
				static_cast<usize>(grimoire_e::ChaoticWarp),
				static_cast<usize>(grimoire_e::Incorporealize)
			};

			static std::uniform_int_distribution<usize> upper_distribution{
				static_cast<usize>(grimoire_e::NecromanticAscendance),
				static_cast<usize>(grimoire_e::CalamitousRetaliation)
			};

			static constexpr usize max_tries{
				(static_cast<usize>(grimoire_e::CalamitousRetaliation) - static_cast<usize>(grimoire_e::NecromanticAscendance) + 1) +
				(static_cast<usize>(grimoire_e::Incorporealize) - static_cast<usize>(grimoire_e::ChaoticWarp) + 1)
			};

			for (usize tries{ 0 }; tries < max_tries; ++tries) {
				const grimoire_e lower_selected{ static_cast<grimoire_e>(lower_distribution(engine)) };

				if (can_acquire(lower_selected) && is_spell(lower_selected)) {
					return lower_selected;
				}

				const grimoire_e upper_selected{ static_cast<grimoire_e>(upper_distribution(engine)) };

				if (can_acquire(upper_selected) && is_spell(upper_selected)) {
					return upper_selected;
				}
			}

			return std::nullopt;
		}
	} // namespace literature
} // namespace necrowarp
