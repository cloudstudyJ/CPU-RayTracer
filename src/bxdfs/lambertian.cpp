#include "bxdfs/lambertian.hpp"
#include "random.hpp"

Vec3 Lambertian::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept { return Random::uniformHemisphere(surfaceInfo.normal); }