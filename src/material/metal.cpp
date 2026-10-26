#include "material/metal.hpp"
#include "bxdfs/ggxReflection.hpp"
#include "texture.hpp"

Metal::Metal(std::shared_ptr<Texture> texture, const f32 roughness) noexcept
    : Material()
    , mRoughness(roughness)
    , mTexture(std::move(texture)) { }

Vec3 Metal::evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mTexture->sample(hitResult.uv), mRoughness).f(wo, wi);
}
f32 Metal::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return GGXReflection(mTexture->sample(hitResult.uv), mRoughness).pdf(wo, wi);
}
BxDFSample Metal::sample(const HitResult& hitResult, const Vec3& wo) const {
    return GGXReflection(mTexture->sample(hitResult.uv), mRoughness).sample(wo);
}