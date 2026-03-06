#pragma once

#include "materials/material.hpp"

struct Lambertian: public Material {
    public:
        Lambertian() noexcept {}
        Lambertian(const Vec3& albedo) noexcept: albedo(albedo) {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) override;

    public:
        Vec3 albedo;
};