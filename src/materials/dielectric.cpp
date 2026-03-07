#include "materials/dielectric.hpp"
#include "math.hpp"
#include "random.hpp"

float32 schlickApprox(const float32 cos, float refractiveIndex) {
    float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 *= r0;

    return r0 + (1 - r0) * Math::pow(1 - cos, 5);
}

SampleResult Dielectric::sample(const Ray& wi, const HitResult& surfaceInfo) {
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