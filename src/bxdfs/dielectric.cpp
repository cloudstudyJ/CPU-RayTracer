#include "bxdfs/dielectric.hpp"
#include "random.hpp"

f32 schlickApprox(const f32 cos, const f32 refractiveIndex) noexcept {
    f32 r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 *= r0;

    f32 x  = 1 - cos;
    f32 x2 = x * x;

    return r0 + (1 - r0) * (x2 * x2 * x);
}

Vec3 Dielectric::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 in = wi.normalize();
    f32 inCos = Vec3::dot(-in, surfaceInfo.normal);
    f32 inSin = Math::sqrt(1 - inCos * inCos);
    f32 eta   = (surfaceInfo.isRayOppositeNormal ? (1 / mRefractiveIndex) : mRefractiveIndex);

    Vec3 out;

    // reflection
    if ((eta * inSin > 1.0f) || (schlickApprox(inCos, mRefractiveIndex) > Random::uniform()))
        out = Vec3::reflect(in, surfaceInfo.normal);

    // refraction
    else {
        Vec3 inPerp  = in + (surfaceInfo.normal * inCos);
        Vec3 outPerp = inPerp * eta;
        Vec3 outPara = -surfaceInfo.normal * (Math::sqrt(1 - outPerp.lengthSquare()));

        out = outPerp + outPara;
    }

    return out;
}