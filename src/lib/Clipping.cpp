#include "Clipping.hpp"

namespace potato {
	// +/- x ==> right/left
	// +/- y ==> up/down
	// +/- z ==> near/far
	int getExtendedCohenSutherlandCode(Vec4f v, float left, float right, float bottom, float top, float near, float far) {
		int bitpattern = 0;
		
		if (v.x < left) {
			bitpattern += BIT_LEFT;
		}
		else if (v.x > right) {
			bitpattern += BIT_RIGHT;
		}
		
		if (v.y < bottom) {
			bitpattern += BIT_BOTTOM;
		}
		else if (v.y > top) {
			bitpattern += BIT_TOP;
		}
		
		if (v.z < far) {
			bitpattern += BIT_FAR;
		}
		else if (v.z > near) {
			bitpattern += BIT_NEAR;
		}

		return bitpattern;
	}
}