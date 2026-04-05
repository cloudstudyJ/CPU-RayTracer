#pragma once

#include "math/vec3.hpp"

struct Camera {
    Vec3 pos;
    Vec3 look;

    f32 yfov{};
};