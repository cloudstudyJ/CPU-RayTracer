#include "materials/material.hpp"

struct Dielectric: public Material {
    public:
        Dielectric() noexcept: refractiveIndex(1.0f) {}
        Dielectric(const float32 refractiveIndex) noexcept: refractiveIndex(refractiveIndex) {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) const override;

    public:
        float32 refractiveIndex;
};