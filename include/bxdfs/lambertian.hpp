#pragma once

#include "bxdfs/bxdf.hpp"

class Lambertian: public BxDF {
    Lambertian()                                 = delete;
    Lambertian(const Lambertian&)                = delete;
    Lambertian(Lambertian&&) noexcept            = delete;
    Lambertian& operator=(const Lambertian&)     = delete;
    Lambertian& operator=(Lambertian&&) noexcept = delete;

    public:
        explicit Lambertian(const Vec3& albedo) noexcept;
        ~Lambertian() noexcept override = default;

        Vec3       f(const Vec3& wo, const Vec3& wi) const noexcept override;
        f32        pdf(const Vec3& wo, const Vec3& wi) const noexcept override;
        BxDFSample sample(const Vec3& wo) const override;

    private:
        Vec3 mAlbedo;
};