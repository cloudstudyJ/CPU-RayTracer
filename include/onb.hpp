#pragma once

#include "vec3.hpp"
#include "math.hpp"

struct ONB {
    ONB() = delete;

    public:
        inline ONB(const Vec3& normal) noexcept;

        Vec3 transform(const Vec3& p) const noexcept { return ((t * p.x) + (b * p.y) + (n * p.z)); }

    public:
        Vec3 t;
        Vec3 b;
        Vec3 n;
};

inline ONB::ONB(const Vec3& normal) noexcept {
    n = normal.normalize();

    Vec3 a = (
        (Math::abs(normal.y) >= 0.999f)
        ? Vec3{1.0f, 0.0f, 0.0f}
        : Vec3{0.0f, 1.0f, 0.0f}
    );

    t = Vec3::cross(a, normal).normalize();
    b = Vec3::cross(normal, t);
}