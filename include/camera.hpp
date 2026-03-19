#pragma once

#include "vec3.hpp"

struct Camera {
    float32 yfov;

    Vec3 pos;
    Vec3 look;
};