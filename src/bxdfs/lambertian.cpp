#include "bxdfs/lambertian.hpp"
#include "random.hpp"

Vec3 Lambertian::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 sampleDir = surfaceInfo.normal + Random::uniformSphere();

    return (sampleDir.isZero() ? surfaceInfo.normal : sampleDir);
}