#pragma once

#include "Vector.hpp"

#define ZR_VEC3 Vec3f(1.0f, 1.0f, 1.0f)

namespace potato {
	struct Ray {
		Vec3f start;
		Vec3f direction;
		Vec3f color = Vec3f(1.0f, 1.0f, 1.0f);


		// Basic constructors
		Ray() = default;
		Ray(Vec3f direction, Vec3f start = ZR_VEC3) : direction(direction), start(start) {};
		
		// Create a ray that shoots from the origin to a point on the virtual screen 
		Ray(float scr_height, float scr_width, float x, float y) {
			
		};


		Ray reflect(Vec4f normal) { return reflect(Vec3f(normal)); };
		Ray reflect(Vec3f normal) {
			// Returns a reflection ray
			return Ray();
		};

		// Check what position the ray collides with some object
		Vec4f collide(int some_object) {
			return Vec4f();
		};
	};
}