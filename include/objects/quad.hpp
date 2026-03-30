#pragma once

#include "objects/object.hpp"

class Quad: public Object {
    public:
        Quad() noexcept;
        Quad(const Vec3& pos, const Vec3& a, const Vec3& b, std::shared_ptr<const Material> material = {}) noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

        Vec3 edgeU() const noexcept { return mU; }
        Vec3 edgeV() const noexcept { return mV; }

    private:
        Vec3 mU;
        Vec3 mV;
};