#pragma once

#include "vec3.hpp"
#include "hitResult.hpp"
#include "ray.hpp"

class Object {
    public:
        virtual HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) = 0;

        Vec3 pos() const noexcept { return mPos; }

    protected:
        Object() noexcept {}
        Object(const Vec3& pos) noexcept: mPos(pos) {}

    protected:
        Vec3 mPos;
};