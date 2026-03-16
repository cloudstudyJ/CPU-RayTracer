#pragma once

#include "bxdfs/bxdf.hpp"

class Lambertian: public BxDF {
    public:
        Vec3 sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept override;
};