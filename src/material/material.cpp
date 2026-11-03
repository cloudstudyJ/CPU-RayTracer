#include "material/material.hpp"
#include "texture.hpp"

Material::Material(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> normalMap) noexcept
    : mBaseColor(std::move(baseColor))
    , mNormalMap(std::move(normalMap)) { }

Vec3 Material::normal(const Vec2& uv) const noexcept {
    return (mNormalMap->sample(uv) * 2.0f - 1.0f);
}
bool Material::hasNormalMap() const noexcept {
    return mNormalMap != nullptr;
}