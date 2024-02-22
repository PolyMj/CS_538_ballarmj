#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
#include "Line.hpp"
using namespace std;

namespace potato {
	
	struct Vert {
		Vec3f pos;
		Vec4f color;
	};

	struct Polygon {
		vector<Vert> vertices;
	};

	struct TriangleMesh {
		vector<Vert> vertices;
		vector<unsigned int> indices;
	};


	template<typename T>
	struct BoundBox {
		Vec3<T> start;
		Vec3<T> end;
	};

	template<typename T>
	Vec3<T> min(Vec3<T> &a, Vec3<T> &b) {
		Vec3<T> c;
		c.x = std::min(a.x, b.x)
		c.y = std::min(a.y, b.y);
		c.z = std::min(a.z, b.z)
		return c;
	}

	template<typename T>
	Vec3<T> max(Vec3<T> &a, Vec3<T> &b) {
		Vec3<T> c;
		c.x = std::max(a.x, b.x)
		c.y = std::max(a.y, b.y);
		c.z = std::max(a.z, b.z)
		return c;
	}

	template<typename T>
	BoundBox<T> computeBounds(Polygon poly) {
		BoundBox<T> box;

		if (poly.vertices.size() > 1) {
			box.start = poly.vertices.at(0);
			box.end = poly.vertices.at(0);
		}
		else {
			box.start = Vec3<T>(0,0,0);
			box.end = box.start;
			return box;
		}

		for (int i = 1; i < poly.vertices.size(); i++) {
			Vec3<T> v = poly.vertices.at(i).pos;
			box.start = min(box.start, v);
			box.end = min(box.end, v);
		}

		return box;
	}

	template<typename T>
	BoundBox<T> computeBounds(Vec3<T> A, Vec3<T> B, Vec3<T> C) {
		BoundBox<T> box;

		box.start = min(A, min(B,C));
		box.end = max(A, max(B,C));

		return box;
	}

	template<typename T, typename S>
	Vec3<float> barycentric(Vec3<T> &A, Vec3<T> &B, Vec3<T> &C, float x, float y) {		
		Vec3<float> barry; // Bro thinks he's the flash
		ImplicitLine<float> AB(A,B);
		ImplicitLine<float> AC(A,C);

		return barycentric(AB, AC, x, y); // Bye barry
	}

	template<typename T, typename S>
	Vec3<float> barycentric(ImplicitLine<float> &AB, ImplicitLine<float> AC, float x, float y) {		
		Vec3<float> barry; // Bro thinks he's the flash
		
		barry.z = AB.eval(x, y) / AB.eval(C.x, C.y);
		barry.y = AB.eval(x, y) / AB.eval(B.x, B.y);
		barry.x = 1.0 - barry.y - barry.z;

		return barry; // Bye barry
	}

	template<typename T>
	void calculateTriangleBarycentric(Vec3<T> A, Vec3<T> B, Vec3<T> C, vector<Vec3f> &allBarry) {
		BoundBox<T> bb = computeBounds(A,B,C);
		int sy = floor(bb.start.y);
		int ey = ciel(bb.end.y);
		int sx = floor(bb.start.x);
		int ex = ciel(bb.end.x);

		for (int y = sy, y <= ey; y++) {
			for (int x = sx; x <= ex; x++) {
				Vec3<float> barry = barycentric(A,B,C,float(x),float(y));
				if (barry.z > 0 && bary.y > 0 && bary.x > 0) {
					allBarry.push_back(barry);
				}
			}
		}
	}

	template<typename T, typename S>
	void fillTraingle(Image<Vec3<S>> *image, Vec3<T> &A, Vec3<T> &B, Vec3<T> &C) {
		BoundBox<T> bb = computeBounds(A,B,C);
		for(auto y = bb.start.y; y <= bb; y+=1) {
			for (auto x = bb.start.x; x <= bb.x; x+=1) {
				Vec3<float> barry = barycentric(A,B,C,x,y);
				if (bary.x > 0 && bary.y > 0 && bary.z > 0) {
					Vec3<S> Color = Vec3<S>(
						static_cast<S>(255.0*barry.x);
						static_cast<S>(255.0*barry.y);
						static_cast<S>(255.0*barry.z);
					)
					image->setPixel(static_cast<int>(x), static_cast<int>(y), color);
				}
			}
		}
	}

}