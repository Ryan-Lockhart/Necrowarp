#pragma once

// IWYU pragma: begin_exports
#include <necrowarp/commands/unary/wander.tpp>
#include <necrowarp/commands/unary/chaotic_warp.tpp>
#include <necrowarp/commands/unary/repulse.tpp>
#include <necrowarp/commands/unary/incorporealize.tpp>
#include <necrowarp/commands/unary/necromantic_ascendance.tpp>
#include <necrowarp/commands/unary/calamitous_retaliation.tpp>
#include <necrowarp/commands/unary/mitose.tpp>
#include <necrowarp/commands/unary/eviscerate.tpp>
#include <necrowarp/commands/unary/metabolise.tpp>
#include <necrowarp/commands/unary/shed.tpp>
#include <necrowarp/commands/unary/nock.tpp>
#include <necrowarp/commands/unary/meditate.tpp>
// IWYU pragma: end_exports

namespace necrowarp {
	template struct entity_command_t<adventurer_t, wander_t>;
	template void entity_command_t<adventurer_t, wander_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, chaotic_warp_t>;
	template void entity_command_t<player_t, chaotic_warp_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, repulse_t>;
	template void entity_command_t<player_t, repulse_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, incorporealize_t>;
	template void entity_command_t<player_t, incorporealize_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, necromantic_ascendance_t>;
	template void entity_command_t<player_t, necromantic_ascendance_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<player_t, calamitous_retaliation_t>;
	template void entity_command_t<player_t, calamitous_retaliation_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<abomination_t, mitose_t>;
	template void entity_command_t<abomination_t, mitose_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<wraith_t, eviscerate_t>;
	template void entity_command_t<wraith_t, eviscerate_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<thetwo_t, metabolise_t>;
	template void entity_command_t<thetwo_t, metabolise_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<thetwo_t, shed_t>;
	template void entity_command_t<thetwo_t, shed_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<ranger_t, nock_t>;
	template void entity_command_t<ranger_t, nock_t>::process<map_type_e::Pocket>() const noexcept;

	template struct entity_command_t<battle_monk_t, meditate_t>;
	template void entity_command_t<battle_monk_t, meditate_t>::process<map_type_e::Pocket>() const noexcept;
} // namespace necrowarp
