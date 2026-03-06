#pragma once

#include "object.hpp"

class Sphere: public Object {
    public:
        Sphere() noexcept: Object(), mRadius(1.0f) {}
        Sphere(const Vec3& pos, const float32 r, std::shared_ptr<Material> material) noexcept: Object(pos, material), mRadius(r) {}

        HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) override;

        float32 radius() const noexcept { return mRadius; }

    private:
        float32 mRadius;
};