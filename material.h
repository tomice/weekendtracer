#ifndef MATERIAL_H
#define MATERIAL_H
#include "hitable.h"
#include "ray.h"

// FIXME: I don't think this is 100% accurate. For Linux/BSD-based systems, use drand48 if it's available.
//        Otherwise, try to find an appropriate compatible library that works on the target system.
//        This is declared multiple times, too, so fix that, as well.
#define drand48() (rand()/(RAND_MAX + 1.0))

struct hit_record;

float schlick(float cosine, float ref_idx) 
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vector3 &v, const vector3 &n, float ni_over_nt, vector3 &refracted)
{
    vector3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);

        return true;
    }
    else {
        return false;
    }
}

vector3 reflect(const vector3 &v, const vector3 &n)
{
    return v - 2 * dot(v, n) * n;
}

vector3 random_in_unit_sphere()
{
    vector3 p;

    do {
        p = 2.0 * vector3(drand48(), drand48(), drand48()) - vector3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}

class material 
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vector3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const vector3 &a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, vector3 &attenuation, ray &scattered) const {
        vector3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;

        return true;
    }
    vector3 albedo;
};

class metal : public material {
public:
    metal(const vector3 &a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    virtual bool scatter(const ray &r_in, const hit_record &rec, vector3 &attenuation, ray &scattered) const {
        vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vector3 albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, vector3 &attenuation, ray &scattered) const {
        vector3 outward_normal;
        vector3 reflected = reflect(r_in.direction(), rec.normal);
        vector3 refracted;
        float ni_over_nt;
        float reflect_prob;
        float cosine;

        attenuation = vector3(1.0, 1.0, 1.0);
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            reflect_prob = 1.0;
        }
        if (drand48() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        }
        else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }
    float ref_idx;
};

#endif