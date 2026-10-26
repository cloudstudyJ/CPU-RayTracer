#pragma once

#include "texture.hpp"

class SolidTexture: public Texture {
    SolidTexture(const SolidTexture&)                = delete;
    SolidTexture(SolidTexture&&) noexcept            = delete;
    SolidTexture& operator=(const SolidTexture&)     = delete;
    SolidTexture& operator=(SolidTexture&&) noexcept = delete;

    public:
        SolidTexture() noexcept = default;
        explicit SolidTexture(const Vec3& albedo) noexcept;
        ~SolidTexture() noexcept override = default;

        Vec3 sample(const Vec2& uv) const noexcept override;

    private:
        Vec3 mAlbedo;
};