#pragma once

#include "vec3.hpp"

struct Camera {
    public:
        Camera() noexcept: yfov(90.0f) {}

    public:
        float32 yfov;

        Vec3 pos;
        Vec3 look;
};