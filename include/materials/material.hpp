#pragma once

#include "sampleResult.hpp"
#include "hitResult.hpp"

class Material {
    public:
        virtual ~Material() noexcept = default;

        virtual SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) const = 0;
};