#pragma once 
 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp" 
#include "Clipping.hpp"
using namespace std; 
 
namespace potato { 
    template<typename T> 
    struct BoundBox { 
        Vec3<T> start; 
        Vec3<T> end; 

        // Default constructor
        BoundBox() = default;

        // Constructor which takes in two points
        BoundBox(Vec3<T> p1, Vec3<T> p2) {
            start = minV(p1, p2);
            end = maxV(p1, p2);
        };

        // Does NOT check if 0 <= u|v <= 2 because efficiency (and laziness)
        bool bounds2D(uint_fast8_t u, uint_fast8_t v, T pos_u, T pos_v) {
            return (
                start[u] <= pos_u && pos_u <= end[u] &&
                start[v] <= pos_v && pos_v <= end[v]
            );
        };

        int getECSLCode(Vec3d pos) {
            return getExtendedCohenSutherlandCode(pos, start.x, end.x, start.y, end.y, start.z, end.z);
        };

        BoundBox<T> split() {
            Vec3d middle = (start + end) / 2.0;
            BoundBox<T> bb2 = BoundBox<T>(middle, end); // New goes from middle to end
            end = middle; // Original bb goes from start to middle
            return bb2;
        }
    }; 
 
    using BoundBoxd = BoundBox<double>;
    using BoundBoxf = BoundBox<float>; 
    using BoundBoxi = BoundBox<int>; 
 
    // Convert bounding box to integer box 
    BoundBoxi convertBoundBox(BoundBoxf &boxf); 
}; 
