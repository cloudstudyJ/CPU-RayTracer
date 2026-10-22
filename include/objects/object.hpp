#pragma once

#include "hitResult.hpp"
#include "aabb.hpp"
#include "material/material.hpp"
#include "components/transform.hpp"

#include <memory>

// fwd
struct Ray;

class Object {
    public:
        virtual ~Object() noexcept = default;

        virtual HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const = 0;

        Transform transform() const noexcept { return mTransform; }

        AABB& aabb() noexcept { return mAABB; }
        const AABB& aabb() const noexcept { return mAABB; }

        const Material* material() const noexcept { return mMaterial.get(); }

    protected:
        Object() noexcept = default;
        Object(const Transform& transform, std::shared_ptr<const Material> material) noexcept: mTransform(transform), mMaterial(material) {}

    protected:
        Transform mTransform;

        AABB mAABB;
        std::shared_ptr<const Material> mMaterial;
};