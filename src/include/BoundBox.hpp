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
    };

    using BoundBoxf = BoundBox<float>;
    using BoundBoxi = BoundBox<int>;   
    
    // Convert bounding box to integer box
    BoundBoxi convertBoundBox(BoundBoxf &boxf);
};
