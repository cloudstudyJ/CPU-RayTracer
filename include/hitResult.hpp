#pragma once

#include "vec3.hpp"

struct HitResult {
    bool isHit{};

    float32 hitT{};

    Vec3 hitPoint;
    Vec3 normal;
};