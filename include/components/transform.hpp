#pragma once

#include "vec3.hpp"

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale   {1.0f, 1.0f, 1.0f};
};