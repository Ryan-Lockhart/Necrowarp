#pragma once

// IWYU pragma: begin_exports
#include <necrowarp/commands/binary/move.tpp>
#include <necrowarp/commands/binary/descend.tpp>
#include <necrowarp/commands/binary/plunge.tpp>
#include <necrowarp/commands/binary/consume.tpp>
#include <necrowarp/commands/binary/clash.tpp>
#include <necrowarp/commands/binary/devour.tpp>
#include <necrowarp/commands/binary/retrieve.tpp>
#include <necrowarp/commands/binary/loose.tpp>
#include <necrowarp/commands/binary/batter.tpp>
#include <necrowarp/commands/binary/precise_warp.tpp>
#include <necrowarp/commands/binary/consume_warp.tpp>
#include <necrowarp/commands/binary/calcify.tpp>
#include <necrowarp/commands/binary/annihilate.tpp>
#include <necrowarp/commands/binary/calcitic_invocation.tpp>
#include <necrowarp/commands/binary/spectral_invocation.tpp>
#include <necrowarp/commands/binary/sanguine_invocation.tpp>
#include <necrowarp/commands/binary/galvanic_invocation.tpp>
#include <necrowarp/commands/binary/ravenous_invocation.tpp>
#include <necrowarp/commands/binary/wretched_invocation.tpp>
#include <necrowarp/commands/binary/cerebral_invocation.tpp>
#include <necrowarp/commands/binary/infernal_invocation.tpp>
// IWYU pragma: end_exports

namespace necrowarp {
	template struct entity_command_t<adventurer_t, move_t>;
	template void entity_command_t<adventurer_t, move_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, descend_t>;
	template void entity_command_t<player_t, descend_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, plunge_t>;
	template void entity_command_t<player_t, plunge_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, consume_t>;
	template void entity_command_t<player_t, consume_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<adventurer_t, clash_t>;
	template void entity_command_t<adventurer_t, clash_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<thetwo_t, devour_t>;
	template void entity_command_t<thetwo_t, devour_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<ranger_t, retrieve_t>;
	template void entity_command_t<ranger_t, retrieve_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<ranger_t, loose_t>;
	template void entity_command_t<ranger_t, loose_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<battle_monk_t, batter_t>;
	template void entity_command_t<battle_monk_t, batter_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, precise_warp_t>;
	template void entity_command_t<player_t, precise_warp_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, consume_warp_t>;
	template void entity_command_t<player_t, consume_warp_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, calcify_t>;
	template void entity_command_t<player_t, calcify_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, annihilate_t>;
	template void entity_command_t<player_t, annihilate_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, calcitic_invocation_t>;
	template void entity_command_t<player_t, calcitic_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, spectral_invocation_t>;
	template void entity_command_t<player_t, spectral_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, sanguine_invocation_t>;
	template void entity_command_t<player_t, sanguine_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, galvanic_invocation_t>;
	template void entity_command_t<player_t, galvanic_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, ravenous_invocation_t>;
	template void entity_command_t<player_t, ravenous_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, wretched_invocation_t>;
	template void entity_command_t<player_t, wretched_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, cerebral_invocation_t>;
	template void entity_command_t<player_t, cerebral_invocation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, infernal_invocation_t>;
	template void entity_command_t<player_t, infernal_invocation_t>::process<map_type_e::Pocket>() const noexcept;
} // namespace necrowarp
