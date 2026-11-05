#include "material/diffuse.hpp"
#include "bxdfs/lambertian.hpp"
#include "texture.hpp"

Diffuse::Diffuse(std::shared_ptr<Texture> baseColor, std::shared_ptr<Texture> normalMap) noexcept
    : Material(std::move(baseColor), std::move(normalMap)) { }

Vec3 Diffuse::evaluate(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return Lambertian(mBaseColor->sample(hitResult.uv)).f(wo, wi);
}
f32 Diffuse::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    return Lambertian(mBaseColor->sample(hitResult.uv)).pdf(wo, wi);
}
BxDFSample Diffuse::sample(const HitResult& hitResult, const Vec3& wo) const {
    return Lambertian(mBaseColor->sample(hitResult.uv)).sample(wo);
}