#pragma once

#include "texture.hpp"

class SolidTexture: public Texture {
    public:
        SolidTexture() noexcept = default;
        SolidTexture(const Vec3& albedo) noexcept: mAlbedo(albedo) {}

        Vec3 color(const f32 u, const f32 v) const noexcept override { return mAlbedo; }

    private:
        Vec3 mAlbedo;
};