#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "interval.h"

class camera {
    public:
        // Default image dimensions
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;  // Count of random samples for each pixel
        int max_depth = 10;          // Maximum number of ray bounces into the scene

        void render(const hittable& world) {
            initialize();

            cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {

                clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;

                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world, max_depth);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);

                    /*
                    // World-space position of the center of pixel (i, j)
                    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

                    // Direction pointing from the camera to that pixel
                    auto ray_direction = pixel_center - center;

                    // Ray with the required direction
                    ray r(center, ray_direction);

                    // Traces the ray into the scene and computes the color seen along that ray
                    pixel_color = ray_color(r, world);
                    write_color(cout, pixel_color); */
                }
            }
            clog << "\rDone.                 \n";
        }

    private:
        int image_height;            // Rendered image height
        double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
        point3 center;               // Camera center
        point3 pixel00_loc;          // Location of pixel 0, 0
        vec3 pixel_delta_u;          // Offset to pixel to the right
        vec3 pixel_delta_v;          // Offset to pixel below


        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;
            
            center = point3(0, 0, 0);

            // Viewport Dimensions
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Vectors across horizontal and vertical viewport edges
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Horizontal and vertical delta between pixels
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of the upper left point of the viewport
            auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

            // Location of the upper left pixel (pixel_delta_u + pixel_delta_v is a movement along the horizontal vector)
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) {

            auto offset = sample_square();   // This is needed to remove the edginess of the picture by aiming in different parts of the pixel
                                             // not just in the middle of it
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
            // This way the pixel is not just background color but with the equal probability either object or background color
            // so on the edge of the object colors mixe
            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);  // Returns the vector to a random point in the [-0.5,-0.5]-[0.5,0.5] unit square
        }

        color ray_color(const ray& r, const hittable& world, int depth) const {
            if (depth <= 0) return color(0,0,0);
            hit_record rec;                                                 // A structure to store intersection info
            if (world.hit(r, interval(0.01, infinity), rec)) {                 // Search from t = 0 to infinity if the ray hits anything
                vec3 direction = random_on_hemisphere(rec.normal);          // Randomly sends the ray from the object, get the color of the surrondings,
                                                                            // recursively traces back to the object and gives its the mixed color
                return 0.5 * ray_color(ray(rec.p, direction), world, depth-1);       // Sends the ray back to the sphere. 0.5 is the fraction of the incoming light refelected
            }

            vec3 unit_direction = unit_vector(r.direction());               // Normalize ray direction to compute the gradient for the background
            auto a = 0.5*(unit_direction.y() + 1.0);                        // Component to add to the blend factor
            return (1.0-a)*color(0.2, 0.9, 0.4) + a*color(0.9, 0.01, 0.1);  // Returns the background color
        }


};






#endif