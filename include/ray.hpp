#pragma once

#include "math/vec3.hpp"
#include "math/math.hpp"

struct Ray {
    public:
        constexpr Vec3 at(const f32 t) const noexcept;

    public:
        Vec3 origin;
        Vec3 direction;
};

constexpr Vec3 Ray::at(const f32 t) const noexcept {
    return origin + direction * t;
}

struct TraversalRay {
    public:
        explicit TraversalRay(const Ray& ray) noexcept {
            for (u32 i = 0; i < 3; ++i) {
                f32 dir = ray.direction.data[i];

                if (Math::abs(dir) <= 1.0E-12f)
                    dir = (dir < 0.0f) ? -1.0E-12f : 1.0E-12f;

                const f32 invDirection = 1 / dir;

                invDir.data[i]    = invDirection;
                orgInvDir.data[i] = ray.origin.data[i] * invDirection;
            }
        }

    public:
        Vec3 invDir;
        Vec3 orgInvDir;
};