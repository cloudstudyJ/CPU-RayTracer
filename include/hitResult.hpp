#pragma once

#include "vec3.hpp"
#include "constants.hpp"

struct HitResult {
    bool isHit{};

    float32 hitT{INF_F32};

    Vec3 hitPoint;
    Vec3 normal;
};