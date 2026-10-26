#include "material/diffuse.hpp"
#include "bxdfs/lambertian.hpp"
#include "texture.hpp"

Diffuse::Diffuse(std::shared_ptr<Texture> texture) noexcept
    : Material()
    , mTexture(std::move(texture)) { }

Vec3 Diffuse::evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return Lambertian(mTexture->sample(hitResult.uv)).f(wo, wi);
}
f32 Diffuse::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return Lambertian(mTexture->sample(hitResult.uv)).pdf(wo, wi);
}
BxDFSample Diffuse::sample(const HitResult& hitResult, const Vec3& wo) const {
    return Lambertian(mTexture->sample(hitResult.uv)).sample(wo);
}