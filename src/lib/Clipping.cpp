#include "Clipping.hpp"

namespace potato {
	// +/- x ==> right/left
	// +/- y ==> up/down
	// +/- z ==> far/near
	int getExtendedCohenSutherlandCode(Vec4<float> v, float left, float right, float bottom, float top, float near, float far) {
		int bitpattern = 0;
		
		if (-v.x + left*v.w > 0) {
			bitpattern += BIT_LEFT;
		}
		else if (v.x - right*v.w > 0) {
			bitpattern += BIT_RIGHT;
		}
		
		if (-v.y + bottom*v.w > 0) {
			bitpattern += BIT_BOTTOM;
		}
		else if (v.y - top*v.w > 0) {
			bitpattern += BIT_TOP;
		}
		
		if (-v.z + near*v.w > 0) {
			bitpattern += BIT_NEAR;
		}
		else if (v.z - far*v.w > 0) {
			bitpattern += BIT_FAR;
		}

		return bitpattern;
	}

	// Same as above but without w coordinate
	int getExtendedCohenSutherlandCode(Vec3d v, double left, double right, double bottom, double top, double near, double far) {
		int bitpattern = 0;
		
		if (-v.x + left > 0) {
			bitpattern += BIT_LEFT;
		}
		else if (v.x - right > 0) {
			bitpattern += BIT_RIGHT;
		}
		
		if (-v.y + bottom > 0) {
			bitpattern += BIT_BOTTOM;
		}
		else if (v.y - top > 0) {
			bitpattern += BIT_TOP;
		}
		
		if (-v.z + near > 0) {
			bitpattern += BIT_NEAR;
		}
		else if (v.z - far > 0) {
			bitpattern += BIT_FAR;
		}

		return bitpattern;
	}
}