#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

#define M_2PI (2 * M_PI)

inline float frand() { return (float)rand() / (float)RAND_MAX; }
inline float min(float a, float b) { return a < b ? a : b; }
inline float max(float a, float b) { return a > b ? a : b; }

class V2
{
public:
    union
    {
        struct { float x, y; };
        float xy[3];
    };

    V2() : x(0), y(0) {}
    V2(float _x, float _y) : x(_x), y(_y) {}
    V2(const V2 &vec) : x(vec.x), y(vec.y) {}

    V2 operator - () const { return V2(-x, -y); }

    V2 operator + (const V2 &vec) const { return V2(x + vec.x, y + vec.y); }
    V2 operator - (const V2 &vec) const { return V2(x - vec.x, y - vec.y); }
    V2 operator * (const V2 &vec) const { return V2(x * vec.x, y * vec.y); }
    V2 operator / (const V2 &vec) const { return V2(x / vec.x, y / vec.y); }
    V2 operator + (float s) const { return V2(x + s, y + s); }
    V2 operator - (float s) const { return V2(x - s, y - s); }
    V2 operator * (float s) const { return V2(x * s, y * s); }
    V2 operator / (float s) const { return V2(x / s, y / s); }

    friend V2 operator + (float s, const V2 &vec) { return V2(s + vec.x, s + vec.y); }
    friend V2 operator - (float s, const V2 &vec) { return V2(s - vec.x, s - vec.y); }
    friend V2 operator * (float s, const V2 &vec) { return V2(s * vec.x, s * vec.y); }
    friend V2 operator / (float s, const V2 &vec) { return V2(s / vec.x, s / vec.y); }

    V2 &operator += (const V2 &vec) { return *this = *this + vec; }
    V2 &operator -= (const V2 &vec) { return *this = *this - vec; }
    V2 &operator *= (const V2 &vec) { return *this = *this * vec; }
    V2 &operator /= (const V2 &vec) { return *this = *this / vec; }
    V2 &operator += (float s) { return *this = *this + s; }
    V2 &operator -= (float s) { return *this = *this - s; }
    V2 &operator *= (float s) { return *this = *this * s; }
    V2 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const V2 &vec) const { return x == vec.x && y == vec.y; }
    bool operator != (const V2 &vec) const { return x != vec.x || y != vec.y; }

    float lengthSquared() const { return x * x + y * y; }
    float length() const { return sqrtf(lengthSquared()); }
    float dot(const V2 &vec) const { return x * vec.x + y * vec.y; }
    V2 unit() const { return *this / length(); }
    void normalize() { *this = unit(); }
    static V2 lerp(const V2 &a, const V2 &b, float percent) { return a + (b - a) * percent; }

    float min() const { return ::min(x, y); }
    float max() const { return ::max(x, y); }
    static V2 min(const V2 &a, const V2 &b) { return V2(::min(a.x, b.x), ::min(a.y, b.y)); }
    static V2 max(const V2 &a, const V2 &b) { return V2(::max(a.x, b.x), ::max(a.y, b.y)); }

    float toAngle() const { return atan2f(y, x); }
    static V2 fromAngle(float theta) { return V2(cosf(theta), sinf(theta)); }
    static V2 uniform() { return fromAngle(frand() * M_2PI); }

    static const V2 X;
    static const V2 Y;
};

class V3
{
public:
    union
    {
        struct { float x, y, z; };
        float xyz[3];
    };

    V3() : x(0), y(0), z(0) {}
    V3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    V3(const V3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}

    V3 operator - () const { return V3(-x, -y, -z); }

    V3 operator + (const V3 &vec) const { return V3(x + vec.x, y + vec.y, z + vec.z); }
    V3 operator - (const V3 &vec) const { return V3(x - vec.x, y - vec.y, z - vec.z); }
    V3 operator * (const V3 &vec) const { return V3(x * vec.x, y * vec.y, z * vec.z); }
    V3 operator / (const V3 &vec) const { return V3(x / vec.x, y / vec.y, z / vec.z); }
    V3 operator + (float s) const { return V3(x + s, y + s, z + s); }
    V3 operator - (float s) const { return V3(x - s, y - s, z - s); }
    V3 operator * (float s) const { return V3(x * s, y * s, z * s); }
    V3 operator / (float s) const { return V3(x / s, y / s, z / s); }

    friend V3 operator + (float s, const V3 &vec) { return V3(s + vec.x, s + vec.y, s + vec.z); }
    friend V3 operator - (float s, const V3 &vec) { return V3(s - vec.x, s - vec.y, s - vec.z); }
    friend V3 operator * (float s, const V3 &vec) { return V3(s * vec.x, s * vec.y, s * vec.z); }
    friend V3 operator / (float s, const V3 &vec) { return V3(s / vec.x, s / vec.y, s / vec.z); }

    V3 &operator += (const V3 &vec) { return *this = *this + vec; }
    V3 &operator -= (const V3 &vec) { return *this = *this - vec; }
    V3 &operator *= (const V3 &vec) { return *this = *this * vec; }
    V3 &operator /= (const V3 &vec) { return *this = *this / vec; }
    V3 &operator += (float s) { return *this = *this + s; }
    V3 &operator -= (float s) { return *this = *this - s; }
    V3 &operator *= (float s) { return *this = *this * s; }
    V3 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const V3 &vec) const { return x == vec.x && y == vec.y && z == vec.z; }
    bool operator != (const V3 &vec) const { return x != vec.x || y != vec.y || z != vec.z; }

    float lengthSquared() const { return x * x + y * y + z * z; }
    float length() const { return sqrtf(lengthSquared()); }
    float dot(const V3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; }
    V3 cross(const V3 &vec) const { return V3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
    V3 unit() const { return *this / length(); }
    void normalize() { *this = unit(); }
    static V3 lerp(const V3 &a, const V3 &b, float percent) { return a + (b - a) * percent; }

    float min() const { return ::min(x, ::min(y, z)); }
    float max() const { return ::max(x, ::max(y, z)); }
    static V3 min(const V3 &a, const V3 &b) { return V3(::min(a.x, b.x), ::min(a.y, b.y), ::min(a.z, b.z)); }
    static V3 max(const V3 &a, const V3 &b) { return V3(::max(a.x, b.x), ::max(a.y, b.y), ::max(a.z, b.z)); }

    V2 toAngles() const { return V2(atan2f(z, x), asinf(y / length())); }
    static V3 fromAngles(float theta, float phi) { return V3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi)); }
    static V3 uniform() { return fromAngles(frand() * M_2PI, asinf(frand() * 2 - 1)); }

    static const V3 X;
    static const V3 Y;
    static const V3 Z;
};

inline std::ostream &operator << (std::ostream &out, const V2 &v) { return out << "<" << v.x << ", " << v.y << ">"; }
inline std::ostream &operator << (std::ostream &out, const V3 &v) { return out << "<" << v.x << ", " << v.y << ", " << v.z << ">"; }

#endif // VECTOR_H
