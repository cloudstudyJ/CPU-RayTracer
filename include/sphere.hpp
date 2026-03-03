#pragma once

#include "types.hpp"
#include "vec3.hpp"

class Sphere {
    public:
        Sphere() noexcept: mRadius(1.0f) {}
        Sphere(const Vec3& centerPos, const float32 r = 1.0f) noexcept: mRadius(r), mCenterPos(centerPos) {}

        float32 radius() const noexcept { return mRadius; }
        Vec3 centerPos() const noexcept { return mCenterPos; }

    private:
        float32 mRadius;

        Vec3 mCenterPos;
};