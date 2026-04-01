#pragma once

#include "bxdfs/bxdf.hpp"

class Metal: public BxDF {
    public:
        Metal() noexcept: mRoughness(0.0f) {}
        Metal(const f32 roughness) noexcept: mRoughness(roughness) {}

        f32 eval() const noexcept override;
        f32 pdf(const Vec3& wi, const Vec3& n) const noexcept override;

        SampleResult sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;

    private:
        f32 mRoughness;
};