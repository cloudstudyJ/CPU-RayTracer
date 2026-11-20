#pragma once

#include "objects/object.hpp"

class Rectangle: public Object {
    public:
        Rectangle() noexcept;
        Rectangle(const Transform& transform, std::shared_ptr<const Material> material = {}) noexcept;
        Rectangle(const Vec3& pos, const Vec3& u, const Vec3& v, std::shared_ptr<const Material> material = {}) noexcept;

        bool      isAnyHit(const Ray& ray, const f32 tMin, const f32 tMax) const noexcept override;
        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const override;

        Vec3 edgeU() const noexcept;
        Vec3 edgeV() const noexcept;
        Vec3 tangent() const noexcept;
        Vec3 bitangent() const noexcept;
        Vec3 normal() const noexcept;

    private:
        void build(const Vec3& pos, const Vec3& u, const Vec3& v) noexcept;

    private:
        Vec3 mU;
        Vec3 mV;
        Vec3 mTangent;
        Vec3 mBitangent;
        Vec3 mNormal;
};