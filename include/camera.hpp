#pragma once

#include "types.hpp"
#include "vec3.hpp"

struct Camera {
    float32 yFOV{90.0f};

    Vec3 pos;
};