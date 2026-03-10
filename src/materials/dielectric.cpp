#include "materials/dielectric.hpp"
#include "math.hpp"
#include "random.hpp"

float32 schlickApprox(const float32 cos, const float32 refractiveIndex) noexcept {
    float32 r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 *= r0;

    float32 x  = 1 - cos;
    float32 x2 = x * x;

    return r0 + (1 - r0) * (x2 * x2 * x);
}

SampleResult Dielectric::sample(const Ray& wi, const HitResult& surfaceInfo) const {
    SampleResult sampleResult;

    Vec3 in = wi.direction.normalize();
    float32 inCos = Vec3::dot(-in, surfaceInfo.normal);
    float32 inSin = Math::sqrt(1 - inCos * inCos);
    float32 eta   = (surfaceInfo.wasRayInside ? refractiveIndex : (1 / refractiveIndex));

    Vec3 out;

    // reflection
    if ((eta * inSin > 1.0f) || (schlickApprox(inCos, refractiveIndex) > Random::uniform()))
        out = Vec3::reflect(in, surfaceInfo.normal);

    // refraction
    else {
        Vec3 inPerp  = in + (surfaceInfo.normal * inCos);
        Vec3 outPerp = inPerp * eta;
        Vec3 outPara = -surfaceInfo.normal * (Math::sqrt(1 - outPerp.lengthSquare()));

        out = outPerp + outPara;
    }

    sampleResult.albedo = {1.0f, 1.0f, 1.0f};
    sampleResult.wo     = {surfaceInfo.hitPoint, out};

    return sampleResult;
}