#pragma once 
#include <cmath> 
#include <string> 
using namespace std; 
 
namespace potato { 
 
    template<typename T> struct Vec3; 
 
    /////////////////////////////////////////////////////////////////////////// 
    // VECTOR LENGTH 
    /////////////////////////////////////////////////////////////////////////// 
 
    template<typename T> 
    struct VecLength { using type = float; }; 
 
    template<> 
    struct VecLength<double> { using type = double; }; 
 
    /////////////////////////////////////////////////////////////////////////// 
    // VEC4 
    /////////////////////////////////////////////////////////////////////////// 
    template<typename T> 
    struct Vec4 { 
        union { T x{}, r, s; }; 
        union { T y{}, g, t; }; 
        union { T z{}, b, p; }; 
        union { T w{}, a, q; }; 
 
        Vec4() = default; 
        Vec4(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}; 
        Vec4(Vec3<T> v, T w): x(v.x), y(v.y), z(v.z), w(w) {}; 
 
        void copyFrom(const Vec4<T> &other) { 
            x = other.x; 
            y = other.y; 
            z = other.z; 
            w = other.w; 
        }; 
 
        template<typename U> 
        auto operator+(Vec4<U> v2) const 
        -> Vec4<decltype(T{} + U{})> 
        { 
            return { 
                x + v2.x, 
                y + v2.y, 
                z + v2.z, 
                w + v2.w 
            }; 
        }; 
 
        template<typename U> 
        auto operator-(Vec4<U> v2) const 
        -> Vec4<decltype(T{} - U{})> 
        { 
            return { 
                x - v2.x, 
                y - v2.y, 
                z - v2.z, 
                w - v2.w 
            }; 
        }; 
 
        template<typename U> 
        auto operator*(Vec4<U> v2) const 
        -> Vec4<decltype(T{} * U{})> 
        { 
            return { 
                x * v2.x, 
                y * v2.y, 
                z * v2.z, 
                w * v2.w 
            }; 
        }; 
 
        template<typename U> 
        auto operator*(U s) const 
        -> Vec4<decltype(T{} * U{})> 
        { 
            return { 
                x * s, 
                y * s, 
                z * s, 
                w * s 
            }; 
        }; 
 
        template<typename U> 
        auto operator/(U s) const 
        -> Vec4<decltype(T{} / U{})> 
        { 
            return { 
                x / s, 
                y / s, 
                z / s, 
                w / s 
            }; 
        }; 
 
        // Allow reading of values 
        T operator[](int i) const { 
            switch(i) 
            { 
                case 0: 
                    return x; 
                case 1: 
                    return y; 
                case 2: 
                    return z; 
                case 3: 
                    return w; 
                default: 
                    throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!"); 
            } 
        }; 
 
        // Allows writing to values 
        T &operator[](int i) { 
            switch(i) 
            { 
                case 0: 
                    return x; 
                case 1: 
                    return y; 
                case 2: 
                    return z; 
                case 3: 
                    return w; 
                default: 
                    throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!"); 
            } 
        }; 
 
        // Override cout 
        ostream& operator<<(ostream& os) { 
            os << "(" << x << "," << y << "," << z << "," << w << ")"; 
            return os; 
        }; 
 
        // Dot 
        template<typename U> 
        auto dot(Vec4<U> v2) const 
        -> decltype(T{} * U{}) 
        { 
            return (x * v2.x + y * v2.y + z * v2.z + w * v2.w); 
        }; 
 
        // Length 
        auto length() -> typename VecLength<T>::type { 
            return sqrt(static_cast<typename VecLength<T>::type>(x*x + y*y + z*z + w*w)); 
        }; 
 
        // Normalize 
        auto normalize() -> typename VecLength<T>::type { 
            auto len = length(); 
            return { 
                x / len, 
                y / len, 
                z / len 
            }; 
        }; 
    }; 
 
    using Vec4f = Vec4<float>; 
    using Vec4i = Vec4<int>; 
    using Vec4d = Vec4<double>; 
    using Vec4u = Vec4<unsigned char>; 
 
    /////////////////////////////////////////////////////////////////////////// 
    // VEC3 
    /////////////////////////////////////////////////////////////////////////// 
    template<typename T> 
    struct Vec3 { 
        union { T x{}, r, s; }; 
        union { T y{}, g, t; }; 
        union { T z{}, b, p; }; 
 
        Vec3() = default; 
        Vec3(T x, T y, T z): x(x), y(y), z(z) {}; 
 
        template<typename U> 
        Vec3(const Vec3<U> &other) : 
            x(static_cast<T>(other.x)), 
            y(static_cast<T>(other.y)), 
            z(static_cast<T>(other.z)) {}; 
 
        // Add in assignment 03 
        template<typename U> 
        Vec3(const Vec4<U> &other) : 
            x(static_cast<T>(other.x)), 
            y(static_cast<T>(other.y)), 
            z(static_cast<T>(other.z)) {}; 
 
        void copyFrom(const Vec3<T> &other) { 
            x = other.x; 
            y = other.y; 
            z = other.z; 
        }; 
 
        template<typename U> 
        auto operator+(const Vec3<U> &v2) const 
        -> Vec3<decltype(T{} + U{})> 
        { 
            return { 
                x + v2.x, 
                y + v2.y, 
                z + v2.z 
            }; 
        }; 
 
        template<typename U> 
        auto operator+(const U &s) const 
        -> Vec3<decltype(T{} + U{})> 
        { 
            return { 
                x + s, 
                y + s, 
                z + s 
            }; 
        }; 
 
        template<typename U> 
        auto operator-(const Vec3<U> &v2) const 
        -> Vec3<decltype(T{} - U{})> 
        { 
            return { 
                x - v2.x, 
                y - v2.y, 
                z - v2.z 
            }; 
        }; 
 
        template<typename U> 
        auto operator*(const Vec3<U> &v2) const 
        -> Vec3<decltype(T{} * U{})> 
        { 
            return { 
                x * v2.x, 
                y * v2.y, 
                z * v2.z 
            }; 
        }; 
 
        template<typename U> 
        auto operator*(const U &s) const 
        -> Vec3<decltype(T{} * U{})> 
        { 
            return { 
                x * s, 
                y * s, 
                z * s 
            }; 
        }; 
 
        template<typename U> 
        auto operator/(const U &s) const 
        -> Vec3<decltype(T{} / U{})> 
        { 
            return { 
                x / s, 
                y / s, 
                z / s 
            }; 
        }; 
 
        // Allow reading of values 
        T operator[](int i) const { 
            switch(i) 
            { 
                case 0: 
                    return x; 
                case 1: 
                    return y; 
                case 2: 
                    return z; 
                default: 
                    throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!"); 
            } 
        }; 
 
        // Allows writing to values 
        T &operator[](int i) { 
            switch(i) 
            { 
                case 0: 
                    return x; 
                case 1: 
                    return y; 
                case 2: 
                    return z; 
                default: 
                    throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!"); 
            } 
        }; 
 
        // Override cout 
        friend ostream& operator<<(ostream& os, const Vec3<T> &v) { 
            os << "(" << v.x << "," << v.y << "," << v.z << ")"; 
            return os; 
        }; 
 
        // Dot 
        template<typename U> 
        auto dot(const Vec3<U> &v2) const 
        -> decltype(T{} * U{}) 
        { 
            return (x * v2.x + y * v2.y + z * v2.z); 
        }; 
 
        // Cross 
        template<typename U> 
        auto cross(const Vec3<U> &v2) const 
        -> Vec3<decltype(T{} * U{})> 
        { 
            return { 
                y*v2.z - z*v2.y, 
                z*v2.x - x*v2.z, 
                x*v2.y - y*v2.x 
            }; 
        }; 
 
        // Length 
        auto length() const -> typename VecLength<T>::type { 
            return sqrt(static_cast<typename VecLength<T>::type>(x*x + y*y + z*z)); 
        }; 
 
        // Normalize 
        auto normalize() const -> Vec3<typename VecLength<T>::type> { 
            auto len = length(); 
            return { 
                x / len, 
                y / len, 
                z / len 
            }; 
        }; 
    }; 
 
    using Vec3f = Vec3<float>; 
    using Vec3i = Vec3<int>; 
    using Vec3d = Vec3<double>; 
    using Vec3u = Vec3<unsigned char>; 
 
    // ADDITIONAL FUNCTIONS 
    template<typename T> 
    Vec3<T> minV(Vec3<T> a, Vec3<T> b) { 
        Vec3<T> c; 
        c.x = std::min(a.x, b.x); 
        c.y = std::min(a.y, b.y); 
        c.z = std::min(a.z, b.z); 
        return c; 
    }; 
 
    template<typename T> 
    Vec3<T> maxV(Vec3<T> a, Vec3<T> b) { 
        Vec3<T> c; 
        c.x = std::max(a.x, b.x); 
        c.y = std::max(a.y, b.y); 
        c.z = std::max(a.z, b.z); 
        return c; 
    }; 
 
    inline Vec3i floorV(Vec3f v) { 
        Vec3i vi; 
        vi.x = (int)std::floor(v.x); 
        vi.y = (int)std::floor(v.y); 
        vi.z = (int)std::floor(v.z); 
        return vi; 
    }; 
 
    inline Vec3i ceilV(Vec3f v) { 
        Vec3i vi; 
        vi.x = (int)std::ceil(v.x); 
        vi.y = (int)std::ceil(v.y); 
        vi.z = (int)std::ceil(v.z); 
        return vi; 
    }; 
 
    inline Vec3i roundV(Vec3f v) { 
        Vec3i vi; 
        vi.x = (int)std::round(v.x); 
        vi.y = (int)std::round(v.y); 
        vi.z = (int)std::round(v.z); 
        return vi; 
    }; 
}; 
