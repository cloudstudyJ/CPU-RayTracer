#include "material/metal.hpp"
#include "bxdfs/ggxReflection.hpp"
#include "bxdfs/lambertian.hpp"
#include "bxdfs/microfacet.hpp"
#include "sampling/random.hpp"
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
    const SurfaceData surface = surfaceData(hitResult.uv);
    const Vec3        half    = (wo + wi).normalized();
    const Vec3        F       = microfacet::fresnelSchlick(surface.f0, Math::abs(Vec3::dot(wo, half)));

    return (
        (Lambertian(surface.diffuse).f(wo, wi) * (Vec3{ 1.0f, 1.0f, 1.0f } - F)) +
        GGXReflection(surface.f0, surface.roughness).f(wo, wi)
    );
}
f32 Metal::pdf(const HitResult& hitResult, const Vec3& wo, const Vec3& wi) const {
    const SurfaceData surface   = surfaceData(hitResult.uv);
    const f32         specLobeQ = specLobeProb(surface.f0, surface.diffuse);

    return (
        (GGXReflection(surface.f0, surface.roughness).pdf(wo, wi) * specLobeQ) +
        (Lambertian(surface.diffuse).pdf(wo, wi) * (1 - specLobeQ))
    );
}
BxDFSample Metal::sample(const HitResult& hitResult, const Vec3& wo) const {
    const SurfaceData surface   = surfaceData(hitResult.uv);
    const f32         specLobeQ = specLobeProb(surface.f0, surface.diffuse);

    const GGXReflection ggx        = GGXReflection(surface.f0, surface.roughness);
    const Lambertian    lambertian = Lambertian(surface.diffuse);

    BxDFSample s = (sampling::uniform() < specLobeQ)
        ? ggx.sample(wo)
        : lambertian.sample(wo);

    if (s.pdf <= 0.0f)
        return {};

    const Vec3 half = (wo + s.wi).normalized();
    const Vec3 F    = microfacet::fresnelSchlick(surface.f0, Math::abs(Vec3::dot(wo, half)));

    s.f   = ggx.f(wo, s.wi) + (lambertian.f(wo, s.wi) * (Vec3{ 1.0f, 1.0f, 1.0f } - F));
    s.pdf = (ggx.pdf(wo, s.wi) * specLobeQ) + (lambertian.pdf(wo, s.wi) * (1 - specLobeQ));

    return s;
}

f32 Metal::specLobeProb(const Vec3& f0, const Vec3& diffuse) const noexcept {
    const f32 ySpec = Utility::luminance(f0);
    const f32 yDiff = Utility::luminance(diffuse);
    const f32 ySum  = ySpec + yDiff;
    const f32 prob  = (ySum > 0.0f)
        ? (ySpec / ySum)
        : 0.5f;

    return prob;
}
auto Metal::surfaceData(const Vec2& uv) const noexcept -> SurfaceData {
    const Vec3 baseColor = mBaseColor->sample(uv);
    const Vec3 orm       = mORM->sample(uv);
    const f32  metal     = Utility::clamp(mMetallic * orm.b, 0.0f, 1.0f);

    return {
        .diffuse   = baseColor * (1 - metal),
        .orm       = orm,
        .f0        = Vec3::lerp({ 0.04f, 0.04f, 0.04f }, baseColor, metal),
        .metallic  = metal,
        .roughness = Utility::clamp(mRoughness * orm.g, 0.0f, 1.0f)
    };
}