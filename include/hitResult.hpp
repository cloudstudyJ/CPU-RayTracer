#pragma once

#include "vec3.hpp"
#include "constants.hpp"

// fwd
struct Material;

struct HitResult {
    bool isHit{};

    float32 hitT{INF_F32};

    Vec3 hitPoint;
    Vec3 normal;

    Material* material{};
};