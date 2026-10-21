#include "bxdfs/ggxReflection.hpp"
#include "bxdfs/microfacet.hpp"
#include "sampling/geometry.hpp"
#include "utility.hpp"

GGXReflection::GGXReflection(const Vec3& f0, const f32 roughness) noexcept
    : BxDF()
    , mF0(f0)
    , mAlpha(Utility::max(0.001f, roughness * roughness)) { }

Vec3 GGXReflection::f(const Vec3& wo, const Vec3& wi) const noexcept {
    if ((wo.z <= 0.0f) || (wi.z <= 0.0f))
        return {};

    const Vec3 half = (wo + wi).normalized();
    const f32  D    = microfacet::ggxNDF(half, mAlpha);
    const f32  G    = microfacet::ggxSmithG2(wo, wi, mAlpha);
    const Vec3 F    = microfacet::fresnelSchlick(mF0, Vec3::dot(wo, half));

    return (F * D * G) / (4 * wo.z * wi.z);
}
f32 GGXReflection::pdf(const Vec3& wo, const Vec3& wi) const noexcept {
    if ((wo.z <= 0.0f) || (wi.z <= 0.0f))
        return {};

    const Vec3 half = (wo + wi).normalized();
    const f32  D    = microfacet::ggxNDF(half, mAlpha);
    const f32  G1   = microfacet::ggxSmithG1(wo, mAlpha);

    return (D * G1) / (4 * wo.z);
}
BxDFSample GGXReflection::sample(const Vec3& wo) const {
    if (wo.z <= 0.0f)
        return {};

    const Vec3 half = microfacet::ggxVNDF(wo, sampling::uniformDisk(), mAlpha);
    const f32  cos  = Vec3::dot(wo, half);

    if (cos <= 0.0f)
        return {};

    const Vec3 wi = (half * 2 * cos) - wo;

    if (wi.z <= 0.0f)
        return {};

    return {
        .wi  = wi,
        .f   = f(wo, wi),
        .pdf = pdf(wo, wi)
    };
}