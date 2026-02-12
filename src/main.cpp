#include "rtweekend.h" // Already includes most of the needed libraries and files

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h" 

using namespace std;

color ray_color(const ray& r, const hittable& world) {

    hit_record rec;                                                 // A structure to store intersection info
    if (world.hit(r, 0, infinity, rec)) {                           // Search from t = 0 to infinity if the ray hits anything
        return 0.5 * (rec.normal + color(1,1,1));                   // Returns the objects color
    }

    vec3 unit_direction = unit_vector(r.direction());               // Normalize ray direction to compute the gradient for the background
    auto a = 0.5*(unit_direction.y() + 1.0);                        // Component to add to the blend factor
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);   // Returns the background color
}

int main() {

    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));      // A small sphere in front of the camera
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100)); // A huge sphere below the first one

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Vectors across horizontal and vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Horizontal and vertical delta between pixels
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Location of the upper left point of the viewport
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

    // Location of the upper left pixel (pixel_delta_u + pixel_delta_v is a movement along the horizontal vector)
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {

        clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;

        for (int i = 0; i < image_width; i++) {

            // World-space position of the center of pixel (i, j)
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

            // Direction pointing from the camera to that pixel
            auto ray_direction = pixel_center - camera_center;

            // Ray with the required direction
            ray r(camera_center, ray_direction);

            // Traces the ray into the scene and computes the color seen along that ray
            color pixel_color = ray_color(r, world);
            write_color(cout, pixel_color);
        }
    }
    clog << "\rDone.                 \n";
}