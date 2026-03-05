#pragma once

#include "vec3.hpp"

class Random {
    public:
        // [0.0f, 1.0f)
        static float32 uniform();
        // [from, to)
        static float32 uniform(const float32 from, const float32 to) { return (from + (to - from) * uniform()); }

        static Vec3 uniformVec3(const float32 from, const float32 to);
        static Vec3 uniformSphere();
        static Vec3 uniformHemisphere(const Vec3& normal);
};