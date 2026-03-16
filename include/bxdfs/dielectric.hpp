#pragma once

#include "bxdfs/bxdf.hpp"

class Dielectric: public BxDF {
    public:
        Dielectric() noexcept: mRefractiveIndex(1.0f) {}
        Dielectric(const float32 refractiveIndex): mRefractiveIndex(refractiveIndex) {}

        Vec3 sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;

    private:
        float32 mRefractiveIndex;
};