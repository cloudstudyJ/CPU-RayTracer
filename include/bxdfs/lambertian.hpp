#pragma once

#include "bxdfs/bxdf.hpp"

class Lambertian: public BxDF {
    public:
        f32 eval() const noexcept override;
        f32 pdf(const Vec3& wi, const Vec3& n) const noexcept override;

        SampleResult sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;
};