#include "material/metal.hpp"
#include "bxdfs/ggxReflection.hpp"
#include "texture.hpp"
#include "utility.hpp"

Metal::Metal(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> orm, const f32 metallic, const f32 roughness) noexcept
    : Material(std::move(baseColor), std::move(normalMap))
    , mMetallic(metallic)
    , mRoughness(roughness)
    , mORM(std::move(orm)) { }

f32 Metal::metallic(const Vec2& uv) const noexcept {
    return Utility::clamp(mMetallic * mORM->sample(uv).b, 0.0f, 1.0f);
}
f32 Metal::roughness(const Vec2& uv) const noexcept {
    return Utility::clamp(mRoughness * mORM->sample(uv).g, 0.0f, 1.0f);
}

Vec3 Metal::evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).f(wo, wi);
}
f32 Metal::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).pdf(wo, wi);
}
BxDFSample Metal::sample(const HitResult& hitResult, const Vec3& wo) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).sample(wo);
}