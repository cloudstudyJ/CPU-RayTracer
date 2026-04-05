#pragma once

#include "math/vec3.hpp"

struct ONB {
    ONB() = delete;

    public:
        inline ONB(const Vec3& normal) noexcept;

        constexpr Vec3 transform(const Vec3& p) const noexcept;

    public:
        Vec3 t;
        Vec3 b;
        Vec3 n;
};

inline ONB::ONB(const Vec3& normal) noexcept {
    n = normal.normalized();

    Vec3 a = (
        (Math::abs(normal.y) >= 0.999f)
        ? Vec3{ .data = { 1.0f, 0.0f, 0.0f } }
        : Vec3{ .data = { 0.0f, 1.0f, 0.0f } }
    );

    t = Vec3::cross(a, normal).normalized();
    b = Vec3::cross(normal, t);
}

constexpr Vec3 ONB::transform(const Vec3& p) const noexcept {
    return t * p.x + b * p.y + n * p.z;
}