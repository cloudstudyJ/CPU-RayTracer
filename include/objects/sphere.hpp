#pragma once

#include "objects/object.hpp"

class Sphere: public Object {
    public:
        Sphere() noexcept;
        Sphere(const Vec3& pos, const f32 r, std::shared_ptr<const Material> material = {}) noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

        f32 radius() const noexcept { return mRadius; }

    private:
        f32 mRadius;
};