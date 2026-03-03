#pragma once

#include "types.hpp"
#include "sphere.hpp"

struct HitResult {
    bool isHit{};

    float32 tIn{}, tOut{};

    const Sphere* object{};
};