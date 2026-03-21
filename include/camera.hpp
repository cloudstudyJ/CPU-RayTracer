#pragma once

#include "vec3.hpp"

struct Camera {
    f32 yfov;

    Vec3 pos;
    Vec3 look;
};