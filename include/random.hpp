#pragma once

#include "vec3.hpp"

namespace Random {
    // [0.0f, 1.0f)
    f32 uniform();
    // [from, to)
    inline f32 uniform(const f32 from, const f32 to) { return (from + (to - from) * uniform()); }

    // [0.0f, 1.0f)
    inline Vec3 uniformVec3() { return {uniform(), uniform(), uniform()}; }
    // [from, to)
    inline Vec3 uniformVec3(const f32 from, const f32 to) { return {uniform(from, to), uniform(from, to), uniform(from, to)}; }

    Vec3 uniformSphere();
    Vec3 uniformHemisphere();
}