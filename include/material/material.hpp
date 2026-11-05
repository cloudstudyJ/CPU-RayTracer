#pragma once

#include "bxdfs/bxdf.hpp"
#include "hitResult.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"

#include <memory>

// fwd
class Texture;

class Material {
    Material(const Material&)                = delete;
    Material(Material&&) noexcept            = delete;
    Material& operator=(const Material&)     = delete;
    Material& operator=(Material&&) noexcept = delete;

    public:
        virtual ~Material() noexcept = default;

        Vec3 normal(const Vec2& uv) const noexcept;
        bool hasNormalMap() const noexcept;

        virtual Vec3       evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const = 0;
        virtual f32        pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const = 0;
        virtual BxDFSample sample(const HitResult& hitResult, const Vec3& wo) const = 0;

    protected:
        Material() noexcept = default;
        explicit Material(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> normalMap) noexcept;

    protected:
        std::shared_ptr<Texture> mBaseColor;
        std::shared_ptr<Texture> mNormalMap;
};