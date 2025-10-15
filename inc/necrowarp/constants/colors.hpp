#pragma once

#include <bleak/constants/colors.hpp>

#include <necrowarp/constants/enums/fluid.hpp>
#include <necrowarp/constants/enums/shackle.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace colors {
		using namespace bleak::colors;

		namespace materials {
			using namespace bleak::colors::materials;

			template<fluid_e Fluid> static constexpr color_t Fluids;

			template<> inline constexpr color_t Fluids<fluid_e::Blood>{ u8{ 122 }, u8{ 3 }, u8{ 10 } };

			template<> inline constexpr color_t Fluids<fluid_e::Ichor>{ u8{ 51 }, u8{ 39 }, u8{ 78 } };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyIchor>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>) };
			
			template<> inline constexpr color_t Fluids<fluid_e::Filth>{ u8{ 138 }, u8{ 138 }, u8{ 0 } };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>) };
			template<> inline constexpr color_t Fluids<fluid_e::IchorousFilth>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

			template<> inline constexpr color_t Fluids<fluid_e::Ectoplasm>{ u8{ 212 }, u8{ 189 }, u8{ 255 } };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ectoplasm>) };
			template<> inline constexpr color_t Fluids<fluid_e::IchorousEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
			template<> inline constexpr color_t Fluids<fluid_e::FilthyEctoplasm>{ mix(Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
			template<> inline constexpr color_t Fluids<fluid_e::BloodyFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };
			template<> inline constexpr color_t Fluids<fluid_e::IchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

			template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

			namespace light {
				template<fluid_e Fluid> static constexpr color_t Fluids;

				template<> inline constexpr color_t Fluids<fluid_e::Blood>{ u8{ 146 }, u8{ 6 }, u8{ 6 } };

				template<> inline constexpr color_t Fluids<fluid_e::Ichor>{ u8{ 69 }, u8{ 56 }, u8{ 99 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchor>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>) };
			
				template<> inline constexpr color_t Fluids<fluid_e::Filth>{ u8{ 178 }, u8{ 170 }, u8{ 8 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousFilth>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

				template<> inline constexpr color_t Fluids<fluid_e::Ectoplasm>{ u8{ 232 }, u8{ 212 }, u8{ 255 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::FilthyEctoplasm>{ mix(Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::BloodyFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };
			}

			namespace dark {
				template<fluid_e Fluid> static constexpr color_t Fluids;

				template<> inline constexpr color_t Fluids<fluid_e::Blood>{ u8{ 98 }, u8{ 0 }, u8{ 15 } };

				template<> inline constexpr color_t Fluids<fluid_e::Ichor>{ u8{ 32 }, u8{ 21 }, u8{ 56 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchor>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>) };
			
				template<> inline constexpr color_t Fluids<fluid_e::Filth>{ u8{ 80 }, u8{ 64 }, u8{ 16 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousFilth>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilth>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>) };

				template<> inline constexpr color_t Fluids<fluid_e::Ectoplasm>{ u8{ 156 }, u8{ 131 }, u8{ 204 } };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::FilthyEctoplasm>{ mix(Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::BloodyFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };
				template<> inline constexpr color_t Fluids<fluid_e::IchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };

				template<> inline constexpr color_t Fluids<fluid_e::BloodyIchorousFilthyEctoplasm>{ mix(Fluids<fluid_e::Blood>, Fluids<fluid_e::Ichor>, Fluids<fluid_e::Filth>, Fluids<fluid_e::Ectoplasm>) };
			}
		} // namespace materials

		namespace metals {
			static constexpr color_t Iron{ u8{ 58 }, u8{ 60 }, u8{ 64 } };
			static constexpr color_t Steel{ u8{ 113 }, u8{ 121 }, u8{ 126 } };

			static constexpr color_t Gold{ u8{ 255 }, u8{ 215 }, u8{ 0 } };
			static constexpr color_t Silver{ u8{ 192 }, u8{ 192 }, u8{ 192 } };
			static constexpr color_t Copper{ u8{ 184 }, u8{ 115 }, u8{ 51 } };

			static constexpr color_t Tin{ u8{ 145 }, u8{ 145 }, u8{ 145 } };
			static constexpr color_t Bronze{ u8{ 205 }, u8{ 127 }, u8{ 50 } };

			static constexpr color_t Zinc{ u8{ 186 }, u8{ 196 }, u8{ 200 } };
			static constexpr color_t Brass{ u8{ 181 }, u8{ 166 }, u8{ 66 } };

			template<shackle_e Shackle> static constexpr color_t Shackles;

			template<> inline constexpr color_t Shackles<shackle_e::Unshackled>{ colors::light::Intrite };
			template<> inline constexpr color_t Shackles<shackle_e::Calcitic>{ u8{ 255 }, u8{ 255 }, u8{ 240 } };
			template<> inline constexpr color_t Shackles<shackle_e::Spectral>{ u8{  72 }, u8{ 100 }, u8{  83 } };
			template<> inline constexpr color_t Shackles<shackle_e::Sanguine>{ u8{ 157 }, u8{  34 }, u8{  53 } };
			template<> inline constexpr color_t Shackles<shackle_e::Galvanic>{ u8{  42 }, u8{  74 }, u8{ 110 } };
			template<> inline constexpr color_t Shackles<shackle_e::Ravenous>{ u8{  72 }, u8{ 100 }, u8{  83 } };
			template<> inline constexpr color_t Shackles<shackle_e::Wretched>{ u8{ 157 }, u8{  34 }, u8{  53 } };
			template<> inline constexpr color_t Shackles<shackle_e::Cerebral>{ u8{ 255 }, u8{ 129 }, u8{ 234 } };
			template<> inline constexpr color_t Shackles<shackle_e::Infernal>{ u8{ 106 }, u8{  27 }, u8{  54 } };
		} // namespace metals
	} // namespace colors
} // namespace necrowarp
