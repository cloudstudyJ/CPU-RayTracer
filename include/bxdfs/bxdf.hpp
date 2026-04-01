#pragma once

#include "HitResult.hpp"

struct SampleResult {
    Vec3 w;
    f32  pdf{};
};

class BxDF {
    public:
        virtual f32 eval() const noexcept = 0;
        virtual f32 pdf(const Vec3& wi, const Vec3& n) const noexcept = 0;

        virtual SampleResult sample(const Vec3& wi, const HitResult& surfaceInfo) const noexcept = 0;
};