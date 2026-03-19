#pragma once

#include "vec3.hpp"

namespace Random {
    // [0.0f, 1.0f)
    float32 uniform();
    // [from, to)
    inline float32 uniform(const float32 from, const float32 to) { return (from + (to - from) * uniform()); }

    // [0.0f, 1.0f)
    inline Vec3 uniformVec3() { return {uniform(), uniform(), uniform()}; }
    // [from, to)
    inline Vec3 uniformVec3(const float32 from, const float32 to) { return {uniform(from, to), uniform(from, to), uniform(from, to)}; }

    Vec3 uniformSphere();
    Vec3 uniformHemisphere(const Vec3& n);
}