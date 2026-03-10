#pragma once

#include "vec3.hpp"

struct AABB {
    public:
        AABB() noexcept;
        AABB(const Vec3& min, const Vec3& max) noexcept;

        static AABB unite(const AABB& a, const AABB& b) noexcept;

    public:
        Vec3 min;
        Vec3 max;
};