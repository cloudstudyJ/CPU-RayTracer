#include "ray.hpp"
#include "camera.hpp"
#include "viewport.hpp"
#include "math.hpp"
#include "objects/quad.hpp"
#include "hitResult.hpp"
#include "utility.hpp"
#include "random.hpp"
#include "materials/materials.hpp"
#include "world.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

constexpr uint32 IMG_W = 600;
constexpr uint32 IMG_H = 600;
constexpr float32 ASPECT_RATIO = static_cast<float32>(IMG_W) / IMG_H;

constexpr uint32  SAMPLES_PER_PIXEL = 200;
constexpr float32 SAMPLE_SCALE      = 1.0f / SAMPLES_PER_PIXEL;
constexpr uint32 MAX_RAY_BOUNCES = 50;

constexpr Vec3 worldY = {0.0f, 1.0f, 0.0f};

const uint32 THREADS = Utility::max(1u, std::thread::hardware_concurrency());

std::vector<Vec3>  framebuffer(IMG_W * IMG_H);
std::vector<uint8> snapshot(IMG_W * IMG_H * 3);

World world;

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        ((linearColor.x > 0.0f) ? Math::sqrt(linearColor.x) : 0.0f),
        ((linearColor.y > 0.0f) ? Math::sqrt(linearColor.y) : 0.0f),
        ((linearColor.z > 0.0f) ? Math::sqrt(linearColor.z) : 0.0f)
    };
}

Vec3 missShader(const Ray& ray) {
    // move range from [-1, 1] to [0, 1]
    float32 t = 0.5f * (ray.direction.normalize().y + 1.0f);

    // White & SkyBlue
    return Vec3::lerp({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult, uint16 depth = 1) {
    if (depth > MAX_RAY_BOUNCES)
        return {};

    SampleResult sample = hitResult.material->sample(ray, hitResult);

    HitResult bounceHitResult = world.traceRay(sample.wo, RAY_T_MIN, INF_F32);
    if (bounceHitResult.isHit)
        return sample.albedo * hitShader(sample.wo, bounceHitResult, depth + 1);

    return missShader(sample.wo) * sample.albedo;
}

void makeCornellBox() {
    auto green = std::make_shared<Lambertian>(std::make_shared<SolidTexture>(Vec3{0.12f, 0.45f, 0.15f}));
    auto red   = std::make_shared<Lambertian>(std::make_shared<SolidTexture>(Vec3{0.65f, 0.05f, 0.05f}));
    auto white = std::make_shared<Lambertian>(std::make_shared<SolidTexture>(Vec3{0.73f, 0.73f, 0.73f}));

    world.addObject<Quad>(Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, green);
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, red);
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 0.0f}, Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, white);
    world.addObject<Quad>(Vec3{555.0f, 555.0f, 555.0f}, Vec3{-555.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, -555.0f}, white);
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 555.0f}, Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, white);
}

int main() {
    Camera camera;
    camera.yfov = 40.0f;
    camera.pos  = {278.0f, 278.0f, -800.0f};
    camera.look = {278.0f, 278.0f, 0.0f};

    Vec3 nz = camera.pos - camera.look;
    float32 focalLength = nz.length();

    Viewport viewport;
    viewport.height = 2 * focalLength * Math::tan(camera.yfov / 2);
    viewport.width  = viewport.height * ASPECT_RATIO;

    Vec3 z = nz.normalize();
    Vec3 x = Vec3::cross(worldY, z).normalize();
    Vec3 y = Vec3::cross(z, x);

    Vec3 viewportU = x * viewport.width;
    Vec3 viewportV = y * -viewport.height;

    Vec3 viewportdU = viewportU / IMG_W;
    Vec3 viewportdV = viewportV / IMG_H;
    Vec3 viewportLT = camera.pos - (viewportU / 2.0f) - (viewportV / 2.0f) - nz;

    makeCornellBox();
    world.buildAccelerationStructure();

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    {
        std::atomic<uint32> nextChunk{};

        auto render = [&]() {
            while (true) {
                uint32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
                uint32 yTo   = Utility::min(yFrom + 1, IMG_H);

                if (yFrom >= IMG_H)
                    break;

                for (uint32 y = yFrom; y < yTo; ++y) {
                    for (uint32 x = 0; x < IMG_W; ++x) {
                        Vec3 color;

                        for (uint16 sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
                            Vec3 jitter = (viewportdU * (x + Random::uniform())) + (viewportdV * (y + Random::uniform()));
                            Vec3 samplePos = viewportLT + jitter;
                            Ray  ray       = {camera.pos, samplePos - camera.pos};

                            // primary ray
                            HitResult hitResult = world.traceRay(ray, RAY_T_MIN, INF_F32);
                            color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));
                        }
                        color *= SAMPLE_SCALE;

                        uint32 idx = (y * IMG_W + x);

                        framebuffer[idx] = color;
                        color = gammaCorrection(color);

                        snapshot[idx * 3    ] = static_cast<uint8>(255.999f * color.r);
                        snapshot[idx * 3 + 1] = static_cast<uint8>(255.999f * color.g);
                        snapshot[idx * 3 + 2] = static_cast<uint8>(255.999f * color.b);
                    }
                }
            }
        };

        for (uint16 i = 0; i < THREADS; ++i)
            threads.emplace_back(render);

        for (auto& thread: threads)
            thread.join();
    }
    end = std::chrono::steady_clock::now();

    std::ofstream writer("result.ppm", std::ios::binary);

    writer << "P6\n";
    writer << IMG_W << ' ' << IMG_H << '\n';
    writer << "255\n";
    writer.write(reinterpret_cast<const char*>(snapshot.data()), snapshot.size());

    std::cout << "Threads: " << THREADS << '\n';
    std::cout << "Resolution: " << IMG_W << 'x' << IMG_H << '\n';
    std::cout << "Max Ray Bounces: " << MAX_RAY_BOUNCES << '\n';
    std::cout << "Samples Per Pixel: " << SAMPLES_PER_PIXEL << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}