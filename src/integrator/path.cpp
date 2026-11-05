#include "integrator/path.hpp"
#include "world.hpp"
#include "ray.hpp"
#include "onb.hpp"
#include "material/material.hpp"
#include "sampling/mis.hpp"

struct RayPayload {
    Ray  ray;
    Vec3 throughput;
    Vec3 radiance;
    f32  prevPDF   {};  // bxdfSample pdf
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
                if (hitObject != light)
                    continue;

                const Vec3 Le        = lightInfo.color * lightInfo.intensity;
                f32        misWeight = 1.0f;

                if (depth > 0) {
                    const Vec3 wi  = hitResult.hitPoint - rayPayload.ray.origin;
                    const f32  cos = Math::abs(Vec3::dot(hitResult.normal, -rayPayload.ray.direction));

                    if (cos <= 0.0f)
                        misWeight = 0.0f;

                    else {
                        const f32 lightPdfArea  = lightInfo.pdf(*light);
                        const f32 lightPdfSolid = (lightPdfArea * wi.lengthSquare()) / cos;
                        misWeight = sampling::powerHeuristic(rayPayload.prevPDF, lightPdfSolid);
                    }
                }

                rayPayload.radiance += (rayPayload.throughput * Le * misWeight);
                break;
            }

            if (!material)
                break;

            Vec3 normal = hitResult.normal;

            if (material->hasNormalMap()) {
                const Vec3 localN = material->normal(hitResult.uv);

                normal = (
                    hitResult.tangent   * localN.x +
                    hitResult.bitangent * localN.y +
                    hitResult.normal    * localN.z
                ).normalized();
            }

            const ONB  onb = ONB(normal);
            const Vec3 wo  = onb.toLocal(-rayPayload.ray.direction);

            // Direct Estimation
            for (const auto& [light, lightInfo]: lightList) {
                if (hitObject == light)
                    continue;

                const SurfaceSample lightSample = lightInfo.sample(*light);

                if ((lightSample.pdf <= 0.0f))
                    continue;

                Vec3       wiLight  = lightSample.point - hitResult.hitPoint;
                const f32  wiLength = wiLight.length();

                if (wiLength <= RAY_T_MIN)
                    continue;

                wiLight /= wiLength;

                f32 cosGeometry = Vec3::dot(hitResult.normal, wiLight);
                f32 cosSurface  = Vec3::dot(normal,  wiLight);
                f32 cosLightSurface = Math::abs(Vec3::dot(lightSample.normal, -wiLight));

                if ((cosGeometry <= 0.0f) || (cosSurface <= 0.0f) || (cosLightSurface <= 0.0f))
                    continue;

                Ray shadowRay = { hitResult.hitPoint, wiLight };

                if (!world.isOccluded(shadowRay, RAY_T_MIN, wiLength - RAY_T_MIN)) {
                    const Vec3 wiLocal   = onb.toLocal(wiLight);
                    const Vec3 L_le      = lightInfo.color * lightInfo.intensity;
                    const Vec3 f         = material->evaluate(hitResult, wo, wiLocal);
                    const f32  pdfLight  = (wiLength * wiLength * lightSample.pdf) / cosLightSurface;
                    const f32  pdfBxDF   = material->pdf(hitResult, wo, wiLocal);
                    const f32  misWeight = sampling::powerHeuristic(pdfLight, pdfBxDF);
                    const Vec3 L_direct  = (f * L_le * cosSurface * misWeight) / pdfLight;

                    rayPayload.radiance += (rayPayload.throughput * L_direct);
                }
            }

            // Next Estimation Ready
            const BxDFSample bxdfSample = material->sample(hitResult, wo);

            if (bxdfSample.pdf <= 0.0f)
                break;

            const Vec3 wiWorld = onb.toWorld(bxdfSample.wi);

            if (Vec3::dot(hitResult.normal, wiWorld) <= 0.0f)
                break;

            rayPayload.ray         = { hitResult.hitPoint, wiWorld };
            rayPayload.throughput *= ((bxdfSample.f * Math::abs(bxdfSample.wi.z)) / bxdfSample.pdf);
            rayPayload.prevPDF     = bxdfSample.pdf;
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
