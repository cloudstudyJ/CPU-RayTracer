#pragma once

#include "HitResult.hpp"

class BxDF {
    public:
        virtual Vec3 sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept = 0;
};