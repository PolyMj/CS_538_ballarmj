#pragma once 
 
#include <iostream> 
#include <cmath> 
using namespace std; 
 
#define EPS_F 1e-5 
#define EPS_D 1e-12 
 
inline bool close(float a, float b) { 
    if(!(abs(a - b) <= EPS_F)) { 
        cout << "ERROR: " << a << " vs. " << b << endl; 
        return false; 
    } 
    return true; 
} 
 
inline bool really_close(double a, double b) { 
    if(!(abs(a - b) <= EPS_D)) { 
        //cout << std::setprecision (15); 
        cout << "ERROR: " << a << " vs. " << b << endl; 
        return false; 
    } 
    return true; 
} 
 
 
