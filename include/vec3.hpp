#pragma once

#include "math.hpp"

struct Vec3 {
    public:
        inline Vec3& operator+=(const Vec3& v) noexcept;

        Vec3 operator*(const float32 s) const noexcept { return {x * s, y * s, z * s}; }
        Vec3 operator/(const float32 s) const { return {x / s, y / s, z / s}; }
        Vec3 operator+(const float32 s) const noexcept { return {x + s, y + s, z + s}; }
        Vec3 operator*(const Vec3& v) const noexcept { return {x * v.x, y * v.y, z * v.z}; }
        Vec3 operator+(const Vec3& v) const noexcept { return {x + v.x, y + v.y, z + v.z}; }
        Vec3 operator-(const Vec3& v) const noexcept { return {x - v.x, y - v.y, z - v.z}; }

        Vec3 operator-() const noexcept { return {-x, -y, -z}; }

        float32 length() const noexcept { return (Math::sqrt(lengthSquare())); }
        float32 lengthSquare() const noexcept { return (x * x + y * y + z * z); }

        Vec3 normalize() const { return {*this / length()}; }

        bool isZero() const noexcept { return (lengthSquare() < 1.0e-12f); }

        static float32 dot(const Vec3& a, const Vec3& b) noexcept { return (a.x * b.x + a.y * b.y + a.z * b.z); }
        static inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept;

        static Vec3 reflect(const Vec3& v, const Vec3& n) noexcept { return {v - n * (2 * Vec3::dot(v, n))}; }
        static Vec3 lerp(const Vec3& a, const Vec3& b, const float32 t) noexcept { return (a + (b - a) * t); }

    public:
        union {
            float32 data[3]{};

            struct { float32 x, y, z; };
            struct { float32 r, g, b; };
            struct { float32 u, v, w; };
        };
};

inline Vec3& Vec3::operator+=(const Vec3& v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

inline Vec3 Vec3::cross(const Vec3& a, const Vec3& b) noexcept {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}