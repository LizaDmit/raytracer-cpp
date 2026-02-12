#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"

class sphere : public hittable {
  public:

    // Constructor
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    // Override - safety feature for virtual. Not mandatory but a good practice.
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {

        vec3 oc = center - r.origin();

        // Coefficient of the quadratic function
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        // Check the smaller root
        auto root = (h - sqrtd) / a;

        // If not in the acceptable range, try the larger root
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;

        // If not in the acceptable range, report no hit
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        // Inttersection point
        rec.t = root;

        // The hit point using the ray equation
        rec.p = r.at(rec.t);

        // The surface normal using hit point
        rec.normal = (rec.p - center) / radius;

        // Normalized vector from the sphere center to the hit point
        vec3 outward_normal = (rec.p - center) / radius;
        
        // Set the direction of the vector
        rec.set_face_normal(r, outward_normal);

        return true;
    }

  private:
    point3 center;
    double radius;
};




#endif 