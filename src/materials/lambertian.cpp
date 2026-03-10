#include "materials/lambertian.hpp"

#include "random.hpp"

SampleResult Lambertian::sample(const Ray& wi, const HitResult& surfaceInfo) const {
    SampleResult sampleResult;

    Vec3 sampleDir = surfaceInfo.normal + Random::uniformSphere();
    if (sampleDir.isZero())
        sampleDir = surfaceInfo.normal;

    sampleResult.wo     = {surfaceInfo.hitPoint, sampleDir};
    sampleResult.albedo = albedo;

    return sampleResult;
}