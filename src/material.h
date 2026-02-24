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

class lambertian : public material {                                         // Matte material
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();     // Generates a random direction biased toward the normal

            if (scatter_direction.near_zero()) scatter_direction = rec.normal; // Prevents the vectors from summing up to zero with the normal
            
            scattered = ray(rec.p, scatter_direction);                      // Creates a new ray starting at the hit point and going in that direction
            attenuation = albedo;                                           // Shows that color of the surface affects the bounced light
            return true;                                                    // Always scatters the light
        }
    private:
        color albedo;
};

class metal : public material {                                               // Metallic material
  public:
    metal(const color& albedo, double fuzz) : albedo(albedo), 
                                              fuzz(fuzz < 1 ? fuzz : 1) {}    // Color and fuzziness

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);              // Computes the mirror reflection direction
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        
        scattered = ray(rec.p, reflected);                                   // Creates the outgoing ray
        attenuation = albedo;                                                // Makes reflected light tinted by the metal’s color (will be multiplied by this color)
        return (dot(scattered.direction(), rec.normal) > 0);                 // Returns true if the ray is reflected to the outside surface
    }

  private:
    color albedo;
    double fuzz;
};

class dielectric : public material {                                           // Glass-like material
    public:
        dielectric(double refraction_index) : 
                                        refraction_index(refraction_index) {}  // Refraction index of the certain material

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);                                // No color absorbption for glass - white color
            double ri = rec.front_face ? (1.0/refraction_index) : 
                                                    refraction_index;          // Chooses either outside or inside absorption index

            vec3 unit_direction = unit_vector(r_in.direction());               // Normalizes incoming rays
            vec3 refracted = refract(unit_direction, rec.normal, ri);          // Computes the "bent" ray using Snell's Law

            scattered = ray(rec.p, refracted);                                 // Creates a new ray from hit point to the refracted direction
            return true;                                                       // Returns true because glass always either refracts or reflects
        }

            private:
                double refraction_index;
};


#endif