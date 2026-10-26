#include "solidTexture.hpp"

SolidTexture::SolidTexture(const Vec3& albedo) noexcept
    : Texture()
    , mAlbedo(albedo) { }

Vec3 SolidTexture::sample(const Vec2& uv) const noexcept {
    return mAlbedo;
}