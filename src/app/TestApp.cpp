#include <iostream>
#include "Ray.hpp"
using namespace potato;
using namespace std;

void printVec3(Vec3f v, bool endline = true) {
	cout << "(";
	for (int i = 0; i < 3; i++) {
		printf("%.3f", v[i]);
		if (i < 2) {
			cout << ", ";
		}
	}
	cout << ")";
	if (endline) cout << endl;
}


void printRay(Ray ray, string name = "Ray") {
	cout << name << ": \n";

	cout << "\tStart:  ";
	printVec3(ray.start);

	cout << "\tDir:    ";
	printVec3(ray.direction);

	cout << endl;
}

void printMat4(Mat4d mat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << mat.cols[j][i] << ", ";
		}
		cout << endl;
	}
	cout << endl;
}

int main() {
	Mat4d a, b, c;
	a = translate<double>(1, 2, 3);
	b = scale<double>(4, 5, 6);
	c = b * a;

	printMat4(a);
	printMat4(b);
	printMat4(c);
}