#pragma once

#include <bleak/constants/colors.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace colors {
		using namespace bleak::colors;

		namespace materials {
			using namespace bleak::colors::materials;

			static constexpr color_t Blood{ u8{ 157 }, u8{ 34 }, u8{ 53 } };
			static constexpr color_t LightBlood{ u8{ 177 }, u8{ 34 }, u8{ 53 } };
			static constexpr color_t DarkBlood{ u8{ 137 }, u8{ 14 }, u8{ 33 } };

			static constexpr color_t Ichor{ u8{ 32 }, u8{ 8 }, u8{ 32 } };
			static constexpr color_t LightIchor{ u8{ 64 }, u8{ 16 }, u8{ 64 } };
			static constexpr color_t DarkIchor{ u8{ 16 }, u8{ 4 }, u8{ 16 } };

			static constexpr color_t BloodyIchor{ mix(Blood, Ichor) };
			static constexpr color_t LightBloodyIchor{ mix(LightBlood, LightIchor) };
			static constexpr color_t DarkBloodyIchor{ mix(DarkBlood, DarkIchor) };
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
