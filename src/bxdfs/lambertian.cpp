#include "bxdfs/lambertian.hpp"
#include "sampling/importance.hpp"
#include "utility.hpp"

Lambertian::Lambertian(const Vec3& albedo) noexcept
    : BxDF()
    , mAlbedo(albedo) { }

Vec3 Lambertian::f(const Vec3& wo, const Vec3& wi) const noexcept {
    if ((wo.z <= 0.0f) || (wi.z <= 0.0f))
        return {};

    return (mAlbedo * Math::INV_PI);
}
f32 Lambertian::pdf(const Vec3& wo, const Vec3& wi) const noexcept {
    if (wo.z <= 0.0f)
        return {};

    return (Utility::max(0.0f, wi.z) * Math::INV_PI);
}
BxDFSample Lambertian::sample(const Vec3& wo) const {
    if (wo.z <= 0.0f)
        return {};

    const Vec3 wi = sampling::cosHemisphere();

    return {
        .wi  = wi,
        .f   = f(wo, wi),
        .pdf = pdf(wo, wi)
    };
}