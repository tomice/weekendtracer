#ifndef HITABLE_H
#define HITABLE_H
#include "ray.h"

class material;

struct hit_record {
    float t;
    vector3 p;
    vector3 normal;
    material *mat_ptr;
};

class hitable {
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif