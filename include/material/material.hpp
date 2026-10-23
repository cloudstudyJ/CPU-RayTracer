#pragma once

#include "bxdfs/bxdf.hpp"
#include "hitResult.hpp"
#include "math/vec3.hpp"

class Material {
    Material(const Material&)                = delete;
    Material(Material&&) noexcept            = delete;
    Material& operator=(const Material&)     = delete;
    Material& operator=(Material&&) noexcept = delete;

    public:
        virtual ~Material() noexcept = default;

        virtual Vec3       evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const = 0;
        virtual f32        pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const = 0;
        virtual BxDFSample sample(const HitResult& hitResult, const Vec3& wo) const = 0;

    protected:
        Material() noexcept = default;
};