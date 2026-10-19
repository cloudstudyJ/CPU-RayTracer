#include "render/renderer.hpp"
#include "sampling/random.hpp"
#include "objects/rectangle.hpp"
#include "texture.hpp"
#include "bxdfs/bxdf.hpp"
#include "ray.hpp"
#include "world.hpp"
#include "jsonParser.hpp"

#include "rapidjson/document.h"

#include <thread>
#include <atomic>

constexpr Vec3 WORLD_Y     = {0.0f, 1.0f, 0.0f};
constexpr Vec3 COLOR_BLACK = {0.0f, 0.0f, 0.0f};

Renderer::RenderConfig getRenderConfig(const char* file) {
    jsonDocument doc = JsonParser::getDocument(file);

    const jsonValue& renderer = doc["Renderer"];
    const jsonValue& sampling = renderer["Sampling"];

    u32 w = JsonParser::toU32(renderer["Resolution"][0]);
    u32 h = JsonParser::toU32(renderer["Resolution"][1]);

    return {
        .imgW        = w,
        .imgH        = h,
        .aspect      = static_cast<f32>(w) / h,
        .spp         = JsonParser::toU32(sampling["SamplesPerPixel"]),
        .maxRayDepth = JsonParser::toU32(sampling["MaxRayDepth"]),
        .threads     = Utility::max(1u, std::thread::hardware_concurrency())
    };
}

struct Viewport {
    Vec3 lt;
    Vec3 du;
    Vec3 dv;
};
Viewport getViewport(const Camera& camera, const f32 imgW, const f32 imgH) noexcept {
    Vec3 negativeZ   = camera.position - camera.look;
    f32  focalLength = negativeZ.length();

    Vec3 z = negativeZ.normalized();
    Vec3 x = Vec3::cross(WORLD_Y, z).normalized();
    Vec3 y = Vec3::cross(z, x);

    f32  viewportY = 2 * focalLength * Math::tan(Math::toRad(camera.fovY * 0.5f));
    f32  viewportX = viewportY * (static_cast<f32>(imgW) / imgH);
    Vec3 viewportU = x * viewportX;
    Vec3 viewportV = y * -viewportY;

    return {
        .lt = camera.position - (viewportU * 0.5f) - (viewportV * 0.5f) - negativeZ,
        .du = viewportU / imgW,
        .dv = viewportV / imgH
    };
}

struct LightSample {
    Vec3 p;
    Vec3 n;
    f32  pdf{};
};

LightSample sampleRect(const Rectangle& rect) {
    const Vec3 edgeU = rect.edgeU();
    const Vec3 edgeV = rect.edgeV();

    Vec2 uv   = sampling::uniform2D(-0.5f, 0.5f);
    Vec3 n    = Vec3::cross(edgeU, edgeV);
    f32  invA = 1 / n.length();

    return {
        .p   = rect.transform().position + (edgeU * uv.u) + (edgeV * uv.v),
        .n   = n * invA,
        .pdf = invA
    };
}


Renderer::Renderer(const char* renderConfigPath) noexcept
    : mConfig(getRenderConfig(renderConfigPath)) {
    mFrameBuffer.resize(mConfig.imgW * mConfig.imgH);
}

void Renderer::render(const World& world) noexcept {
    std::vector<std::thread> threads;
    threads.reserve(mConfig.threads);

    const Camera&  camera   = world.getMainCamera();
    const Viewport viewport = getViewport(camera, mConfig.imgW, mConfig.imgH);

    std::atomic<u32> nextChunk{};

    auto render = [&]() {
        while (true) {
            u32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
            u32 yTo   = Utility::min(yFrom + 1, mConfig.imgH);

            if (yFrom >= mConfig.imgH)
                break;

            for (u32 y = yFrom; y < yTo; ++y) {
                for (u32 x = 0; x < mConfig.imgW; ++x) {
                    Vec3 color = renderPixel(world, camera, viewport, x, y);

                    if (color.r != color.r) color.r = 0.0f;
                    if (color.g != color.g) color.g = 0.0f;
                    if (color.b != color.b) color.b = 0.0f;

                    mFrameBuffer[(y * mConfig.imgW + x)] = color;
                }
            }
        }
    };

    for (u16 i = 0; i < mConfig.threads; ++i)
        threads.emplace_back(render);

    for (auto& thread: threads)
        thread.join();
}

auto Renderer::renderConfig() const noexcept -> const RenderConfig {
    return mConfig;
}
const std::vector<Vec3> Renderer::frameBuffer() const noexcept {
    return mFrameBuffer;
}

Vec3 Renderer::missPass() noexcept {
    return COLOR_BLACK;
}
Vec3 Renderer::hitPass(const World& world, const Ray& ray, const HitResult& hitResult, u16 depth) noexcept {
    if (depth > mConfig.maxRayDepth)
        return {};

    const auto& hitObject = hitResult.object;
    const auto& material  = hitObject->material();
    const auto& bxdf      = (material ? material->bxdf : nullptr);

    Vec3 L_e;
    Vec3 L_direct;
    Vec3 L_indirect;

    for (const auto& [light, info]: world.getLights()) {
        if (hitObject == light) {
            L_e = info.color * info.intensity;
            break;
        }
    }

    if (bxdf) {
        Vec3 f = material->texture->color(hitResult.u, hitResult.v) * bxdf->eval();

        // Direct Lighting
        for (const auto& [light, info]: world.getLights()) {
            if (hitObject == light)
                continue;

            LightSample lightSample = sampleRect(*static_cast<Rectangle*>(light));
            Vec3        wiLight     = lightSample.p - hitResult.hitPoint;
            f32         r           = wiLight.length();
                        wiLight     = wiLight / r;
            Ray         wiLightRay  = {hitResult.hitPoint, wiLight};
            f32         cosTheta    = Vec3::dot(hitResult.normal, wiLight);
            f32         cosPhi      = Vec3::dot(lightSample.n, -wiLight);

            if ((cosTheta <= 0.0f) || (cosPhi <= 0.0f))
                continue;

            // visbility check
            HitResult shadowHitResult = world.traceRay(wiLightRay, RAY_T_MIN, r - RAY_T_MIN);
            if (!shadowHitResult.isHit) {
                Vec3 L_le = info.color * info.intensity;

                L_direct += (f * L_le * cosTheta * cosPhi) / (r * r * lightSample.pdf);
            }
        }

        // Indirect Lighting
        SampleResult sampleResult = bxdf->sample(ray.direction, hitResult);

        Vec3 wi    = sampleResult.w;
        Ray  wiRay = {hitResult.hitPoint, wi};

        HitResult wiHitResult = world.traceRay(wiRay, RAY_T_MIN, INF_F32);

        Vec3 L_i;
        bool hitLight = false;

        if (wiHitResult.isHit) {
            for (const auto& [light, info]: world.getLights()) {
                if (wiHitResult.object == light) {
                    hitLight = true;
                    break;
                }
            }

            L_i = (hitLight ? Vec3{} : hitPass(world, wiRay, wiHitResult, depth + 1));
        }
        else
            L_i = missPass();

        f32 cosTheta   = Utility::max(0.0f, Vec3::dot(hitResult.normal, wi));
        f32 pdf        = sampleResult.pdf;
            L_indirect = (f * L_i * cosTheta) / pdf;
    }

    return L_e + L_direct + L_indirect;
}

Vec3 Renderer::renderPixel(const World& world, const Camera& camera, const Viewport& viewport, const u32 x, const u32 y) noexcept {
    const u32 SQRT_SPP       = static_cast<u16>(Math::sqrt(mConfig.spp));
    const f32 INV_SQRT_SPP   = 1.0f / SQRT_SPP;
    const f32 SQRT_SPP_SCALE = INV_SQRT_SPP * INV_SQRT_SPP;

    Vec3 color;

    for (u16 sampleY = 0; sampleY < SQRT_SPP; ++sampleY) {
        for (u16 sampleX = 0; sampleX < SQRT_SPP; ++sampleX) {
            f32 offsetX = x + ((sampleX + sampling::uniform()) * INV_SQRT_SPP);
            f32 offsetY = y + ((sampleY + sampling::uniform()) * INV_SQRT_SPP);

            Vec3 samplePos = viewport.lt + (viewport.du * offsetX) + (viewport.dv * offsetY);
            Ray  ray       = {camera.position, samplePos - camera.position};

            HitResult hitResult = world.traceRay(ray, RAY_T_MIN, INF_F32);
            color += (hitResult.isHit ? hitPass(world, ray, hitResult) : missPass());
        }
    }

    return color * SQRT_SPP_SCALE;
}