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
    struct BaryData { 
        ImplicitLine<float> AB; 
        ImplicitLine<float> AC; 
        float Cval {}; 
        float Bval {}; 
        
        // May rewrite to use Vec2's, but that would require creating Vec2's
        BaryData(Vec3f &A, Vec3f &B, Vec3f &C) { 
            AB = ImplicitLine<float>(A,B); 
            AC = ImplicitLine<float>(A,C); 
            Cval = AB.eval(C.x, C.y); 
            Bval = AC.eval(B.x, B.y); 
        }; 
    }; 
 
    inline Vec3f barycentric(   BaryData &bd, 
                                float x, float y) { 
        Vec3f bary; 
        bary.z = bd.AB.eval(x, y) / bd.Cval; 
        bary.y = bd.AC.eval(x, y) / bd.Bval; 
        bary.x = 1.0f - bary.y - bary.z; 
        return bary; 
    }; 

    inline bool isInside(Vec3f bary) {
        return (
            bary.x >= 0 &&
            bary.y >= 0 &&
            bary.z >= 0
        );
    };

    // Returns a boolean of whether or not the point is inside the triangle
    // Assumes you've already validated that the point is coplanar with the triangle
    // Will use whichever two axes are smallest in the normal for barycentric calculations
    inline Vec3f bary3D(Vec3f pos, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f normal) {
        // Find two smallest normal values and store their indicies
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
        Vec3f A = Vec3f(p1[u], p1[v], 0.0f);
        Vec3f B = Vec3f(p2[u], p2[v], 0.0f);
        Vec3f C = Vec3f(p3[u], p3[v], 0.0f);

        // Get bary data
        BaryData bd = BaryData(A, B, C);
        Vec3f bary = barycentric(bd, pos[u], pos[v]);

        return bary;
    }

    // Interpolates 3 values using barycentric coordinates
    // Allows any type T such that T*float is defined
    template<typename T>
    inline T interpolateBary(Vec3f bary, T A, T B, T C) {
        return A*bary.x + B*bary.y + C*bary.z;
    };

    // Regularly adds/ignors values along each axis depending on the bary value
    // Uses like "gridlines" to see the shape of value distribution and see if laws of perspective are being obeyed
    template<typename T>
    inline T interpolateBaryGrid(Vec3f bary, T A, T B, T C) {
        T res;
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
