#pragma once

#include <bleak.hpp>

#include <optional>

#include <magic_enum/magic_enum_switch.hpp>
#include <magic_enum/magic_enum_utility.hpp>

#include <necrowarp/patronage.hpp>

#include <necrowarp/constants/enums/grimoire.tpp>
#include <necrowarp/constants/enums/shackle.tpp>

namespace necrowarp {
	using namespace bleak;

	template<shackle_e Shackle> constexpr grimoire_e shackle_breaker{};

	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Calcitic>{ grimoire_e::CalciticInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Spectral>{ grimoire_e::SpectralInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Sanguine>{ grimoire_e::SanguineInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Galvanic>{ grimoire_e::GalvanicInvocation };

	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Ravenous>{ grimoire_e::RavenousInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Wretched>{ grimoire_e::WretchedInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Cerebral>{ grimoire_e::CerebralInvocation };
	template<> inline constexpr grimoire_e shackle_breaker<shackle_e::Infernal>{ grimoire_e::InfernalInvocation };

	template<grimoire_e Grimoire> struct grimoire_s {
	  private:
	  	static inline u32 uses{ 0 };

		static inline bool acquired{ false };

	  public:
	  	static constexpr bool Locked{ false };

		static inline u32 get_uses() noexcept { return uses; }

		static inline bool is_acquired() noexcept { return acquired; }

		static inline bool can_acquire() noexcept { return !Locked && !acquired; }

		static inline bool acquire() noexcept {
			if (!can_acquire()) {
				return false;
			}

			acquired = true;

			return acquired;
		}

		static inline bool can_use() noexcept { return !Locked && acquired; }

		static inline bool use() noexcept {
			if (!can_use()) {
				return false;
			}

			++uses;

			return true;
		}

		static inline void reset() noexcept { uses = 0; acquired = false; }
	};

	template<patron_e Patron> static constexpr auto patron_dowry = std::array<grimoire_e, 0>{};

	template<> inline constexpr std::array<grimoire_e, 2> patron_dowry<patron_e::None>{
		grimoire_e::ChaoticWarp,
		grimoire_e::PreciseWarp,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Kalypdrot>{
		grimoire_e::PreciseWarp,
		grimoire_e::Calcify,
		grimoire_e::CalciticInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Akurakhaithan>{
		grimoire_e::PreciseWarp,
		grimoire_e::Incorporealize,
		grimoire_e::SpectralInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Merirfin>{
		grimoire_e::PreciseWarp,
		grimoire_e::Annihilate,
		grimoire_e::SanguineInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Praethornyn>{
		grimoire_e::Annihilate,
		grimoire_e::SanguineInvocation,
		grimoire_e::GalvanicInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Tsendikor>{
		grimoire_e::ChaoticWarp,
		grimoire_e::RavenousInvocation,
		grimoire_e::WretchedInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Ionna>{
		grimoire_e::ChaoticWarp,
		grimoire_e::CalciticInvocation,
		grimoire_e::WretchedInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Rathghul>{
		grimoire_e::Repulse,
		grimoire_e::Incorporealize,
		grimoire_e::CerebralInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 4> patron_dowry<patron_e::Moslager>{
		grimoire_e::ChaoticWarp,
		grimoire_e::PreciseWarp,
		grimoire_e::CerebralInvocation,
		grimoire_e::InfernalInvocation,
	};

	template<> inline constexpr std::array<grimoire_e, 3> patron_dowry<patron_e::Sudoclor>{
		grimoire_e::ChaoticWarp,
		grimoire_e::NecromanticAscendance,
		grimoire_e::CalamitousRetaliation,
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

		template<map_type_e MapType> static inline bool is_softlocked() noexcept;
	} // namespace literature
} // namespace necrowarp
