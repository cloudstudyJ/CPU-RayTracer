#pragma once

#include "material/material.hpp"

#include <memory>

// fwd
class Texture;

class Metal: public Material {
    Metal()                            = delete;
    Metal(const Metal&)                = delete;
    Metal(Metal&&) noexcept            = delete;
    Metal& operator=(const Metal&)     = delete;
    Metal& operator=(Metal&&) noexcept = delete;

    public:
        explicit Metal(std::shared_ptr<Texture> texture, const f32 roughness) noexcept;
        ~Metal() noexcept override = default;

        BxDFSample interact(const HitResult& hitResult, const Vec3& wo) const override;

    private:
        f32                      mRoughness{};
        std::shared_ptr<Texture> mTexture;
};