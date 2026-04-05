#pragma once

#include "math/vec3.hpp"

namespace Random {
    // [0.0f, 1.0f)
    f32 uniform();
    // [from, to)
    inline f32 uniform(const f32 from, const f32 to);

    // [0.0f, 1.0f)
    inline Vec3 uniformVec3();
    // [from, to)
    inline Vec3 uniformVec3(const f32 from, const f32 to);

    Vec3 uniformSphere();
    Vec3 uniformHemisphere();
    Vec3 cosWeightedHemisphere();
}

inline f32 Random::uniform(const f32 from, const f32 to) {
    return from + (to - from) * uniform();
}

inline Vec3 Random::uniformVec3() {
    return { .data = { uniform(), uniform(), uniform() } };
}
inline Vec3 Random::uniformVec3(const f32 from, const f32 to) {
    return { .data = { uniform(from, to), uniform(from, to), uniform(from, to) } };
}