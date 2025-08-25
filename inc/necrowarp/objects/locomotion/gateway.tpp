#pragma once

#include <necrowarp/objects/locomotion/gateway.hpp>

#include <necrowarp/object_state.hpp>
#include <necrowarp/object_state.tpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Adventurer>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.50) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Adventurer>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.50) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Mercenary>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.25) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Mercenary>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.25) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Ranger>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.075) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Ranger>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.075) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Skulker>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.075) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Skulker>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.075) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Mansling>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Mansling>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::MistLady>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::MistLady>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::BannerBearer>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::BannerBearer>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Medicus>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Medicus>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::BattleMonk>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::BattleMonk>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Berserker>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Berserker>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Hexeater>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.025) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Hexeater>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.025) };

	template<> inline constexpr i16 gateway_t::MinimumStability<entity_e::Paladin>{ static_cast<i16>(globals::MinimumApproximateTideSize * 0.05) };
	template<> inline constexpr i16 gateway_t::MaximumStability<entity_e::Paladin>{ static_cast<i16>(globals::MaximumApproximateTideSize * 0.05) };

	template<RandomEngine Generator> inline constexpr i16 gateway_t::determine_stability(entity_e entity, disposition_e disposition, ref<Generator> engine) noexcept {
		const i16 multiplier{ static_cast<i16>(disposition == disposition_e::Sadistic ? 2 : 1) };

		return magic_enum::enum_switch([&](auto val) -> i16 {
			constexpr entity_e cval{ val };

			using entity_type = typename to_entity_type<cval>::type;

			if constexpr (is_good<entity_type>::value) {
				return std::uniform_int_distribution<i16>{
					static_cast<i16>(max<i16>(MinimumStability<cval>, 1) * multiplier),
					static_cast<i16>(min<i16>(MaximumStability<cval>, globals::MaximumApproximateTideSize) * multiplier)
				}(engine);
			} else {
				return 0;
			}
		}, entity);
	}

	inline gateway_t::gateway_t(entity_e entity, disposition_e disposition) noexcept :
		entity{ entity },
		idle_animation{ indices::Gateway, random_engine, true },
		initial{ determine_stability(entity, disposition, random_engine) },
		stability{ initial }
	{}

	template<map_type_e MapType> inline bool gateway_t::spawn(offset_t position) noexcept {
		if (!has_stability() || !game_map<MapType>.dependent within<region_e::Interior>(position) || game_map<MapType>[position].solid || entity_registry<MapType>.contains(position)) {
			return false;
		}

		const bool spawned{
			magic_enum::enum_switch([&](auto val) -> bool {
				constexpr entity_e cval{ val };

				using entity_type = typename to_entity_type<cval>::type;

				if constexpr (is_good<entity_type>::value) {
					if constexpr (is_same<entity_type, paladin_t>::value) {
						return entity_registry<MapType>.add(position, entity_type{ random_engine });
					} else {
						return entity_registry<MapType>.add(position, entity_type{});
					}
				} else {
					return false;
				}
			}, entity)
		};

		if (spawned) {
			erode();
		}

		return spawned;
	}
} // namespace necrowarp
