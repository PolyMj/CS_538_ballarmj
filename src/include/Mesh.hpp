#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
#include "Line.hpp"
using namespace std;

namespace potato {
	template<typename T, typename C>
	struct Vert {
		Vec3<T> pos;
		Vec3<C> color;
	};

	template<typename T, typename C>
	struct Polygon {
		vector<Vert> vertices;
	};

	template<typename T, typename C>
	struct TriangleMesh {
		vector<Vert<T,C>> vertices;
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

	template<typename T, typename C>
	BoundBox<T> computeBounds(Polygon<T,C> poly) {
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

		// Calculate Gamma (B-A)
		Line<T,S> lineAB;
		lineAB.start = A;
		lineAB.end = B;
		barry.z = implicit(lineAB, x, y) / implicit(lineAB, C.x, C.y);

		// Calculate Beta (C-A)
		Line<T,S> lineAC;
		lineAC.start = A;
		lineAC.end = C;
		barry.y = implicit(lineAC, x, y) / implicit(lineAB, B.x, B.y);

		// Find remainder (Alpha)
		barry.x = 1 - barry.y - barry.z;

		return barry; // Bye barry
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