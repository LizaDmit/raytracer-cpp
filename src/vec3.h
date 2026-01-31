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


#endif