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

        Vec3() = default;
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {};

        auto length() -> typename VecLength<T>::type {
            return sqrt(static_cast<typename VecLength<T>::type>(
                x*x + y*y + z*z
            ));
        };

        template<typename U>
        auto operator+(Vec3<U> v) const -> Vec3<decltype(T{}+U{})> {
            return {
                x + v.x,
                y + v.y,
                z + v.z
            };
        };

        friend ostream& operator<<(ostream& os, Vec3<T> v) {
            os << "(" << v.x << "," << v.y << "," << v.z << ")";
            return os;
        };

        T& operator[](int i) {
            if(i < 0 || i >= 3) 
                throw std::out_of_range(
                    "ERROR: " + to_string(i) 
                    + " out of bounds!");
            switch(i) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
            }
        };


    };

    using Vec3f = Vec3<float>;
    using Vec3i = Vec3<int>;
    using Vec3d = Vec3<double>;
};