#pragma once

#include "math/vec3.hpp"
#include "math/vec4.hpp"

struct Transform {
    alignas(16) Vec3 position;
    alignas(16) Vec4 rotation;
    alignas(16) Vec3 scale;
};