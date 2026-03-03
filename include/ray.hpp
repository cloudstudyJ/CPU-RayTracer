#pragma once

#include "vec3.hpp"

class Ray {
    public:
        Ray() noexcept {}
        Ray(const Vec3& origin, const Vec3& dir) noexcept: mOrigin(origin), mDir(dir) {}

        Vec3 origin() const noexcept { return mOrigin; }
        Vec3 direction() const noexcept { return mDir; }

    private:
        Vec3 mOrigin;
        Vec3 mDir;
};