#pragma once

#include "material/material.hpp"

#include <memory>

// fwd
class Texture;

class Diffuse: public Material {
    Diffuse()                              = delete;
    Diffuse(const Diffuse&)                = delete;
    Diffuse(Diffuse&&) noexcept            = delete;
    Diffuse& operator=(const Diffuse&)     = delete;
    Diffuse& operator=(Diffuse&&) noexcept = delete;

    public:
        explicit Diffuse(std::shared_ptr<Texture> texture) noexcept;
        ~Diffuse() noexcept override = default;

        BxDFSample interact(const HitResult& hitResult, const Vec3& wo) const override;

    private:
        std::shared_ptr<Texture> mTexture;
};