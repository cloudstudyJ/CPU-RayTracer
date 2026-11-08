#pragma once

#include "material/material.hpp"

class Metal: public Material {
    Metal()                            = delete;
    Metal(const Metal&)                = delete;
    Metal(Metal&&) noexcept            = delete;
    Metal& operator=(const Metal&)     = delete;
    Metal& operator=(Metal&&) noexcept = delete;

    public:
        explicit Metal(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> orm, const f32 metallic = 1.0f, const f32 roughness = 1.0f) noexcept;
        ~Metal() noexcept override = default;

        f32 metallic(const Vec2& uv) const noexcept;
        f32 roughness(const Vec2& uv) const noexcept;

        Vec3       evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const override;
        f32        pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const override;
        BxDFSample sample(const HitResult& hitResult, const Vec3& wo) const override;

    private:
        f32                      mRoughness;
        f32                      mMetallic;
        std::shared_ptr<Texture> mORM;
};