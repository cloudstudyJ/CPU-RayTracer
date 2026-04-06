#pragma once

#include "math/vec3.hpp"

struct Camera {
    Vec3 position;
    Vec3 look;

    f32 fovY{};
};