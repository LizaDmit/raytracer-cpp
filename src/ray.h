#ifndef RAY_H
#define RAY_H

#include "vec3.h"
using namespace std;

class ray {

    public:

        ray() {}                                                                           // Empty constructor

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {} // Constructor

        const point3& origin() const  { return orig; }                                     // Gettters
        const vec3& direction() const { return dir; }


        point3 at(double t) const {                                                        // Finds where the vector points
            return orig + t*dir;} 

    private:
        point3 orig;
        vec3 dir;

};



#endif