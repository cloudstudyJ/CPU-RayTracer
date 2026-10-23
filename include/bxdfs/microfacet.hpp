#pragma once

#include "types.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

namespace microfacet {
    f32  ggxSmithLambda(const Vec3& w, const f32 alpha) noexcept;
    f32  ggxSmithG1(const Vec3& w, const f32 alpha) noexcept;
    f32  ggxSmithG2(const Vec3& wo, const Vec3& wi, const f32 alpha) noexcept;
    f32  ggxNDF(const Vec3& h, const f32 alpha) noexcept;
    Vec3 ggxVNDF(const Vec3& wo, Vec2 uv, const f32 alpha) noexcept;
    Vec3 fresnelSchlick(const Vec3& f0, const f32 cos) noexcept;
}