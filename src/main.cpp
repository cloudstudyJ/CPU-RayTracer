#include "types.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "viewport.hpp"
#include "math.hpp"
#include "sphere.hpp"
#include "hitResult.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include "random.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

constexpr uint16 IMG_W = 400;
constexpr uint16 IMG_H = 225;
constexpr float32 ASPECT_RATIO = static_cast<float32>(IMG_W) / IMG_H;

constexpr uint16  SAMPLES_PER_PIXEL = 100;
constexpr float32 SAMPLE_SCALE      = 1.0f / SAMPLES_PER_PIXEL;

const uint32 THREADS = Utility::max(1u, std::thread::hardware_concurrency());
const uint16 MAX_RAY_BOUNCES = 50;

std::vector<Vec3>  framebuffer(IMG_W * IMG_H);
std::vector<uint8> snapshot(IMG_W * IMG_H * 3);

std::vector<std::unique_ptr<Object>> objects;

Vec3 missShader(const Ray& ray) {
    // move range from [-1, 1] to [0, 1]
    float32 t = 0.5f * (ray.direction.normalize().y + 1.0f);

    // White & SkyBlue
    return Vec3::lerp({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult, uint16 depth = 1) {
    if (depth > MAX_RAY_BOUNCES)
        return {};

    Vec3 scatterDir = hitResult.normal + Random::uniformSphere();
    Ray  scatterRay = {hitResult.hitPoint, scatterDir};

    HitResult bounceHitResult;
    for (const auto& object: objects) {
        HitResult currentHitResult = object->hitTest(scatterRay, RAY_T_MIN, bounceHitResult.hitT);

        if (currentHitResult.isHit)
            bounceHitResult = currentHitResult;
    }

    if (bounceHitResult.isHit)
        return hitShader(scatterRay, bounceHitResult, depth + 1) * 0.5f;

    return missShader(scatterRay) * 0.5f;
}

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        ((linearColor.x > 0.0f) ? Math::sqrt(linearColor.x) : 0.0f),
        ((linearColor.y > 0.0f) ? Math::sqrt(linearColor.y) : 0.0f),
        ((linearColor.z > 0.0f) ? Math::sqrt(linearColor.z) : 0.0f)
    };
}
void takeScreenshot() {
    for (uint16 y = 0; y < IMG_H; ++y) {
        for (uint16 x = 0; x < IMG_W; ++x) {
            const Vec3 color = gammaCorrection(framebuffer[y * IMG_W + x]);

            // convert to 8-bits linear color space [0, 255]
            uint32 idx = (y * IMG_W + x) * 3;
            snapshot[idx    ] = static_cast<uint8>(255.999f * color.r);
            snapshot[idx + 1] = static_cast<uint8>(255.999f * color.g);
            snapshot[idx + 2] = static_cast<uint8>(255.999f * color.b);
        }
    }
}
void writeImage() {
    std::ofstream writer("result.ppm", std::ios::binary);

    // write header
    writer << "P6\n";
    writer << IMG_W << ' ' << IMG_H << '\n';
    writer << 255 << '\n';

    // write image data
    writer.write(reinterpret_cast<char*>(snapshot.data()), snapshot.size());
}

int main() {
    Camera camera;
    Viewport viewport;

    viewport.height = 2 * viewport.focalLength * Math::tan(camera.yFOV / 2);
    viewport.width  = viewport.height * ASPECT_RATIO;

    Vec3 dUV = Vec3{viewport.width / IMG_W, (-viewport.height) / IMG_H, 0.0f};
    Vec3 viewportLT = camera.pos + Vec3{(-viewport.width) / 2, viewport.height / 2, -viewport.focalLength};
    Vec3 pixelOriginCenter = viewportLT + (dUV * 0.5f);

    objects.push_back(std::make_unique<Sphere>(Vec3{0.0f, 0.0f, -1.0f}, 0.5f));
    objects.push_back(std::make_unique<Sphere>(Vec3{0.0f, -100.5f, -1.0f}, 100.0f));

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    {
        std::atomic<uint32> nextChunk{};

        auto render = [&]() {
            while (true) {
                uint32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
                uint32 yTo   = Utility::min(static_cast<uint16>(yFrom + 1), IMG_H);

                if (yFrom >= IMG_H)
                    break;

                for (uint32 y = yFrom; y < yTo; ++y) {
                    for (uint32 x = 0; x < IMG_W; ++x) {
                        Vec3 color;

                        for (uint16 sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
                            Vec3 jitter    = {(x + Random::uniform()) * dUV.x, (y + Random::uniform()) * dUV.y, 0.0f};
                            Vec3 samplePos = viewportLT + jitter;
                            Ray  ray       = {camera.pos, samplePos - camera.pos};

                            HitResult hitResult;
                            for (const auto& object: objects) {
                                HitResult currentHitResult = object->hitTest(ray, RAY_T_MIN, hitResult.hitT);

                                if (currentHitResult.isHit)
                                    hitResult = currentHitResult;
                            }

                            color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));
                        }

                        framebuffer[y * IMG_W + x] = color * SAMPLE_SCALE;
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

    takeScreenshot();
    writeImage();

    std::cout << "Threads: " << THREADS << '\n';
    std::cout << "Max Ray-Bounces: " << MAX_RAY_BOUNCES << '\n';
    std::cout << "Resolution: " << IMG_W << 'x' << IMG_H << '\n';
    std::cout << "SPP: " << SAMPLES_PER_PIXEL << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}