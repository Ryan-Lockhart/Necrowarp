#pragma once

#include <bleak.hpp>

#include <necrowarp/globals.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct scorekeeper_t {
	  private:
		f32 score;

		inline f32 current_depth_multiplier() const noexcept {
			return map<f32>(static_cast<f32>(game_stats.game_depth),
				globals::MinimumDepth,
				globals::MaximumDepth,

				globals::MinimumDepthMultiplier,
				globals::MaximumDepthMultiplier
			);
		}

		inline f32 current_disposition_multiplier() const noexcept {
			const patron_e current_patron{ player.patron };

			if (current_patron == patron_e::None) {
				return 2.0f;
			}

			switch (get_patron_disposition(current_patron)) {
				case disposition_e::Sadistic: {
					return current_patron == patron_e::Sudoclor ? 2.0f : 1.5f;
				} case disposition_e::Apathetic: {
					return 1.0f;
				} case disposition_e::Cooperative: {
					return current_patron == patron_e::Sudoclor ? 0.25f : 0.5f;
				}
			}
		}

		inline f32 current_wave_size_multiplier() const noexcept {
			return map<f32>(static_cast<f32>(game_stats.wave_size),
				game_stats_t::MinimumWaveSize,
				game_stats_t::MaximumWaveSize,

				globals::MinimumWaveSizeMultiplier,
				globals::MaximumWaveSizeMultiplier
			);
		}

	  public:
		inline f32 current_multiplier() const noexcept {
			return std::max<f32>(
				std::numeric_limits<f32>::epsilon(),
				1.0f
					* current_depth_multiplier()
					* current_disposition_multiplier()
					* current_wave_size_multiplier()
			);
		}

		inline i32 get_score() const noexcept { return static_cast<i32>(std::ceilf(score)); }

		inline void add(f32 value) noexcept {
			value *= current_multiplier();

			if (value <= 0.0f) {
				return;
			}

			score += value;
		}

		inline void add(entity_e slain) noexcept {
			switch (slain) {
				case entity_e::Adventurer: {
					add(10.0f);
					break;
				} case entity_e::Mercenary:
				  case entity_e::Medicus:
				  case entity_e::Ranger:
				  case entity_e::Skulker: {
					add(25.0f);
					break;
				} case entity_e::Thetwo:
				  case entity_e::Mansling:
				  case entity_e::MistLady:
				  case entity_e::BannerBearer: {
					add(50.0f);
					break;
				} case entity_e::BattleMonk:
				  case entity_e::Berserker:
				  case entity_e::Hexeater:
				  case entity_e::Paladin: {
					add(100.0f);
					break;
				} default: {
					break;
				}
			}
		}

		inline void add(entity_e slain, f32 multiplier) noexcept {
			switch (slain) {
				case entity_e::Adventurer: {
					add(10.0f * multiplier);
					break;
				} case entity_e::Mercenary:
				  case entity_e::Medicus:
				  case entity_e::Ranger:
				  case entity_e::Skulker: {
					add(25.0f * multiplier);
					break;
				} case entity_e::Thetwo:
				  case entity_e::Mansling:
				  case entity_e::MistLady:
				  case entity_e::BannerBearer: {
					add(50.0f * multiplier);
					break;
				} case entity_e::BattleMonk:
				  case entity_e::Berserker:
				  case entity_e::Hexeater:
				  case entity_e::Paladin: {
					add(100.0f * multiplier);
					break;
				} default: {
					break;
				}
			}
		}

		inline void reset() noexcept { score = 0.0f; }
	} static inline scorekeeper;
} // namespace necrowarp
