#include <cmath>
#include <cstdlib>
#include <iostream>

class vector3 {
public:
    vector3() {}
    vector3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vector3& operator+() const { return *this; }
    inline vector3 operator-() const { return vector3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline vector3& operator +=(const vector3 &v2);
    inline vector3& operator -=(const vector3 &v2);
    inline vector3& operator *=(const vector3 &v2);
    inline vector3& operator /=(const vector3 &v2);
    inline vector3& operator *=(const float t);
    inline vector3& operator /=(const float t);

    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    inline float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    inline void make_unit_vector();

    float e[3];
};

inline std::istream& operator>>(std::istream &is, vector3 &t) 
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vector3 &t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];

    return os;
}

inline void vector3::make_unit_vector()
{
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline vector3 operator+(const vector3 &v1, const vector3 &v2)
{
    return vector3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vector3 operator-(const vector3 &v1, const vector3 &v2)
{
    return vector3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vector3 operator*(const vector3 &v1, const vector3 &v2)
{
    return vector3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vector3 operator/(const vector3 &v1, const vector3 &v2)
{
    return vector3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vector3 operator*(float t, const vector3 &v)
{
    return vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vector3 operator/(vector3 &v, float t)
{
    return vector3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vector3 operator*(const vector3 &v, float t)
{
    return vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline float dot(const vector3 &v1, const vector3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vector3 cross(const vector3 &v1, const vector3 &v2)
{
    return vector3(
        (  v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
        (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
        (  v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
    );
}

inline vector3& vector3::operator+=(const vector3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    
    return *this;
}

inline vector3& vector3::operator-=(const vector3 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    
    return *this;
}
inline vector3& vector3::operator*=(const vector3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    
    return *this;
}

inline vector3& vector3::operator/=(const vector3 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    
    return *this;
}

inline vector3& vector3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    
    return *this;
}

inline vector3& vector3::operator/=(const float t)
{
    float k = 1.0 / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    
    return *this;
}

inline vector3 unit_vector(vector3 v)
{
    return v / v.length();
}