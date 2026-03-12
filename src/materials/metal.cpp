#include "materials/metal.hpp"

#include "random.hpp"

SampleResult Metal::sample(const Ray& wi, const HitResult& surfaceInfo) const {
    SampleResult sampleResult;

    Vec3 sampleDir = Vec3::reflect(wi.direction, surfaceInfo.normal).normalize();
    sampleDir += (Random::uniformSphere() * mRoughness);

    sampleResult.wo = {surfaceInfo.hitPoint, sampleDir};

    if (Vec3::dot(sampleDir, surfaceInfo.normal) > 0.0f)
        sampleResult.albedo = mTexture->color(surfaceInfo.u, surfaceInfo.v);

    return sampleResult;
}