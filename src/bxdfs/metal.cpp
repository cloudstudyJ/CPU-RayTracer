#include "bxdfs/metal.hpp"
#include "random.hpp"

Vec3 Metal::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 sampleDir = Vec3::reflect(wi, surfaceInfo.normal);
    sampleDir += (Random::uniformSphere() * mRoughness);

    return ((Vec3::dot(sampleDir, surfaceInfo.normal) < 0.0f) ? Vec3{} : sampleDir.normalize());
}