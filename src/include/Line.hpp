#pragma once
#include <cmath>
#include <iostream>
#include "Vector.hpp"
//#include "Buffer.hpp"
using namespace std;

namespace potato {
    template<typename T, typename C>
    struct Line {
        Vec3<T> start {};
        Vec3<T> end {};
        Vec3<C> color {};
    };
}
