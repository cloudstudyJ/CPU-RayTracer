#pragma once

#include "materials/material.hpp"

struct Metal: public Material {
    public:
        Metal() noexcept: roughness(0.1f) {}
        Metal(const Vec3& albedo, const float32 roughness) noexcept: albedo(albedo), roughness(roughness) {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) const override;

    public:
        float32 roughness;

        Vec3 albedo;
};