#include "bxdfs/dielectric.hpp"
#include "sampling/random.hpp"

namespace {
    f32 schlickApprox(const f32 cos, const f32 refractiveIndex) noexcept {
        f32 r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
        r0 *= r0;

        f32 x  = 1 - cos;
        f32 x2 = x * x;

        return r0 + (1 - r0) * (x2 * x2 * x);
    }
}

// temp
f32 Dielectric::eval() const noexcept { return 1.0f; }
f32 Dielectric::pdf(const Vec3& wi, const Vec3& n) const noexcept { return 1.0f; }

SampleResult Dielectric::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 in = wi.normalized();
    f32 inCos = Vec3::dot(-in, surfaceInfo.normal);
    f32 inSin = Math::sqrt(1 - inCos * inCos);
    f32 eta   = (surfaceInfo.isRayOppositeNormal ? (1 / mRefractiveIndex) : mRefractiveIndex);

    Vec3 out;
    f32  pdf{};
    f32  fresnel = schlickApprox(inCos, mRefractiveIndex);

    // reflections
    if ((eta * inSin > 1.0f)) {
        out = Vec3::reflect(in, surfaceInfo.normal);
        pdf = 1.0f;
    }
    else if (fresnel > sampling::uniform()) {
        out = Vec3::reflect(in, surfaceInfo.normal);
        pdf = fresnel;
    }

    // refraction
    else {
        Vec3 inPerp  = in + (surfaceInfo.normal * inCos);
        Vec3 outPerp = inPerp * eta;
        Vec3 outPara = -surfaceInfo.normal * (Math::sqrt(1 - outPerp.lengthSquare()));

        out = outPerp + outPara;
        pdf = 1 - fresnel;
    }

    return {
        .w   = out,
        .pdf = pdf
    };
}