#include "bxdfs/lambertian.hpp"
#include "random.hpp"
#include "onb.hpp"
#include "utility.hpp"

f32 Lambertian::eval() const noexcept { return Math::INV_PI; }
f32 Lambertian::pdf(const Vec3& wi, const Vec3& n) const noexcept { return Utility::max(0.0f, Vec3::dot(n, wi)) * Math::INV_PI; }

SampleResult Lambertian::sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept {
    Vec3 w = ONB(surfaceInfo.normal).transform(Random::cosWeightedHemisphere());

    return {
        .w   = w,
        .pdf = pdf(w, surfaceInfo.normal)
    };
}