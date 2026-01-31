#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 t;
        vec3 normal;
        double t;
};

// Like a parent class for other children classes like spheres, triangles, etc;
// exists to enforce a common pattern:
class hittable {
    virtual ~hittable() = default;


    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};








#endif