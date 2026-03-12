#pragma once

#include "texture.hpp"

class SolidTexture: public Texture {
    public:
        SolidTexture() noexcept = default;
        SolidTexture(const Vec3& albedo) noexcept: mAlbedo(albedo) {}

        Vec3 color(const float32 u, const float32 v) const noexcept override { return mAlbedo; }

    private:
        Vec3 mAlbedo;
};