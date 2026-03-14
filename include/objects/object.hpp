#pragma once

#include "hitResult.hpp"
#include "aabb.hpp"
#include "materials/material.hpp"
#include "ray.hpp"

#include <memory>

class Object {
    public:
        virtual ~Object() noexcept = default;

        virtual HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const = 0;

        Vec3 pos() const noexcept { return mPos; }

        Material* material() noexcept { return mMaterial.get(); }
        const Material* material() const noexcept { return mMaterial.get(); }

        AABB& aabb() noexcept { return mAABB; }
        const AABB& aabb() const noexcept { return mAABB; }

    protected:
        Object() noexcept = default;
        Object(const Vec3& pos, std::shared_ptr<Material> material) noexcept: mPos(pos), mMaterial(material) {}

    protected:
        Vec3 mPos;

        AABB mAABB;
        std::shared_ptr<Material> mMaterial;
};