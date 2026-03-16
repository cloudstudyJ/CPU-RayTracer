#pragma once

#include "bxdfs/bxdf.hpp"

class Metal: public BxDF {
    public:
        Metal() noexcept: mRoughness(0.0f) {}
        Metal(const float32 roughness) noexcept: mRoughness(roughness) {}

        Vec3 sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;

    private:
        float32 mRoughness;
};