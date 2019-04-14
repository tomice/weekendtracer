#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <cmath>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

// FIXME: I don't think this is 100% accurate. For Linux/BSD-based systems, use drand48 if it's available.
//        Otherwise, try to find an appropriate compatible library that works on the target system.
//        This is declared multiple times, too, so fix that, as well.
#define drand48() (rand()/(RAND_MAX + 1.0))

vector3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        vector3 attenuation;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vector3(0, 0, 0);
        }
    }
    else {
        vector3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - t) * vector3(1.0, 1.0, 1.0) + t * vector3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] =  new sphere(vector3(0, -1000, 0), 1000, new lambertian(vector3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vector3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48()); 
            if ((center - vector3(4, 0.2, 0)).length() > 0.9) { 
                if (choose_mat < 0.8) {
                    list[i++] = new sphere(center, 0.2, 
                        new lambertian(vector3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                }
                else if (choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2,
                        new metal(vector3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())),  0.5 * drand48()));
                }
                else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vector3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vector3(-4, 1, 0), 1.0, new lambertian(vector3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vector3(4, 1, 0), 1.0, new metal(vector3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

int main(int argc, char *argv[])
{
    const int nx = 1200;
    const int ny = 800;
    const int ns = 10;
    const int color_range = 255;
    const float R = cos(M_PI / 4);
    const float dist_to_focus = 10.0;
    const float aperture = 0.1;
    std::ofstream file;
    vector3 lookfrom(13, 2, 3);
    vector3 lookat(0, 0, 0);
    vector3 col(0, 0, 0);
    hitable *list[5];
    list[0] = new sphere(vector3(0, 0, -1), 0.5, new lambertian(vector3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vector3(0, -100.5, -1), 100, new lambertian(vector3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vector3(1, 0, -1), 0.5, new metal(vector3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vector3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vector3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, 5);
    world = random_scene();
    camera cam(lookfrom, lookat, vector3(0, 1, 0), 20, 
               static_cast<float>(nx) / static_cast<float>(ny), 
               aperture, dist_to_focus);

    file.open("image.ppm");
    if (!file) {
        std::cerr << "Error: Could not open file\n";
        exit(1);
    }
    else {
        file << "P3\n# Basic RGB color file\n"
             << nx << " " << ny << "\n" << color_range << "\n";
        // FIXME: This main loop is like O(n^3). Find a better way of doing this so it's faster
        for (int y = ny - 1; y >= 0; y--) {
            for (int x = 0; x < nx; x++) {
                for (int s = 0; s < ns; s++) {
                    const float u = static_cast<float>(x + drand48()) / static_cast<float>(nx);
                    const float v = static_cast<float>(y + drand48()) / static_cast<float>(ny);
                    const ray r = cam.get_ray(u, v);
                    const vector3 p = r.point_at_parameter(2.0);
                    
                    col += color(r, world, 0);    
                }
                col /= static_cast<float>(ns);
                col = vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                const int x_r = static_cast<int>(255.99 * col.r());
                const int x_g = static_cast<int>(255.99 * col.g());
                const int x_b = static_cast<int>(255.99 * col.b());

                file << x_r << " " << x_g << " " << x_b << "\n";
            }
        }
        file.close();
    }

    return 0;
}