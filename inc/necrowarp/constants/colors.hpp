#pragma once

#include <bleak/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace colors {
		using namespace bleak::colors;

		namespace materials {
			using namespace bleak::colors::materials;

			static constexpr color_t Blood{ u8{ 122 }, u8{ 3 }, u8{ 10 } };

			static constexpr color_t Ichor{ u8{ 51 }, u8{ 39 }, u8{ 78 } };

			static constexpr color_t BloodyIchor{ mix(Blood, Ichor) };
			
			static constexpr color_t Filth{ u8{ 138 }, u8{ 138 }, u8{ 0 } };

			static constexpr color_t BloodyFilth{ mix(Blood, Filth) };
			static constexpr color_t IchorousFilth{ mix(Ichor, Filth) };

			static constexpr color_t BloodyIchorousFilth{ mix(Blood, Ichor, Filth) };

			static constexpr color_t Ectoplasm{ u8{ 212 }, u8{ 189 }, u8{ 255 } };

			static constexpr color_t BloodyEctoplasm{ mix(Blood, Ectoplasm) };
			static constexpr color_t IchorousEctoplasm{ mix(Ichor, Ectoplasm) };
			static constexpr color_t FilthyEctoplasm{ mix(Filth, Ectoplasm) };

			static constexpr color_t BloodyIchorousEctoplasm{ mix(Blood, Ichor, Ectoplasm) };
			static constexpr color_t BloodyFilthyEctoplasm{ mix(Blood, Filth, Ectoplasm) };
			static constexpr color_t IchorousFilthyEctoplasm{ mix(Ichor, Filth, Ectoplasm) };

			static constexpr color_t BloodyIchorousFilthyEctoplasm{ mix(Blood, Ichor, Filth, Ectoplasm) };

			namespace light {
				static constexpr color_t Blood{ u8{ 146 }, u8{ 6 }, u8{ 6 } };

				static constexpr color_t Ichor{ u8{ 69 }, u8{ 56 }, u8{ 99 } };

				static constexpr color_t BloodyIchor{ mix(Blood, Ichor) };
			
				static constexpr color_t Filth{ u8{ 178 }, u8{ 170 }, u8{ 8 } };

				static constexpr color_t BloodyFilth{ mix(Blood, Filth) };
				static constexpr color_t IchorousFilth{ mix(Ichor, Filth) };

				static constexpr color_t BloodyIchorousFilth{ mix(Blood, Ichor, Filth) };

				static constexpr color_t Ectoplasm{ u8{ 232 }, u8{ 212 }, u8{ 255 } };

				static constexpr color_t BloodyEctoplasm{ mix(Blood, Ectoplasm) };
				static constexpr color_t IchorousEctoplasm{ mix(Ichor, Ectoplasm) };
				static constexpr color_t FilthyEctoplasm{ mix(Filth, Ectoplasm) };

				static constexpr color_t BloodyIchorousEctoplasm{ mix(Blood, Ichor, Ectoplasm) };
				static constexpr color_t BloodyFilthyEctoplasm{ mix(Blood, Filth, Ectoplasm) };
				static constexpr color_t IchorousFilthyEctoplasm{ mix(Ichor, Filth, Ectoplasm) };

				static constexpr color_t BloodyIchorousFilthyEctoplasm{ mix(Blood, Ichor, Filth, Ectoplasm) };
			}

			namespace dark {
				static constexpr color_t Blood{ u8{ 98 }, u8{ 0 }, u8{ 15 } };

				static constexpr color_t Ichor{ u8{ 32 }, u8{ 21 }, u8{ 56 } };

				static constexpr color_t BloodyIchor{ mix(Blood, Ichor) };
			
				static constexpr color_t Filth{ u8{ 80 }, u8{ 64 }, u8{ 16 } };

				static constexpr color_t BloodyFilth{ mix(Blood, Filth) };
				static constexpr color_t IchorousFilth{ mix(Ichor, Filth) };

				static constexpr color_t BloodyIchorousFilth{ mix(Blood, Ichor, Filth) };

				static constexpr color_t Ectoplasm{ u8{ 156 }, u8{ 131 }, u8{ 204 } };

				static constexpr color_t BloodyEctoplasm{ mix(Blood, Ectoplasm) };
				static constexpr color_t IchorousEctoplasm{ mix(Ichor, Ectoplasm) };
				static constexpr color_t FilthyEctoplasm{ mix(Filth, Ectoplasm) };

				static constexpr color_t BloodyIchorousEctoplasm{ mix(Blood, Ichor, Ectoplasm) };
				static constexpr color_t BloodyFilthyEctoplasm{ mix(Blood, Filth, Ectoplasm) };
				static constexpr color_t IchorousFilthyEctoplasm{ mix(Ichor, Filth, Ectoplasm) };

				static constexpr color_t BloodyIchorousFilthyEctoplasm{ mix(Blood, Ichor, Filth, Ectoplasm) };
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

			namespace shackles {
				static constexpr color_t Calcitic{ u8{ 255 }, u8{ 255 }, u8{ 240 } };
				static constexpr color_t Spectral{ u8{  72 }, u8{ 100 }, u8{  83 } };
				static constexpr color_t Sanguine{ u8{ 157 }, u8{  34 }, u8{  53 } };
				static constexpr color_t Galvanic{ u8{  42 }, u8{  74 }, u8{ 110 } };
				static constexpr color_t Ravenous{ u8{  72 }, u8{ 100 }, u8{  83 } };
				static constexpr color_t Wretched{ u8{ 157 }, u8{  34 }, u8{  53 } };
				static constexpr color_t Abyssal{ u8{  42 }, u8{  74 }, u8{ 110 } };
			} // namespace shackles
		} // namespace metals
	} // namespace colors
} // namespace necrowarp
