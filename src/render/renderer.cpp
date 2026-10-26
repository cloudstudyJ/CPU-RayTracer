#include "render/renderer.hpp"
#include "sampling/random.hpp"
#include "ray.hpp"
#include "world.hpp"
#include "jsonParser.hpp"

#include "rapidjson/document.h"

#include <thread>
#include <atomic>

constexpr Vec3 WORLD_Y = { 0.0f, 1.0f, 0.0f };

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

Renderer::Renderer(const char* renderConfigPath) noexcept
    : mConfig(getRenderConfig(renderConfigPath))
    , mIntegrator(mConfig.maxRayDepth) {
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

Vec3 Renderer::renderPixel(const World& world, const Camera& camera, const Viewport& viewport, const u32 x, const u32 y) noexcept {
    const u32 SQRT_SPP       = static_cast<u16>(Math::sqrt(mConfig.spp));
    const f32 INV_SQRT_SPP   = 1.0f / SQRT_SPP;
    const f32 SQRT_SPP_SCALE = INV_SQRT_SPP * INV_SQRT_SPP;

    Vec3 radiance;

    for (u16 sampleY = 0; sampleY < SQRT_SPP; ++sampleY) {
        for (u16 sampleX = 0; sampleX < SQRT_SPP; ++sampleX) {
            f32 offsetX = x + ((sampleX + sampling::uniform()) * INV_SQRT_SPP);
            f32 offsetY = y + ((sampleY + sampling::uniform()) * INV_SQRT_SPP);

            Vec3 samplePos  = viewport.lt + (viewport.du * offsetX) + (viewport.dv * offsetY);
            Ray  ray        = { camera.position, (samplePos - camera.position).normalized() };
            radiance       += mIntegrator.Li(world, ray);
        }
    }

    return radiance * SQRT_SPP_SCALE;
}
