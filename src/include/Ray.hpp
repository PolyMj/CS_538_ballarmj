#pragma once

#include "Vector.hpp"
#include "Settings.hpp" // For near plane and fov (VIEW_HEIGHT)

#define ZERO_VEC3 Vec3f(0.0f, 0.0f, 0.0f)

namespace potato {
	struct Ray {
		Vec3f start;
		Vec3f direction;
		Vec3f color = Vec3f(1.0f, 1.0f, 1.0f);


		// Basic constructors
		Ray() = default;
		Ray(Vec3f direction, Vec3f start = ZERO_VEC3) : 
			direction(direction.normalize()), 
			start(start) {};
		
		// Create a ray that shoots from the origin to a point on the virtual screen 
		Ray(int res_x, int res_y, int p_x, int p_y) {
			float ray_x = static_cast<float>(p_x);
			float ray_y = static_cast<float>(p_y);
			
			// Offset so that center of "screen" is at origin
			ray_x -= (float)res_x/2;
			ray_y -= (float)res_y/2;

			// Rescale to acheive proper FOV
			float scale_y = VIEW_HEIGHT / res_y;
			float scale_x = scale_y * res_x / res_y;
			ray_x *= scale_x;
			ray_y *= scale_y;
			
			// Rays will start from a position on the near plane
			// Must be NEGATIVE of NEAR_PLANE since camera is facing in negative Z
			direction = Vec3f(ray_x, ray_y, -NEAR_PLANE);
			start = direction;
			direction = direction.normalize();
		};


		// Returns the reflection of the current ray off of the face defined by the given normal
		Ray reflect(Vec3f point, Vec3f normal) {
			Ray ray = Ray();
			ray.direction = direction - normal * (direction.dot(normal)/normal.dot(normal)) * 2;
			ray.start = point;
			ray.color = color;
			return ray;
		};

		// Return positiong of ray at parameter T
		Vec3f posFromT(float T) {
			return start + direction * T;
		};

		// Check what position the ray collides with some object
		// Value is a parameter for the ray
		// Any negative ==> Doesn't collide
		float collide(Vec3f point, Vec3f normal) {
			float vdn = direction.dot(normal);
			
			if (vdn == 0) return -1;

			return (point-start).dot(normal) / vdn;
		};
	};
}