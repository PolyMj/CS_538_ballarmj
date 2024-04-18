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

int main() {
	Ray original = Ray(Vec3f(-1.5, -1.3, -1.2));

	Ray reflection = original.reflect(Vec3f(0.0f,0.0f,0.0f), Vec3f(-1.0f, -1.0f, -1.0f));

	printRay(original, "Original");
	printRay(reflection, "Reflection");
}