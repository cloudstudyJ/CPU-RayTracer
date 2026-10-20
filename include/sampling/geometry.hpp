#pragma once

#include "math/vec3.hpp"

namespace sampling {
    Vec3 uniformSphere(const f32 radius = 1.0f);
    Vec3 uniformHemisphere(const f32 radius = 1.0f);
    Vec3 uniformRectangle(const Vec3& edgeU, const Vec3& edgeV);
}