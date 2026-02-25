#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
using namespace std;


class vec3 {

    public:
        double e[3];

        vec3(): e{0,0,0} {}                                         // #1: Default constructor
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}    // #2: Parameterized constructor

        double x() const { return e[0]; }   // Getter function
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }    // Overloading "-": if written -vec3, all members become negative. Would cause an error otherwise

        double operator[](int i) const { return e[i]; } // "Array-like" behaviour
        double& operator[](int i) { return e[i]; }

        // "this" - refers to the object that was called with the mentioned operator
        
        vec3& operator+=(const vec3& v) {    // "Vector-like" behaviour: addition
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        } 
        vec3& operator*=(double t) {        // "Vector-like" behaviour: scalar multiplication
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        vec3& operator/=(double t) {        // "Vector-like" behaviour: scalar division
            return *this *= 1/t;
        }
        double length() const {             // "Vector-like" behaviour: length 
            return sqrt(length_squared());
        }
        double length_squared() const {     // Helper function for getting the length
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];   
        }
        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }
        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }
        bool near_zero() const {             // Returns true if the vector is close to zero in all dimensions
            auto s = 1e-8;
            return (std::abs(e[0]) < s) && (std::abs(e[1]) < s) && (std::abs(e[2]) < s);
        }

};

using point3 = vec3;        // Just for readability of the code. Same as vec3 but different name

inline ostream& operator<<(ostream& out, const vec3& v) {       // Allows to print the vector
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {           // Allows to add vectors
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {           // Allows to add vectors
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {           // Multiplication for color calculation
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {                // Scaling
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {                // Scaling (different order)
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {                // Scaling by fraction
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {               // Dot product of 2 vectors
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {               // Cross product: returns a vector perpendicular to the input ones (in 3D)
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {                        // Vector normalization
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);                            // Generates a random vector
        auto lensq = p.length_squared();                        // Calculates how far the point is from the origin
        if (1e-160 < lensq && lensq <= 1)                       // Keeps only the points inside the sphere that are not too close to the center
            return p / sqrt(lensq);                             // Returns a unit vector
    }                                                           
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)                      // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;                                     // Computes the ray reflected from metal
}

inline vec3 refract(const vec3& uv, const vec3& n, 
                    double etai_over_etat) {                     // uv - incoming ray direction (unit len), n - unit surface normal, etai_over_etat - indices fro Snell's Law
    
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);                // Calculates cos from 𝐚*𝐛=|𝐚||𝐛|cos𝜃, when a and b - unit vectors

    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);      // Computes tangent to normal part of the refracted ray (formula from Snell's Law)

    vec3 r_out_parallel = 
    -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n; // Computes part parallel to n (from Pythagorean theorem)

    return r_out_perp + r_out_parallel;                           // Returns the final ray direction
}

inline vec3 random_in_unit_disk() {                               // Simulates a circular lens in a camera
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)                               // Checks if the randomly chosen point is in the unit circle
            return p;                                             // Based on rejection sampling
    }
}

#endif