#pragma once

#include "math/math.hpp"

struct Vec3 {
    public:
        constexpr Vec3& operator+=(const f32 s) noexcept;
        constexpr Vec3& operator-=(const f32 s) noexcept;
        constexpr Vec3& operator*=(const f32 s) noexcept;
        constexpr Vec3& operator/=(const f32 s);
        constexpr Vec3& operator+=(const Vec3& v) noexcept;
        constexpr Vec3& operator-=(const Vec3& v) noexcept;
        constexpr Vec3& operator*=(const Vec3& v) noexcept;
        constexpr Vec3& operator/=(const Vec3& v);

        constexpr Vec3 operator+(const f32 s) const noexcept;
        constexpr Vec3 operator-(const f32 s) const noexcept;
        constexpr Vec3 operator*(const f32 s) const noexcept;
        constexpr Vec3 operator/(const f32 s) const;
        constexpr Vec3 operator+(const Vec3& v) const noexcept;
        constexpr Vec3 operator-(const Vec3& v) const noexcept;
        constexpr Vec3 operator*(const Vec3& v) const noexcept;
        constexpr Vec3 operator/(const Vec3& v) const;

        constexpr Vec3 operator-() const noexcept;

        inline    f32 length()       const noexcept;
        constexpr f32 lengthSquare() const noexcept;

        inline Vec3& normalize()  noexcept;
        inline Vec3  normalized() const noexcept;

        constexpr bool isZero() const noexcept;

        static constexpr f32  dot(const Vec3& a, const Vec3& b)   noexcept;
        static constexpr Vec3 cross(const Vec3& a, const Vec3& b) noexcept;

        static constexpr Vec3 reflect(const Vec3& v, const Vec3& n) noexcept;
        static constexpr Vec3 lerp(const Vec3& a, const Vec3& b, const f32 t) noexcept;

    public:
        union {
            f32 data[3]{};

            struct { f32 x, y, z; };
            struct { f32 r, g, b; };
            struct { f32 u, v, w; };
        };
};

constexpr Vec3& Vec3::operator+=(const f32 s) noexcept {
    x += s;
    y += s;
    z += s;

    return *this;
}
constexpr Vec3& Vec3::operator-=(const f32 s) noexcept {
    x -= s;
    y -= s;
    z -= s;

    return *this;
}
constexpr Vec3& Vec3::operator*=(const f32 s) noexcept {
    x *= s;
    y *= s;
    z *= s;

    return *this;
}
constexpr Vec3& Vec3::operator/=(const f32 s) {
    x /= s;
    y /= s;
    z /= s;

    return *this;
}
constexpr Vec3& Vec3::operator+=(const Vec3& v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}
constexpr Vec3& Vec3::operator-=(const Vec3& v) noexcept {
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}
constexpr Vec3& Vec3::operator*=(const Vec3& v) noexcept {
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}
constexpr Vec3& Vec3::operator/=(const Vec3& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}

constexpr Vec3 Vec3::operator+(const f32 s) const noexcept {
    return { .data = { x + s, y + s, z + s } };
}
constexpr Vec3 Vec3::operator-(const f32 s) const noexcept {
    return { .data = { x - s, y - s, z - s } };
}
constexpr Vec3 Vec3::operator*(const f32 s) const noexcept {
    return { .data = { x * s, y * s, z * s } };
}
constexpr Vec3 Vec3::operator/(const f32 s) const {
    return { .data = { x / s, y / s, z / s } };
}
constexpr Vec3 Vec3::operator+(const Vec3& v) const noexcept {
    return { .data = { x + v.x, y + v.y, z + v.z } };
}
constexpr Vec3 Vec3::operator-(const Vec3& v) const noexcept {
    return { .data = { x - v.x, y - v.y, z - v.z } };
}
constexpr Vec3 Vec3::operator*(const Vec3& v) const noexcept {
    return { .data = { x * v.x, y * v.y, z * v.z } };
}
constexpr Vec3 Vec3::operator/(const Vec3& v) const {
    return { .data = { x / v.x, y / v.y, z / v.z } };
}

constexpr Vec3 Vec3::operator-() const noexcept {
    return { .data = { -x, -y, -z } };
}

inline f32 Vec3::length() const noexcept {
    return Math::sqrt(lengthSquare());
}
constexpr f32 Vec3::lengthSquare() const noexcept {
    return x * x + y * y + z * z;
}
inline Vec3& Vec3::normalize() noexcept {
    const f32 invLen = 1.0f / length();

    x *= invLen;
    y *= invLen;
    z *= invLen;

    return *this;
}
inline Vec3 Vec3::normalized() const noexcept {
    const f32 invLen = 1.0f / length();

    return *this * invLen;
}

constexpr bool Vec3::isZero() const noexcept {
    return lengthSquare() < 1.0E-12f;
}

constexpr f32 Vec3::dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
constexpr Vec3 Vec3::cross(const Vec3& a, const Vec3& b) noexcept {
    return {
        .data = {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        }
    };
}

constexpr Vec3 Vec3::reflect(const Vec3& v, const Vec3& n) noexcept {
    return v - n * (2.0f * Vec3::dot(v, n));
}
constexpr Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, const f32 t) noexcept {
    return a + (b - a) * t;
}