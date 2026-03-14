#pragma once

#include "objects/object.hpp"

class Quad: public Object {
    public:
        Quad() noexcept;
        Quad(const Vec3& pos, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material) noexcept;

        HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const override;

        Vec3 edgeU() const noexcept { return mU; }
        Vec3 edgeV() const noexcept { return mV; }

    private:
        Vec3 mU;
        Vec3 mV;
};