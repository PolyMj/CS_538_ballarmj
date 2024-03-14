#pragma once 
 
#include <cmath> 
using namespace std; 
 
namespace potato { 
        /////////////////////////////////////////////////////////////////////////// 
    // MAT4 
    /////////////////////////////////////////////////////////////////////////// 
 
    template<typename T> 
    struct Mat4 { 
        Vec4<T> cols[4]; 
 
        Mat4() {}; 
        Mat4(T val) { 
            cols[0][0] = val; 
            cols[1][1] = val; 
            cols[2][2] = val; 
            cols[3][3] = val; 
        }; 
        Mat4(   T v00, T v01, T v02, T v03, 
                T v10, T v11, T v12, T v13, 
                T v20, T v21, T v22, T v23, 
                T v30, T v31, T v32, T v33) { 
 
            cols[0][0] = v00; 
            cols[0][1] = v01; 
            cols[0][2] = v02; 
            cols[0][3] = v03; 
 
            cols[1][0] = v10; 
            cols[1][1] = v11; 
            cols[1][2] = v12; 
            cols[1][3] = v13; 
 
            cols[2][0] = v20; 
            cols[2][1] = v21; 
            cols[2][2] = v22; 
            cols[2][3] = v23; 
 
            cols[3][0] = v30; 
            cols[3][1] = v31; 
            cols[3][2] = v32; 
            cols[3][3] = v33; 
        }; 
        Mat4(Vec4<T> c0, Vec4<T> c1, Vec4<T> c2, Vec4<T> c3) { 
            cols[0] = c0; 
            cols[1] = c1; 
            cols[2] = c2; 
            cols[3] = c3; 
        }; 
 
        // Override cout 
        friend ostream& operator<<(ostream& os, const Mat4<T> &m) { 
            os << "|\t" << m.cols[0][0] << "\t" << m.cols[1][0] << "\t" << m.cols[2][0] << "\t" << m.cols[3][0] << "\t|" << endl; 
            os << "|\t" << m.cols[0][1] << "\t" << m.cols[1][1] << "\t" << m.cols[2][1] << "\t" << m.cols[3][1] << "\t|" << endl; 
            os << "|\t" << m.cols[0][2] << "\t" << m.cols[1][2] << "\t" << m.cols[2][2] << "\t" << m.cols[3][2] << "\t|" << endl; 
            os << "|\t" << m.cols[0][3] << "\t" << m.cols[1][3] << "\t" << m.cols[2][3] << "\t" << m.cols[3][3] << "\t|" << endl; 
            return os; 
        }; 
 
        Vec4<T>& operator[](int i) { 
            if(i >= 0 && i < 4) { 
                return cols[i]; 
            } 
            else { 
                throw std::out_of_range("ERROR: Out of range!"); 
            } 
        }; 
 
        Vec4<T> operator[](int i) const { 
            if(i >= 0 && i < 4) { 
                return cols[i]; 
            } 
            else { 
                throw std::out_of_range("ERROR: Out of range!"); 
            } 
        }; 
 
        Mat4<T> transpose() const { 
            return { 
                cols[0][0], cols[1][0], cols[2][0], cols[3][0], 
                cols[0][1], cols[1][1], cols[2][1], cols[3][1], 
                cols[0][2], cols[1][2], cols[2][2], cols[3][2], 
                cols[0][3], cols[1][3], cols[2][3], cols[3][3] 
            }; 
        }; 
 
        Vec4<T> operator*(const Vec4<T> &v) const 
        { 
            return { 
                cols[0][0] * v.x + cols[1][0] * v.y + cols[2][0] * v.z + cols[3][0] * v.w, 
                cols[0][1] * v.x + cols[1][1] * v.y + cols[2][1] * v.z + cols[3][1] * v.w, 
                cols[0][2] * v.x + cols[1][2] * v.y + cols[2][2] * v.z + cols[3][2] * v.w, 
                cols[0][3] * v.x + cols[1][3] * v.y + cols[2][3] * v.z + cols[3][3] * v.w, 
            }; 
        }; 
 
        Mat4<T> operator*(const Mat4<T> &m) const 
        { 
            Mat4<T> t = transpose(); 
            return { 
                t.cols[0].dot(m.cols[0]), t.cols[0].dot(m.cols[1]), t.cols[0].dot(m.cols[2]), t.cols[0].dot(m.cols[3]), 
                t.cols[1].dot(m.cols[0]), t.cols[1].dot(m.cols[1]), t.cols[1].dot(m.cols[2]), t.cols[1].dot(m.cols[3]), 
                t.cols[2].dot(m.cols[0]), t.cols[2].dot(m.cols[1]), t.cols[2].dot(m.cols[2]), t.cols[2].dot(m.cols[3]), 
                t.cols[3].dot(m.cols[0]), t.cols[3].dot(m.cols[1]), t.cols[3].dot(m.cols[2]), t.cols[3].dot(m.cols[3]), 
            }; 
        }; 
 
        Mat4<T> operator*(const T &s) const 
        { 
            return { 
                cols[0]*s, 
                cols[1]*s, 
                cols[2]*s, 
                cols[3]*s 
            }; 
        }; 
    }; 
 
    using Mat4f = Mat4<float>; 
 
    /////////////////////////////////////////////////////////////////////////// 
    // MAT3 
    /////////////////////////////////////////////////////////////////////////// 
 
    template<typename T> 
    struct Mat3 { 
        Vec3<T> cols[3]; 
 
        Mat3() {}; 
        Mat3(T val) { 
            cols[0][0] = val; 
            cols[1][1] = val; 
            cols[2][2] = val; 
        }; 
        Mat3(T v00, T v01, T v02, T v10, T v11, T v12, T v20, T v21, T v22) { 
            cols[0][0] = v00; 
            cols[0][1] = v01; 
            cols[0][2] = v02; 
 
            cols[1][0] = v10; 
            cols[1][1] = v11; 
            cols[1][2] = v12; 
 
            cols[2][0] = v20; 
            cols[2][1] = v21; 
            cols[2][2] = v22; 
        }; 
        Mat3(Vec3<T> c0, Vec3<T> c1, Vec3<T> c2) { 
            cols[0] = c0; 
            cols[1] = c1; 
            cols[2] = c2; 
        }; 
        Mat3(const Mat4<T> &other) { 
            cols[0] = other.cols[0]; 
            cols[1] = other.cols[1]; 
            cols[2] = other.cols[2]; 
        } 
 
        // Override cout 
        friend ostream& operator<<(ostream& os, const Mat3<T> &m) { 
            os << "|\t" << m.cols[0][0] << "\t" << m.cols[1][0] << "\t" << m.cols[2][0] << "\t|" << endl; 
            os << "|\t" << m.cols[0][1] << "\t" << m.cols[1][1] << "\t" << m.cols[2][1] << "\t|" << endl; 
            os << "|\t" << m.cols[0][2] << "\t" << m.cols[1][2] << "\t" << m.cols[2][2] << "\t|" << endl; 
            return os; 
        }; 
 
        Vec3<T>& operator[](int i) { 
            if(i >= 0 && i < 3) { 
                return cols[i]; 
            } 
            else { 
                throw std::out_of_range("ERROR: Out of range!"); 
            } 
        }; 
 
        Vec3<T> operator[](int i) const { 
            if(i >= 0 && i < 3) { 
                return cols[i]; 
            } 
            else { 
                throw std::out_of_range("ERROR: Out of range!"); 
            } 
        }; 
 
        Mat3<T> transpose() const { 
            return { 
                cols[0][0], cols[1][0], cols[2][0], 
                cols[0][1], cols[1][1], cols[2][1], 
                cols[0][2], cols[1][2], cols[2][2] 
            }; 
        }; 
 
        Vec3<T> operator*(const Vec3<T> &v) const 
        { 
            return { 
                cols[0][0] * v.x + cols[1][0] * v.y + cols[2][0] * v.z, 
                cols[0][1] * v.x + cols[1][1] * v.y + cols[2][1] * v.z, 
                cols[0][2] * v.x + cols[1][2] * v.y + cols[2][2] * v.z 
            }; 
        }; 
 
        Mat3<T> operator*(const Mat3<T> &m) const 
        { 
            Mat3<T> t = transpose(); 
            return { 
                t.cols[0].dot(m.cols[0]), t.cols[0].dot(m.cols[1]), t.cols[0].dot(m.cols[2]), 
                t.cols[1].dot(m.cols[0]), t.cols[1].dot(m.cols[1]), t.cols[1].dot(m.cols[2]), 
                t.cols[2].dot(m.cols[0]), t.cols[2].dot(m.cols[1]), t.cols[2].dot(m.cols[2]), 
            }; 
        }; 
 
        Mat3<T> operator*(const T &s) const 
        { 
            return { 
                cols[0]*s, 
                cols[1]*s, 
                cols[2]*s 
            }; 
        }; 
    }; 
 
    using Mat3f = Mat3<float>; 
}; 
