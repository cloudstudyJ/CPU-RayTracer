#pragma once

#include "materials/material.hpp"
#include "solidTexture.hpp"

#include <memory>

// fwd
class Texture;

class Metal: public Material {
    public:
        Metal() noexcept: mRoughness(0.1f), mTexture(std::make_shared<SolidTexture>(Vec3{0.5f, 0.5f, 0.5f})) {}
        Metal(const float32 roughness, std::shared_ptr<Texture> texture) noexcept: mRoughness(roughness), mTexture(texture)  {}

        SampleResult sample(const Ray& wi, const HitResult& surfaceInfo) const override;

        float32 roughness() const noexcept { return mRoughness; }

    private:
        float32 mRoughness;

        std::shared_ptr<Texture> mTexture;
};