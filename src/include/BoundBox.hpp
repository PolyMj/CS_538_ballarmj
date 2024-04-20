#pragma once 
 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp" 
using namespace std; 
 
namespace potato { 
    template<typename T> 
    struct BoundBox { 
        Vec3<T> start; 
        Vec3<T> end; 

        // Does NOT check if 0 <= u|v <= 2 because efficiency (and laziness)
        bool bounds2D(uint_fast8_t u, uint_fast8_t v, T pos_u, T pos_v) {
            return (
                start[u] <= pos_u && pos_u <= end[u] &&
                start[v] <= pos_v && pos_v <= end[v]
            );
        }
    }; 
 
    using BoundBoxd = BoundBox<double>;
    using BoundBoxf = BoundBox<float>; 
    using BoundBoxi = BoundBox<int>; 
 
    // Convert bounding box to integer box 
    BoundBoxi convertBoundBox(BoundBoxf &boxf); 
}; 
