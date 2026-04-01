#pragma once

#include "bxdfs/bxdf.hpp"

class Dielectric: public BxDF {
    public:
        Dielectric() noexcept: mRefractiveIndex(1.0f) {}
        Dielectric(const f32 refractiveIndex): mRefractiveIndex(refractiveIndex) {}

        f32 eval() const noexcept override;
        f32 pdf(const Vec3& wi, const Vec3& n) const noexcept override;

        SampleResult sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;

    private:
        f32 mRefractiveIndex;
};