#include "bxdfs/metal.hpp"
#include "sampling/geometry.hpp"

// temp
f32 Metal::eval() const noexcept { return 1.0f; }
f32 Metal::pdf(const Vec3& wi, const Vec3& n) const noexcept { return 1.0f; }

SampleResult Metal::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 sampleDir = Vec3::reflect(wi, surfaceInfo.normal);
    sampleDir += (sampling::uniformSphere() * mRoughness);
    sampleDir = ((Vec3::dot(sampleDir, surfaceInfo.normal) < 0.0f) ? Vec3{} : sampleDir.normalized());

    return {
        .w   = sampleDir,
        .pdf = pdf(wi, surfaceInfo.normal)
    };
}