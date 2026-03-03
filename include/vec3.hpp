#pragma once

#include "types.hpp"

struct Vec3 {
    public:
        Vec3 operator*(const float& s) const noexcept { return {x * s, y * s, z * s}; }
        Vec3 operator/(const float& s) const { return {x / s, y / s, z / s}; }
        Vec3 operator+(const Vec3& v) const noexcept { return {x + v.x, y + v.y, z + v.z}; }
        Vec3 operator-(const Vec3& v) const noexcept { return {x - v.x, y - v.y, z - v.z}; }

        union {
            float32 data[3]{};

            struct { float32 x, y, z; };
            struct { float32 r, g, b; };
        };
};