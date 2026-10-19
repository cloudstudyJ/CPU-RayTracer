#pragma once

#include "types.hpp"
#include "math/vec2.hpp"

namespace sampling {
    f32  uniform();     // [0.0f, 1.0f)
    Vec2 uniform2D();   // [0.0f, 1.0f)^2

    f32  uniform(const f32 from, const f32 to);     // [from, to)
    Vec2 uniform2D(const f32 from, const f32 to);   // [from, to)^2
}