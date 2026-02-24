#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"

class material {
    public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();     // Generates a random direction biased toward the normal

            if (scatter_direction.near_zero()) scatter_direction = rec.normal; // Prevents the vectors from summing up to zero with the normal
            
            scattered = ray(rec.p, scatter_direction);                      // Creates a new ray starting at the hit point and going in that direction
            attenuation = albedo;                                           // Shows that color of the surface affects the bounced light
            return true;
        }
    private:
        color albedo;
};

class metal : public material {
  public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);              // Computes the mirror reflection direction
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        
        scattered = ray(rec.p, reflected);                                   // Creates the outgoing ray
        attenuation = albedo;                                                // Makes reflected light tinted by the metal’s color (will be multiplied by this color)
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    color albedo;
    double fuzz;
};


#endif