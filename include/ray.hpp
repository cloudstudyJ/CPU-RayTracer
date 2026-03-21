#pragma once

#include "vec3.hpp"

struct Ray {
    public:
        Vec3 at(const f32 t) const noexcept { return (origin + direction * t); }

    public:
        Vec3 origin;
        Vec3 direction;
};