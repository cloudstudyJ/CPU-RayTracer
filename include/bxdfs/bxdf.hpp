#pragma once

#include "math/vec3.hpp"

struct BxDFSample {
    Vec3 wi;        // Direction to incoming radiance src (new sampled)
    Vec3 f;
    f32  pdf{};
};

class BxDF {
    BxDF(const BxDF&)                = delete;
    BxDF(BxDF&&) noexcept            = delete;
    BxDF& operator=(const BxDF&)     = delete;
    BxDF& operator=(BxDF&&) noexcept = delete;

    public:
        virtual ~BxDF() noexcept = default;

        virtual Vec3       f(const Vec3& wo, const Vec3& wi) const noexcept = 0;
        virtual f32        pdf(const Vec3& wo, const Vec3& wi) const noexcept = 0;
        virtual BxDFSample sample(const Vec3& wo) const = 0;

    protected:
        BxDF() noexcept = default;
};