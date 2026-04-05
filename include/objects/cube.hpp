#pragma once

#include "objects/rectangle.hpp"

class Cube: public Object {
    public:
        Cube() noexcept;
        Cube(const Transform& transform, std::shared_ptr<const Material> material = {}) noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

    private:
        Rectangle mFaces[6];
};