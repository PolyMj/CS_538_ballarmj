#pragma once

#include <random>
#include "Vector.hpp"
#include "Meshd.hpp"
#include "Settings.hpp" // For near plane and fov (VIEW_HEIGHT)

#define ZERO_VEC3 Vec3d(0.0f, 0.0f, 0.0f)

namespace potato {	
	struct Ray {
		Vec3d start;
		Vec3d direction;
		Vec3d specular = Vec3d(1.0, 1.0, 1.0);
		Vec3d diffuse = Vec3d(0.0, 0.0, 0.0);


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
			double scale = VIEW_HEIGHT / res_y;
			ray_x *= scale;
			ray_y *= scale;
			
			// Rays will start from a position on the near plane
			// Must be NEGATIVE of NEAR_PLANE and ray_x since camera is facing in negative Z
			direction = Vec3d(-ray_x, ray_y, -NEAR_PLANE);
			start = direction;
			direction = direction.normalize();
		};


		// Modifies itself to become a new reflection ray (For imperfect/randomized reflections)
		void reflectSelf(Vertd v, Vec3d diffuse_scalar, Vec3d rand) {
			// Get new direction vector
			direction = direction - v.normal * (direction.dot(v.normal)/v.normal.dot(v.normal)) * 2;
			
			// Interpolate between random direction vector and reflection direction
			if (rand.dot(v.normal))
				rand = rand * -1.0;
			direction = (direction.mix(rand.normalize(), v.roughness/2.0)).normalize();


			// Moves the ray forward very slightly so it doesn't insersect with what it collided with
			start = v.pos + direction * BASICALLY_ZERO;

			// Diffuse += (diffuse color) * (remaining light being reflected) * (non-metallicity) * (scalar)
			diffuse = diffuse + (v.diffuse * specular * (1.0-v.metallicity) * diffuse_scalar);
			// Specular *= (specular color) * (metallicity)
			specular = specular * (v.specular * v.metallicity);
		};

		// Modifies itself to become a new reflection ray (For perfect reflections, e.g. mirrors)
		void reflectSelf(Vertd v, Vec3d diffuse_scalar) {
			// Get new direction vector
			direction = direction - v.normal * (direction.dot(v.normal)/v.normal.dot(v.normal)) * 2;

			// Moves the ray forward very slightly so it doesn't insersect with what it collided with
			start = v.pos + direction * BASICALLY_ZERO;

			// Diffuse += (diffuse color) * (remaining light being reflected) * (non-metallicity) * (scalar)
			diffuse = diffuse + (v.diffuse * specular * (1.0-v.metallicity) * diffuse_scalar);
			// Specular *= (specular color) * (metallicity)
			specular = specular * (v.specular * v.metallicity);
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


		// Check for each intersection the ray may have with a bounding box
		bool intersectsBBox(BoundBoxd bb, double &t1, double &t2) {
			// For each side:
				// Get intersection distance, check if negative
					// Get intersection point, check if within side's bounds
			// Assign first found collision to t1, next to t2, then return
				// If t2 is never found, t2 = -1
				// If t1 is never found, return false, set t1 = -1, t2 = -1

			Vec3d intersect;
			double t;
			bool foundIntersection = false;
			// Check each Z plane
			if (abs(direction.z) >= BASICALLY_ZERO) {
				t = (bb.start.z-start.z) / direction.z;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 1, intersect[0], intersect[1])) {
						t1 = t;
						foundIntersection = true;
					}
				}
				t = (bb.end.z-start.z) / direction.z;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 1, intersect[0], intersect[1])) {
						if (foundIntersection) {
							t2 = t;
							return true;
						}
						else {
							t1 = t;
							foundIntersection = true;
						}
					}
				}
			}
			// Check each Y plane
			if (abs(direction.y) >= BASICALLY_ZERO) {
				t = (bb.start.y-start.y) / direction.y;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 2, intersect[0], intersect[2])) {
						if (foundIntersection) {
							t2 = t;
							return true;
						}
						else {
							t1 = t;
							foundIntersection = true;
						}
					}
				}
				t = (bb.end.y-start.y) / direction.y;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(0, 2, intersect[0], intersect[2])) {
						if (foundIntersection) {
							t2 = t;
							return true;
						}
						else {
							t1 = t;
							foundIntersection = true;
						}
					}
				}
			}
			// Check each X plane
			if (abs(direction.x) >= BASICALLY_ZERO) {
				t = (bb.start.x-start.x) / direction.x;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(1, 2, intersect[1], intersect[2])) {
						if (foundIntersection) {
							t2 = t;
							return true;
						}
						else {
							t1 = t;
							foundIntersection = true;
						}
					}
				}
				t = (bb.end.x-start.x) / direction.x;
				if (t > 0) {
					intersect = posFromT(t);
					if (bb.bounds2D(1, 2, intersect[1], intersect[2])) {
						if (foundIntersection) {
							t2 = t;
							return true;
						}
						else {
							t1 = t;
							foundIntersection = true;
						}
					}
				}
			}
			
			// If only one insersection was found (somehow)
			if (foundIntersection) {
				t2 = -1.0;
				return true;
			}
			else {
				t1 = -1.0;
				t2 = -1.0;
				return false;
			}
		}
	};
}