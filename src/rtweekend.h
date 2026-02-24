#ifndef RTWEEKEN_H
#define RTWEEKEN_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory> // For shared pointers

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);     // Returns a random real number in [0,1)
}

inline double random_double(double min, double max) {
    return min + (max - min)*random_double();  // Returns a random real in [min,max)
}

// Common headers: re-export of commonly used headers so other files only need to include one file
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"


#endif