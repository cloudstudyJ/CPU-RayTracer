#pragma once

#include "types.hpp"
#include "sphere.hpp"

struct HitResult {
    bool isHit{};

    float32 tMin{}, tMax{};

    const Sphere* object{};
};