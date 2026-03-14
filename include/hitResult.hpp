#pragma once

#include "vec3.hpp"
#include "constants.hpp"

// fwd
struct Material;

struct HitResult {
    bool isHit{};
    bool isRayOppositeNormal{};

    float32 hitT{INF_F32};
    float32 u{};
    float32 v{};

    Vec3 hitPoint;
    Vec3 normal;

    const Material* material{};
};