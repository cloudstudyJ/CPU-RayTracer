#pragma once

#include "objects/object.hpp"

class Rectangle: public Object {
    public:
        Rectangle() noexcept;
        Rectangle(const Transform& transform, std::shared_ptr<const Material> material = {}) noexcept;
        Rectangle(const Vec3& pos, const Vec3& u, const Vec3& v, std::shared_ptr<const Material> material = {}) noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

        Vec3 edgeU() const noexcept { return mU; }
        Vec3 edgeV() const noexcept { return mV; }

    private:
        void build(const Vec3& pos, const Vec3& u, const Vec3& v) noexcept;

    private:
        Vec3 mU;
        Vec3 mV;
};