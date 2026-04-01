#include "bxdfs/lambertian.hpp"
#include "random.hpp"
#include "onb.hpp"

Vec3 Lambertian::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept { return ONB(surfaceInfo.normal).transform(Random::uniformHemisphere()); }