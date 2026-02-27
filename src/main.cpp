#include "rtweekend.h" // Already includes most of the needed libraries and files

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h" 
#include "camera.h"
#include "material.h"

using namespace std;


int main() {

    hittable_list world;  // Creates the container that holds the collection of all objects that rays can hit

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -1; a < 1; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    
    /*auto R = std::cos(pi/4);

    auto material_left  = make_shared<lambertian>(color(0.9,0,0.5));
    auto material_right = make_shared<lambertian>(color(0,1,0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));*/
    
    /*auto material_ground = make_shared<lambertian>(color(0.4, 0.9, 0.1));
    auto material_center = make_shared<lambertian>(color(0.2, 0.3, 1.0));
    // Hollow glass sphere is modeled just as a bubble of air inside the glass environment
    auto material_left   = make_shared<dielectric>(1.50);        
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.1, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));*/

    camera cam; // Sets up the camera

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;                  // Closiness 
    cam.lookfrom = point3(12,2,3);  // Point of view
    cam.lookat = point3(0,0,0);    // Direction of view
    cam.vup = vec3(0,1,0);          // Camera tilt

    cam.defocus_angle = 0.0;       // Controls the aperture size (how wide the lens opening is)
    cam.focus_dist    = 10.0;        // Sets the distance from the camera to the sharp focus plane

    cam.render(world); // Loops over every pixel in the "world" and writes its color output


}