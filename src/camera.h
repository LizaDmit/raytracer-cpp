#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "interval.h"
#include "material.h"

class camera {
    public:
        // Default image dimensions
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;  // Count of random samples for each pixel
        int max_depth = 10;          // Maximum number of ray bounces into the scene

        point3 lookfrom = point3(0,0,0); // Point camera is looking from
        point3 lookat = point3(0,0,-1);  // Point camera is looking at
        vec3 vup = vec3(0,1,0);          // Camera-relative "up" direction


        double defocus_angle = 0;  // Variation angle of rays through each pixel
        double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

        double vfov = 90; // Vertical view angle - field of view

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
        vec3 u, v, w;              // Camera frame basis vectors
        vec3 defocus_disk_u;       // Defocus disk horizontal radius
        vec3 defocus_disk_v;       // Defocus disk vertical radius


        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;
            

            // Viewport Dimensions
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Set up of the orthonormal basis for the camera
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Vectors across horizontal and vertical viewport edges
            auto viewport_u = viewport_width * u;   // Stretches u to the whole viewport witdth 
            auto viewport_v = viewport_height * -v; // Stretches v to the whole viewport height (- because pixels rows go downward)

            // Horizontal and vertical delta between pixels
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of the upper left point of the viewport
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;

            // Location of the upper left pixel (pixel_delta_u + pixel_delta_v is a movement along the horizontal vector)
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Calculate the camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));  // How wide the lens opening is in world space
            defocus_disk_u = u * defocus_radius;                                                 // Scales the camera’s right vector by that radius
            defocus_disk_v = v * defocus_radius;                                                 // Scales the camera’s up vector by that radius
        }

        ray get_ray(int i, int j) {

            auto offset = sample_square();   // This is needed to remove the edginess of the picture by aiming in different parts of the pixel
                                             // not just in the middle of it
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
            // This way the pixel is not just background color but with the equal probability either object or background color
            // so on the edge of the object colors mixe
            
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);  // Returns the vector to a random point in the [-0.5,-0.5]-[0.5,0.5] unit square
        }

        point3 defocus_disk_sample() const {
            // Returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);   // Returns a random point inside the circular aperture centered at the camera
        }

        color ray_color(const ray& r, const hittable& world, int depth) const {
            if (depth <= 0) return color(0,0,0);

            hit_record rec;                                                 // A structure to store intersection info
            
            if (world.hit(r, interval(0.001, infinity), rec)) {                 // Search from t = 0 to infinity if the ray hits anything
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, world, depth-1);
                return color(0,0,0);
            /*    vec3 direction = rec.normal + random_unit_vector();         // Randomly sends the ray from the object, get the color of the surrondings,
                                                                            // recursively traces back to the object and gives its the mixed color
                return 0.5 * ray_color(ray(rec.p, direction), world, depth-1);       // Sends the ray back to the sphere. 0.5 is the fraction of the incoming light refelected*/
            }

            vec3 unit_direction = unit_vector(r.direction());               // Normalize ray direction to compute the gradient for the background
            auto a = 0.5*(unit_direction.y() + 1.0);                        // Component to add to the blend factor
            return (1.0-a)*color(0.2, 0.5, 0.7) + a*color(0.2, 0.8, 0.6);  // Returns the background color
        }


};






#endif