#pragma once
#include <cmath>
#include <iostream>
#include "Vector.hpp"
#include "Buffer.hpp"
#include "Buffer.hpp"
using namespace std;

namespace potato {
    template<typename T, typename C>
    struct Line {
        Vec3<T> start {};
        Vec3<T> end {};
        Vec3<C> color {};
    };

	template<typename T>
	float implicit(Vec3<T> start, Vec3<T> end, float x, float y) {
		auto dy = start.y - end.y;
		auto dx = end.x - start.x;
		auto c1 = start.x * end.y;
		auto c2 = end.x * start.y;

		auto result = dy*x + dx*y + c1 - c2; // Essentially returns distance from line
	}

	template<typename T, typename C>
	float implicit(Line<T,C> line, float x, float y) {
		return implicit(line.start, line.end, x, y);
	}

	template<typename T, typename C>
	void drawLineDDA(Image<Vec3<C>> *image, Line<T,C> &line) {
		Vec3<T> d = line.end - line.start;
		int steps = 0;
		float xInc, yInc;
		float x = line.start.x;
		float y = line.start.y;

		if (abs(d.x > d.y)) {
			steps = abs(d.x);
		}
		else {
			steps = abs(d.y);
		}

		xInc = float(d.x) / float(steps);
		yInc = float(d.y) / float(steps);

		image->setPixel((int)round(x), (int)round(y), line.color);

		for(int k = 0; k < steps; k++) {
			x += xInc;
			y += yInc;

			image->setPixel((int)round(x), (int)round(y), line.color);
		}
	};
};
