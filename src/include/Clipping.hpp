#pragma once

#include "Mesh.hpp"

namespace potato {
	#define BIT_LEFT 1
	#define BIT_RIGHT 2
	#define BIT_BOTTOM 4
	#define BIT_TOP 8
	#define BIT_NEAR 16
	#define BIT_FAR 32

	int getExtendedCohenSutherlandCode(Vec4f v, float left, float right, float bottom, float top, float near, float far);
}