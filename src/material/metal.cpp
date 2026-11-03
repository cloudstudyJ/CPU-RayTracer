#include "material/metal.hpp"
#include "bxdfs/ggxReflection.hpp"
#include "texture.hpp"
#include "utility.hpp"

Metal::Metal(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> orm, const f32 roughness) noexcept
    : Material()
    , mRoughness(roughness)
    , mBaseColor(std::move(baseColor))
    , mORM(std::move(orm)) { }

Vec3 Metal::evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).f(wo, wi);
}
f32 Metal::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).pdf(wo, wi);
}
BxDFSample Metal::sample(const HitResult& hitResult, const Vec3& wo) const {
    return GGXReflection(mBaseColor->sample(hitResult.uv), roughness(hitResult.uv)).sample(wo);
}

const f32 Metal::roughness(const Vec2& uv) const noexcept {
    return Utility::clamp(mRoughness * mORM->sample(uv).y, 0.0f, 1.0f);
}