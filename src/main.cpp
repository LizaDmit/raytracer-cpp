#include "rtweekend.h" // Already includes most of the needed libraries and files

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h" 
#include "camera.h"
#include "material.h"

using namespace std;


int main() {

    hittable_list world;  // Creates the container that holds the collection of all objects that rays can hit

    auto material_ground = make_shared<lambertian>(color(0.6, 0.9, 0.4));
    auto material_center = make_shared<lambertian>(color(0.5, 0.7, 1.0));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right  = make_shared<metal>(color(0.6, 0.8, 0.4));

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    camera cam; // Sets up the camera

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world); // Loops over every pixel in the "world" and writes its color output


}