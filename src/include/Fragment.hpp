#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
using namespace std;

namespace potato {
    struct Fragment {
        Vec3i pos {};        
        Vec4f color {};
    };
};