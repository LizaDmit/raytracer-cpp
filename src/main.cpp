#include "rtweekend.h" // Already includes most of the needed libraries and files

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h" 
#include "camera.h"

using namespace std;


int main() {

    hittable_list world;  // Creates the container that holds the collection of all objects that rays can hit

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));      // Adds a small sphere in front of the camera to the "world"
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100)); // Adds a huge sphere below the first one to the "world"

    camera cam; // Sets up the camera

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;

    cam.render(world); // Loops over every pixel in the "world" and writes its color output


}