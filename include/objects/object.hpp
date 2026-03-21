#pragma once

#include "hitResult.hpp"
#include "aabb.hpp"
#include "material.hpp"
#include "ray.hpp"

class Object {
    public:
        virtual ~Object() noexcept = default;

        virtual HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const = 0;

        Vec3 pos() const noexcept { return mPos; }

        AABB& aabb() noexcept { return mAABB; }
        const AABB& aabb() const noexcept { return mAABB; }

        const Material* material() const noexcept { return mMaterial; }

    protected:
        Object() noexcept = default;
        Object(const Vec3& pos, const Material* material) noexcept: mPos(pos), mMaterial(material) {}

    protected:
        Vec3 mPos;

        AABB mAABB;
        const Material* mMaterial;
};