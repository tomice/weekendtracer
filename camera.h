#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"

// FIXME: I don't think this is 100% accurate. For Linux/BSD-based systems, use drand48 if it's available.
//        Otherwise, try to find an appropriate compatible library that works on the target system.
//        This is declared multiple times, too, so fix that, as well.
#define drand48() (rand()/(RAND_MAX + 1.0))

// FIXME: This exists on some systems, so instead of doing a #define, figure out the target system's
//        available PI constant and utilize that
#define M_PI 3.14159265358979323846264338327950288

vector3 random_in_unit_disk()
{
    vector3 p;

    do {
        p = 2.0 * vector3(drand48(), drand48(), 0) - vector3(1, 1, 0);
    } while (dot(p, p) >= 1.0);

    return p;
}

class camera {
public:
    camera(vector3 lookfrom, vector3 lookat, vector3 vup, float vfov, float aspect, float aperture, float focus_dist) {
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    ray get_ray(float s, float t) { 
        vector3 rd = lens_radius * random_in_unit_disk();
        vector3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
    }
    vector3 origin;
    vector3 lower_left_corner;
    vector3 horizontal;
    vector3 vertical;
    vector3 u;
    vector3 v;
    vector3 w;
    float lens_radius;
};

#endif