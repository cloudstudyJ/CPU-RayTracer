#pragma once

#include "hitResult.hpp"
#include "materials/material.hpp"
#include "ray.hpp"

#include <memory>

class Object {
    public:
        virtual ~Object() noexcept = default;
        virtual HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) = 0;

        Vec3 pos() const noexcept { return mPos; }

    protected:
        Object() noexcept {}
        Object(const Vec3& pos, std::shared_ptr<Material> material) noexcept: mPos(pos), mMaterial(material) {}

    protected:
        Vec3 mPos;

        std::shared_ptr<Material> mMaterial;
};