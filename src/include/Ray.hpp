#pragma once

#include "Vector.hpp"
#include "Settings.hpp" // For near plane and fov (VIEW_HEIGHT)

#define ZERO_VEC3 Vec3d(0.0f, 0.0f, 0.0f)
#define BASICALLY_ZERO 0.00001

namespace potato {
	struct Ray {
		Vec3d start;
		Vec3d direction;
		Vec3d color = Vec3d(1.0f, 1.0f, 1.0f);


		// Basic constructors
		Ray() = default;
		Ray(Vec3d direction, Vec3d start = ZERO_VEC3) : 
			direction(direction.normalize()), 
			start(start) {};
		
		// Create a ray that shoots from the origin to a point on the virtual screen 
		Ray(int res_x, int res_y, int p_x, int p_y) {
			double ray_x = static_cast<double>(p_x);
			double ray_y = static_cast<double>(p_y);
			
			// Offset so that center of "screen" is at origin
			ray_x -= (double)res_x/2;
			ray_y -= (double)res_y/2;

			// Rescale to acheive proper FOV
			double scale_y = VIEW_HEIGHT / res_y;
			double scale_x = scale_y * res_x / res_y;
			ray_x *= scale_x;
			ray_y *= scale_y;
			
			// Rays will start from a position on the near plane
			// Must be NEGATIVE of NEAR_PLANE since camera is facing in negative Z
			direction = Vec3d(ray_x, ray_y, -NEAR_PLANE);
			start = direction;
			direction = direction.normalize();
		};


		// Returns the reflection of the current ray off of the face defined by the given normal
		Ray reflect(Vec3d point, Vec3d normal) {
			Ray ray = Ray();
			ray.direction = direction - normal * (direction.dot(normal)/normal.dot(normal)) * 2;
			ray.start = point;
			ray.color = color;
			return ray;
		};

		// Return positiong of ray at parameter T
		Vec3d posFromT(double T) {
			return start + direction * T;
		};

		// Check what position the ray collides with some object
		// Value is a parameter for the ray
		// Any negative ==> Doesn't collide
		double collide(Vec3d point, Vec3d normal) {
			double vdn = direction.dot(normal);
			
			if (vdn == 0) return -1;

			return (point-start).dot(normal) / vdn;
		};

		bool intersectsBBox(BoundBoxd bb) {
			// For each side:
				// Get intersection point
					// t = distance / direction.axis {direction.axis != 0}
					// Intersection = direction * t {t >= 0}
				// If within bounds of side: return true

			Vec3d intersect;
			double t;
			// Check each Z plane
			if (abs(direction.z) >= BASICALLY_ZERO) {
				t = (bb.start.z-start.z) / direction.z;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 1, intersect[0], intersect[1])) {
						return true;
					}
				}
				t = (bb.end.z-start.z) / direction.z;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 1, intersect[0], intersect[1])) {
						return true;
					}
				}
			}
			// Check each Y plane
			if (abs(direction.y) >= BASICALLY_ZERO) {
				t = (bb.start.y-start.y) / direction.y;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 2, intersect[0], intersect[2])) {
						return true;
					}
				}
				t = (bb.end.y-start.y) / direction.y;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 2, intersect[0], intersect[2])) {
						return true;
					}
				}
			}
			// Check each X plane
			if (abs(direction.x) >= BASICALLY_ZERO) {
				t = (bb.start.x-start.x) / direction.x;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(1, 2, intersect[1], intersect[2])) {
						return true;
					}
				}
				t = (bb.end.x-start.x) / direction.x;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(1, 2, intersect[1], intersect[2])) {
						return true;
					}
				}
			}
			// If no intersections found
			return false;
		}
	};
}