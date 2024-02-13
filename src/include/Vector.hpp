#pragma once
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;


namespace potato {
    template<typename T>
    struct VecLength { using type = float; };

    template<>
    struct VecLength<double> {
        using type = double;
    };

    template<typename T>
    struct Vec3 {
        union { T x{}, r, s; };
        union { T y{}, g, t; };
        union { T z{}, b, p; };

		// Simple constructors
        Vec3() = default;
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {};

		// Copy constructor
		template<typename U>
		Vec3(const Vec3<U> &other) {
			x = static_cast<U>(other.x);
			y = static_cast<U>(other.y);
			z = static_cast<U>(other.z);
		};

			// Start rithmetic overloads //
        template<typename U>
        auto operator+(const Vec3<U> &v2) const -> Vec3<decltype(T{}+U{})> {
            return {
                x + v2.x,
                y + v2.y,
                z + v2.z
            };
        };

		template<typename U>
        auto operator-(const Vec3<U> &v2) const -> Vec3<decltype(T{}-U{})> {
            return {
                x - v2.x,
                y - v2.y,
                z - v2.z
            };
        };

		template<typename U>
        auto operator*(const Vec3<U> &v2) const -> Vec3<decltype(T{}*U{})> {
            return {
                x * v2.x,
                y * v2.y,
                z * v2.z
            };
        };

		template<typename U>
        auto operator*(const U &s) const -> Vec3<decltype(T{}*U{})> {
            return {
                x * s,
                y * s,
                z * s
            };
        };

		template<typename U>
        auto operator/(const U &s) const -> Vec3<decltype(T{}*U{})> {
            return {
                x / s,
                y / s,
                z / s
            };
        };
			// End arithmetic overloads

			// Start array-esk overloads //
		T operator[](int i) const {
            switch(i) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    throw std::out_of_range(
                        "ERROR: " + to_string(i) 
                        + " out of bounds!");
            }
		}
        T& operator[](int i) { 
            switch(i) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    throw std::out_of_range(
                        "ERROR: " + to_string(i) 
                        + " out of bounds!");
            }
        };
			// End array-esk overloads //

		// For "printing" to streams
        friend ostream& operator<<(ostream& os, Vec3<T> v) {
            os << "(" << v.x << "," << v.y << "," << v.z << ")";
            return os;
        };

			// Start vector-specific operations //
		template<typename U>
		auto dot(const Vec3<U> &v2) const -> decltype(T{} * U{}) {
			return x*v2.x + y*v2.y + z*v2.z;
		};

		template<typename U>
		auto cross(const Vec3<U> &v2) const -> Vec3<decltype(T{} * U{})> {
			return {
				(y * v2.z) - (z * v2.y),
				(z * v2.x) - (x * v2.z),
				(x * v2.y) - (y * v2.x)
			};
		};

        auto length() const -> typename VecLength<T>::type {
            return sqrt(static_cast<typename VecLength<T>::type>(
                x*x + y*y + z*z
            ));
        };

		auto normalize() const -> Vec3<typename VecLength<T>::type> {
			typename VecLength<T>::type l = length();
			return {
				x / l,
				y / l,
				z / l
			};
		};
			// End vector-specific operations //
	};

	// Ease-of-use type definitions
    using Vec3f = Vec3<float>;
    using Vec3i = Vec3<int>;
    using Vec3d = Vec3<double>;
    using Vec3u = Vec3<unsigned char>;
};