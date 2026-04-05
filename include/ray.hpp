#pragma once

#include "math/vec3.hpp"

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