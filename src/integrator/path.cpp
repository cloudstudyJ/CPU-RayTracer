#include "integrator/path.hpp"
#include "world.hpp"
#include "ray.hpp"
#include "onb.hpp"
#include "material/material.hpp"

struct RayPayload {
    Ray  ray;
    Vec3 throughput;
    Vec3 radiance;
};

namespace integrator {
    Path::Path(const u32 maxRayDepth) noexcept
        : mMaxRayDepth(maxRayDepth) { }

    Vec3 Path::Li(const World& world, const Ray& ray) const {
        RayPayload rayPayload = {
            .ray        = ray,
            .throughput = { 1.0f, 1.0f, 1.0f }
        };

        const auto& lightList = world.getLights();

        for (u32 depth = 0; depth < mMaxRayDepth; ++depth) {
            HitResult hitResult = world.traceRay(rayPayload.ray, RAY_T_MIN, INF_F32);

            // Environment
            if (!hitResult.isHit) {
                rayPayload.radiance += (rayPayload.throughput * missPass());
                break;
            }

            const auto& hitObject = hitResult.object;
            const auto& material  = hitObject->material();

            // Emission Accumulation
            for (const auto& [light, lightInfo]: lightList) {
                if (hitObject == light) {
                    if (depth == 0)
                        rayPayload.radiance += (rayPayload.throughput * (lightInfo.color * lightInfo.intensity));

                    break;
                }
            }

            if (!material)
                break;

            const ONB  onb = ONB(hitResult.normal);
            const Vec3 wo  = onb.toLocal(-rayPayload.ray.direction);

            // Direct Estimation
            for (const auto& [light, lightInfo]: lightList) {
                if (hitObject == light)
                    continue;

                const SurfaceSample lightSample = lightInfo.sample(*light);
                      Vec3          wiLight     = lightSample.point - hitResult.hitPoint;
                const f32           wiLength    = wiLight.length();

                if (wiLength <= RAY_T_MIN)
                    continue;

                wiLight /= wiLength;

                f32 cosSurface      = Vec3::dot(hitResult.normal  ,  wiLight);
                f32 cosLightSurface = Vec3::dot(lightSample.normal, -wiLight);

                if ((cosSurface <= 0.0f) || (cosLightSurface <= 0.0f) || (lightSample.pdf <= 0.0f))
                    continue;

                Ray shadowRay = { hitResult.hitPoint, wiLight };

                if (!world.isOccluded(shadowRay, RAY_T_MIN, wiLength - RAY_T_MIN)) {
                    const Vec3 L_le     = lightInfo.color * lightInfo.intensity;
                    const Vec3 f        = material->evaluate(hitResult, wo, onb.toLocal(wiLight));
                    const Vec3 L_direct = (f * L_le * cosSurface * cosLightSurface) / (wiLength * wiLength * lightSample.pdf);

                    rayPayload.radiance += (rayPayload.throughput * L_direct);
                }
            }

            // Indirect Estimation
            BxDFSample indirectSample = material->sample(hitResult, wo);

            if (indirectSample.pdf <= 0.0f)
                break;

            rayPayload.throughput *= ((indirectSample.f * Math::abs(indirectSample.wi.z)) / indirectSample.pdf);
            rayPayload.ray         = { hitResult.hitPoint, onb.toWorld(indirectSample.wi) };
        }

        return rayPayload.radiance;
    }

    Vec3 Path::hitPass() const {
        return {};
    }
    Vec3 Path::missPass() const {
        // BLACK_BACKGROUND
        return {};
    }
}