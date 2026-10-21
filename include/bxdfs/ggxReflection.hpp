#pragma once

#include "bxdfs/bxdf.hpp"

class GGXReflection: public BxDF {
    GGXReflection()                                    = delete;
    GGXReflection(const GGXReflection&)                = delete;
    GGXReflection(GGXReflection&&) noexcept            = delete;
    GGXReflection& operator=(const GGXReflection&)     = delete;
    GGXReflection& operator=(GGXReflection&&) noexcept = delete;

    public:
        explicit GGXReflection(const Vec3& f0, const f32 roughness) noexcept;
        virtual ~GGXReflection() noexcept = default;

        Vec3       f(const Vec3& wo, const Vec3& wi) const noexcept override;
        f32        pdf(const Vec3& wo, const Vec3& wi) const noexcept override;
        BxDFSample sample(const Vec3& wo) const override;

    private:
        Vec3 mF0;
        f32  mAlpha{};
};