#pragma once

#include <cmath>
#include <iostream>
#include "Vector.hpp"
using namespace std;

namespace potato {
    template<typename T, typename C>    
    struct Line{
        Vec3<T> start {};
        Vec3<T> end{};
        Vec3<C> color{};
    };
       
    template<typename T>
    struct ImplicitLine {
        Vec3<T> start {};
        Vec3<T> end{};
        T dx {};
        T dy {};
        T c1 {};
        T c2 {};

        ImplicitLine() = default;

        ImplicitLine(Vec3<T> start, Vec3<T> end) {
            this->start = start;
            this->end = end;
            dy = end.y - start.y;
            dx = end.x - start.x;
            c1 = start.x*end.y;
            c2 = end.x*start.y;
        };

        float eval(float x, float y) {
           return -dy*x + dx*y + c1 - c2; 
        };
    };   
};
