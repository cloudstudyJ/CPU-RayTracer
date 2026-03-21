#pragma once

#include "objects/quad.hpp"

class Box: public Object {
    public:
        Box() noexcept;
        Box(const Vec3& a, const Vec3& b, const Material* material) noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

    private:
        Quad mFaces[6];
};