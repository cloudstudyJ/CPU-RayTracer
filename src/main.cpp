#include "camera.hpp"
#include "math/math.hpp"
#include "utility.hpp"
#include "random.hpp"
#include "material.hpp"
#include "solidTexture.hpp"
#include "objects.hpp"
#include "bxdfs.hpp"
#include "math/vec2.hpp"
#include "worldBuilder.hpp"
#include "ray.hpp"
#include "jsonParser.hpp"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

struct RenderSettings {
    u32 imgWidth {};
    u32 imgHeight{};
    f32 aspectRatio{};

    u32 samplesPerPixel{};
    u32 maxRayBounces  {};

    u32 threads{};
} renderSettings;

constexpr f32 TWO_PI = Math::PI * 2;
constexpr f32 INV_TWO_PI = 1 / TWO_PI;

constexpr Vec3 WORLD_Y     = {0.0f, 1.0f, 0.0f};
constexpr Vec3 COLOR_BLACK = {0.0f, 0.0f, 0.0f};
constexpr Vec3 COLOR_WHITE = {1.0f, 1.0f, 1.0f};

std::vector<Vec3> framebuffer;
std::vector<byte> snapshot;

World world;

RenderSettings getRenderSettings(const char* file) {
    jsonDocument doc = JsonParser::getDocument(file);

    const jsonValue& renderer = doc["Renderer"];
    const jsonValue& sampling = renderer["Sampling"];

    u32 w = JsonParser::toU32(renderer["Resolution"][0]);
    u32 h = JsonParser::toU32(renderer["Resolution"][1]);

    return {
        .imgWidth    = w,
        .imgHeight   = h,
        .aspectRatio = static_cast<f32>(w) / h,

        .samplesPerPixel = JsonParser::toU32(sampling["SamplesPerPixel"]),
        .maxRayBounces   = JsonParser::toU32(sampling["MaxRayBounces"]),

        .threads = Utility::max(1u, std::thread::hardware_concurrency())
    };
}

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        .data = {
            ((linearColor.x > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.x)) : 0.0f),
            ((linearColor.y > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.y)) : 0.0f),
            ((linearColor.z > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.z)) : 0.0f)
        }
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

    f32  u = Random::uniform(-0.5f, 0.5f);
    f32  v = Random::uniform(-0.5f, 0.5f);
    Vec3 n = Vec3::cross(edgeU, edgeV);
    f32  a = n.length();

    return {
        .p   = rect.transform().position + (edgeU * u) + (edgeV * v),
        .n   = n / a,
        .pdf = 1.0f / a
    };
}

// Black Backround
Vec3 missShader() {
    return COLOR_BLACK;
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult, u16 depth = 1) {
    if (depth > renderSettings.maxRayBounces)
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

            L_i = (hitLight ? Vec3{} : hitShader(wiRay, wiHitResult, depth + 1));
        }
        else
            L_i = missShader();

        f32 cosTheta   = Utility::max(0.0f, Vec3::dot(hitResult.normal, wi));
        f32 pdf        = sampleResult.pdf;
            L_indirect = (f * L_i * cosTheta) / pdf;
    }

    return L_e + L_direct + L_indirect;
}

int main() {
    renderSettings = std::move(getRenderSettings("../../renderConfig.json"));
    world = std::move(WorldBuilder::buildWorld("../../resources/worlds/cornellBox.json"));

    framebuffer.resize(renderSettings.imgWidth * renderSettings.imgHeight);
    snapshot.resize(renderSettings.imgWidth * renderSettings.imgHeight * 3);

    Camera camera = {
        .pos  = { .data = { 0.0f, 0.0f, 3.9f } },
        .look = { .data = { 0.0f, 0.0f, 0.0f } },

        .yfov = 40.0f
    };

    Vec3 nz = camera.pos - camera.look;
    f32 focalLength = nz.length();

    Vec3 z = nz.normalized();
    Vec3 x = Vec3::cross(WORLD_Y, z).normalized();
    Vec3 y = Vec3::cross(z, x);

    Vec2 viewport;
    viewport.y = 2 * focalLength * Math::tan(Math::toRad(camera.yfov / 2));
    viewport.x = viewport.y * renderSettings.aspectRatio;

    Vec3 viewportU = x * viewport.x;
    Vec3 viewportV = y * -viewport.y;

    Vec3 viewportdU = viewportU / renderSettings.imgWidth;
    Vec3 viewportdV = viewportV / renderSettings.imgHeight;
    Vec3 viewportLT = camera.pos - (viewportU / 2.0f) - (viewportV / 2.0f) - nz;

    const f32 SQRT_SPP       = static_cast<u16>(Math::sqrt(renderSettings.samplesPerPixel));
    const f32 INV_SQRT_SPP   = 1 / SQRT_SPP;
    const f32 SQRT_SPP_SCALE = 1 / (SQRT_SPP * SQRT_SPP);

    std::vector<std::thread> threads;
    threads.reserve(renderSettings.threads);

    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    {
        std::atomic<u32> nextChunk{};

        auto render = [&]() {
            while (true) {
                u32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
                u32 yTo   = Utility::min(yFrom + 1, renderSettings.imgHeight);

                if (yFrom >= renderSettings.imgHeight)
                    break;

                for (u32 y = yFrom; y < yTo; ++y) {
                    for (u32 x = 0; x < renderSettings.imgWidth; ++x) {
                        Vec3 color;

                        for (u16 sampleY = 0; sampleY < SQRT_SPP; ++sampleY) {
                            for (u16 sampleX = 0; sampleX < SQRT_SPP; ++sampleX) {
                                f32 offsetX = x + ((sampleX + Random::uniform()) * INV_SQRT_SPP);
                                f32 offsetY = y + ((sampleY + Random::uniform()) * INV_SQRT_SPP);

                                Vec3 samplePos = viewportLT + (viewportdU * offsetX) + (viewportdV * offsetY);
                                Ray  ray       = {camera.pos, samplePos - camera.pos};

                                HitResult hitResult = world.traceRay(ray, RAY_T_MIN, INF_F32);
                                color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader());
                            }
                        }
                        color *= SQRT_SPP_SCALE;

                        if (color.r != color.r) color.r = 0.0f;
                        if (color.g != color.g) color.g = 0.0f;
                        if (color.b != color.b) color.b = 0.0f;

                        u32 idx = (y * renderSettings.imgWidth + x);

                        framebuffer[idx] = color;
                        color = gammaCorrection(color);
                        color.r = Utility::clamp(color.x, 0.0f, 1.0f);
                        color.g = Utility::clamp(color.y, 0.0f, 1.0f);
                        color.b = Utility::clamp(color.z, 0.0f, 1.0f);

                        snapshot[idx * 3    ] = static_cast<byte>(255.999f * color.r);
                        snapshot[idx * 3 + 1] = static_cast<byte>(255.999f * color.g);
                        snapshot[idx * 3 + 2] = static_cast<byte>(255.999f * color.b);
                    }
                }
            }
        };

        for (u16 i = 0; i < renderSettings.threads; ++i)
            threads.emplace_back(render);

        for (auto& thread: threads)
            thread.join();
    }
    end = std::chrono::steady_clock::now();

    std::ofstream writer("result.ppm", std::ios::binary);

    writer << "P6\n";
    writer << renderSettings.imgWidth << ' ' << renderSettings.imgHeight << '\n';
    writer << "255\n";
    writer.write(reinterpret_cast<const char*>(snapshot.data()), snapshot.size());

    std::cout << "Threads: " << renderSettings.threads << '\n';
    std::cout << "Resolution: " << renderSettings.imgWidth << 'x' << renderSettings.imgHeight << '\n';
    std::cout << "Max Ray Bounces: " << renderSettings.maxRayBounces << '\n';
    std::cout << "Samples Per Pixel: " << renderSettings.samplesPerPixel << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}