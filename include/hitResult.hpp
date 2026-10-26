#pragma once

#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "constants.hpp"

// fwd
struct Object;

struct HitResult {
    bool          isHit      {};
    bool          isFrontFace{};
    f32           hitT       { INF_F32 };
    Vec2          uv;
    Vec3          hitPoint;
    Vec3          normal;
    const Object* object     {};
};