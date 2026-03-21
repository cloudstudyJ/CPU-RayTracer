#pragma once

#include "vec3.hpp"
#include "constants.hpp"

// fwd
struct Material;

struct HitResult {
    bool isHit{};
    bool isRayOppositeNormal{};

    f32 hitT{INF_F32};
    f32 u{};
    f32 v{};

    Vec3 hitPoint;
    Vec3 normal;

    const Material* material{};
};