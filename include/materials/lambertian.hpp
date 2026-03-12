#pragma once

#include "materials/material.hpp"
#include "solidTexture.hpp"

#include <memory>

// fwd
class Texture;
class SolidTexture;

class Lambertian: public Material {
    public:
        Lambertian() noexcept: mTexture(std::make_shared<SolidTexture>(Vec3{0.5f, 0.5f, 0.5f})) {}
        Lambertian(std::shared_ptr<Texture> texture) noexcept: mTexture(texture) {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) const override;

    private:
        std::shared_ptr<Texture> mTexture;
};