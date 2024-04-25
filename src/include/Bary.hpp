#pragma once 
 
#include <cmath> 
#include <vector> 
#include <iostream>
// TIL C++ has therese weird specific-length integers and some "more optimized" versions? Hooray?
#include <cstdint> // uint_fast8_t
#include "Vector.hpp" 
#include "Line.hpp" 
using namespace std; 

#define GRID_CNT 5
#define GRID_DIV (1.0f)/GRID_CNT
 
namespace potato { 
    template<typename T> // Please only use doubles or floats, things might break otherwise
    struct BaryData { 
        ImplicitLine<T> AB; 
        ImplicitLine<T> AC; 
        T Cval {}; 
        T Bval {}; 
        
        // May rewrite to use Vec2's, but that would require creating Vec2's
        BaryData(Vec3<T> &A, Vec3<T> &B, Vec3<T> &C) { 
            AB = ImplicitLine<T>(A,B); 
            AC = ImplicitLine<T>(A,C); 
            Cval = AB.eval(C.x, C.y); 
            Bval = AC.eval(B.x, B.y); 
        }; 
    }; 
    
    template<typename T>
    inline Vec3<T> barycentric(   BaryData<T> &bd, 
                                T x, T y) { 
        Vec3<T> bary; 
        bary.z = bd.AB.eval(x, y) / bd.Cval; 
        bary.y = bd.AC.eval(x, y) / bd.Bval; 
        bary.x = 1.0 - bary.y - bary.z; 
        return bary; 
    }; 

    template<typename T>
    inline bool isInside(Vec3<T> bary) {
        return (
            bary.x >= 0 &&
            bary.y >= 0 &&
            bary.z >= 0
        );
    };

    // Returns the barycentric weights of pos using p1, p2, p3 as the vertices of a triangle
    // Looks at the face's normal to determine which 2 axes to use (barycentric is 2D, points are 3D)
    template<typename T>
    inline Vec3<T> bary3D(Vec3<T> pos, Vec3<T> p1, Vec3<T> p2, Vec3<T> p3, Vec3<T> normal) {
        // Find two smallest normal values and store their indicies/axes
        uint_fast8_t u, v;
        if (abs(normal.x) < abs(normal.y)) {
            u = 0;
            if (abs(normal.y) < abs(normal.z))
                v = 1;
            else
                v = 2;
        }
        else {
            u = 1;
            if (abs(normal.x) < abs(normal.z))
                v = 0;
            else
                v = 2;
        }

        // Get points of triangle using 2 axes only
        Vec3<T> A = Vec3<T>(p1[u], p1[v], 0.0);
        Vec3<T> B = Vec3<T>(p2[u], p2[v], 0.0);
        Vec3<T> C = Vec3<T>(p3[u], p3[v], 0.0);

        // Get bary data
        BaryData<T> bd = BaryData<T>(A, B, C);
        Vec3<T> bary = barycentric(bd, pos[u], pos[v]);

        return bary;
    }

    // Converts from Vec4's to Vec3's and calls the above function
    template<typename T>
    inline Vec3<T> bary3D(Vec4<T> pos, Vec4<T> p1, Vec4<T> p2, Vec4<T> p3, Vec3<T> normal) {
        return bary3D(pos, Vec3<T>(p1), Vec3<T>(p2), Vec3<T>(p3), normal);
    }

    // Interpolates 3 values using barycentric coordinates
    // Allows any type T such that T*float is defined
    template<typename T, typename V>
    inline V interpolateBary(Vec3<T> bary, V A, V B, V C) {
        return A*bary.x + B*bary.y + C*bary.z;
    };

    // Regularly adds/ignores values along each axis depending on the bary value
    // Uses like "gridlines" to see the shape of value distribution and see if laws of perspective are being obeyed
    template<typename T, typename V>
    inline V interpolateBaryGrid(Vec3<T> bary, V A, V B, V C) {
        V res;
        if (fmod(bary.x, GRID_DIV) < GRID_DIV/2.0f) {
            res = res + A;
        }
        if (fmod(bary.y, GRID_DIV) < GRID_DIV/2.0f) {
            res = res + B;
        }
        if (fmod(bary.z, GRID_DIV) < GRID_DIV/2.0f) {
            res = res + C;
        }
        return res;
    }
};
