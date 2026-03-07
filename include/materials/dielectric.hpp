#include "materials/material.hpp"

struct Dielectric: public Material {
    public:
        Dielectric() noexcept {}
        Dielectric(const float32 refractiveIndex) noexcept: refractiveIndex(refractiveIndex) {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) override;

    public:
        float32 refractiveIndex{};
};