#pragma once

#include "math/vec3.hpp"

struct ONB {
    ONB() = delete;

    public:
        explicit inline ONB(const Vec3& normal) noexcept;

        constexpr Vec3 toLocal(const Vec3& p) const noexcept;
        constexpr Vec3 toWorld(const Vec3& p) const noexcept;

    public:
        Vec3 t;
        Vec3 b;
        Vec3 n;
};

inline ONB::ONB(const Vec3& normal) noexcept {
    n = normal.normalized();

    Vec3 a = (
        (Math::abs(n.y) >= 0.999f)
            ? Vec3{ 1.0f, 0.0f, 0.0f }
            : Vec3{ 0.0f, 1.0f, 0.0f }
    );

    t = Vec3::cross(a, n).normalized();
    b = Vec3::cross(n, t);
}

constexpr Vec3 ONB::toLocal(const Vec3& p) const noexcept {
    return {
        Vec3::dot(t, p),
        Vec3::dot(b, p),
        Vec3::dot(n, p)
    };
}
constexpr Vec3 ONB::toWorld(const Vec3& p) const noexcept {
    return ((t * p.x) + (b * p.y) + (n * p.z));
}