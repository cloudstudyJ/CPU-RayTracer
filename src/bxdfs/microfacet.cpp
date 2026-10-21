#include "bxdfs/microfacet.hpp"
#include "math/math.hpp"
#include "constants.hpp"
#include "utility.hpp"

namespace microfacet {
    f32 ggxSmithLambda(const Vec3& w, const f32 alpha) noexcept {
        const f32 cosSQ = w.z * w.z;

        if (cosSQ <= 0.0f)
            return INF_F32;

        return 0.5f * (Math::sqrt(1 + alpha * alpha * (Utility::max(1.0f, 1 - cosSQ) / cosSQ)) - 1);
    }
    f32 ggxSmithG1(const Vec3& w, const f32 alpha) noexcept {
        if (w.z <= 0.0f)
            return {};

        return 1 / (1 + ggxSmithLambda(w, alpha));
    }
    // Height-Correlated
    f32 ggxSmithG2(const Vec3& wo, const Vec3& wi, const f32 alpha) noexcept {
        if ((wo.z <= 0.0f) || (wi.z <= 0.0f))
            return {};

        return 1 / (1 + ggxSmithLambda(wo, alpha) + ggxSmithLambda(wi, alpha));
    }
    // Isotropic
    f32 ggxNDF(const Vec3& half, const f32 alpha) noexcept {
        if (half.z <= 0.0f)
            return {};

        const f32 alphaSQ = alpha * alpha;
        const f32 denom   = (half.z * half.z) * (alphaSQ - 1) + 1;

        return (alphaSQ * Math::INV_PI * (denom * denom));
    }
    // Isotropic
    Vec3 ggxVNDF(const Vec3& wo, Vec2 uv, const f32 alpha) noexcept {
        const Vec3 v     = Vec3{ alpha * wo.x, alpha * wo.y, wo.z }.normalized();
        const f32  lenSQ = v.x * v.x + v.y * v.y;

        const Vec3 t1 = (
            (lenSQ > 0.0f)
                ? Vec3{ -v.y, v.x , 0.0f } / Math::sqrt(lenSQ)
                : Vec3{ 1.0f, 0.0f, 0.0f }
        );
        const Vec3 t2 = Vec3::cross(v, t1);
        const f32  s  = 0.5f * (1 + v.z);
        uv.y = (1 - s) * Math::sqrt(Utility::max(0.0f, 1 - uv.x * uv.x)) + s * uv.y;

        const f32  z    = Math::sqrt(Utility::max(0.0f, 1 - uv.x * uv.x - uv.y * uv.y));
        const Vec3 half = t1 * uv.x + t2 * uv.y + v * z;

        return Vec3{
            alpha * half.x,
            alpha * half.y,
            Utility::max(0.0f, half.z)
        }.normalized();
    }
    Vec3 fresnelSchlick(const Vec3& f0, const f32 cos) noexcept {
        const f32 x  = Utility::clamp(1 - cos, 0.0f, 1.0f);
        const f32 x2 = x * x;
        const f32 x5 = x2 * x2 * x;

        return f0 + ((Vec3{ 1.0f, 1.0f, 1.0f } - f0) * x5);
    }
}